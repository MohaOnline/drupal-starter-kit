<?php

/**
 * The $batch can include the following values. Only 'operations'
 * and 'finished' are required, all others will be set to default values.
 *
 * @param operations
 *   An array of callbacks and arguments for the callbacks.
 *   There can be one callback called one time, one callback
 *   called repeatedly with different arguments, different
 *   callbacks with the same arguments, one callback with no
 *   arguments, etc. (Use an empty array if you want to pass 
 *   no arguments.)
 *
 * @param finished
 *   A callback to be used when the batch finishes.
 *
 * @param title
 *   A title to be displayed to the end user when the batch starts. The default is 'Processing'.
 *
 * @param init_message
 *   An initial message to be displayed to the end user when the batch starts.
 *
 * @param progress_message
 *   A progress message for the end user. Placeholders are available.
 *   Placeholders note the progression by operation, i.e. if there are
 *   2 operations, the message will look like:
 *    'Processed 1 out of 2.'
 *    'Processed 2 out of 2.'
 *   Placeholders include:
 *     @current, @remaining, @total and @percentage
 *
 * @param error_message
 *   The error message that will be displayed to the end user if the batch
 *   fails.
 *
 * @param file
 *   Path to file containing the callbacks declared above. Always needed when
 *   the callbacks are not in a .module file.
 *
 */
function media_unique_batch_generate_sha1($do_sha1, $do_deletes, $options3, $options4) {
  // For this example, we decide that we can safely process
  // 20 entities at a time without a timeout.
  $limit = variable_get('media_unique_batch_limit', 20);
  $bundle = variable_get('media_unique_bundle_to_process', 'image');

  $args = array(
    ':type' => variable_get('media_unique_bundle_to_process', 'image'),
  );
  // With each pass through the callback, retrieve the next group of fids.
  $result = db_query('SELECT fid, uri FROM {file_managed} WHERE type = :type ORDER BY fid ASC', $args)->fetchAll();

  $count_max = 0;
  $operations = NULL;
  $count_ten = 0;
  $fid_array = array();
  $count_for = 0;
  global $user;
  $_SESSION['media_unique_' . $user->name] = 0;
  $_SESSION['media_unique_' . $user->name . '_deletes'] = 0;
  if ($do_sha1) {
    $count_max = count($result);
    foreach ($result as $file) {
      $count_ten++;
      $count_for++;
      $fid_array[] = $file->fid;
      if ($count_ten == $limit) {
        $operations[] = array('__set_sha1_hash_fid_array', array($fid_array));
        $fid_array = array();
        $count_ten = 0;
      }
      if ($count_for == $count_max && $count_max > 0) {
        // Done.
        $operations[] = array('__set_sha1_hash_fid_array', array($fid_array));
        $_SESSION['media_unique_' . $user->name] = $count_max;
      }
    }
  }
  $result_array = array();
  $count_ten = 0;
  if ($do_deletes) {
    $records = __detect_and_prepare_dupes();
    $count_max = count($records);
    foreach ($records as $record) {
      $count_ten++;
      $count_for++;
      $result_array[] = $record;
      if ($count_ten == $limit) {
        $operations[] = array('__delete_dupes', array($result_array));
      }
      if ($count_for == $count_max && $count_max > 0) {
        $operations[] = array('__delete_dupes', array($result_array));
        $_SESSION['media_unique_' . $user->name] = $count_max;
      }
    }
  }
  $batch = array(
    'operations' => $operations,
    'finished' => 'media_unique_batch_finished',
    'title' => t('Processing Media Unique generate sha1 Batch for files of type "@bundle" in multiples of @limit', array(
      '@bundle' => $bundle,
      '@limit' => $limit
      )
    ),
    'init_message' => t('Media Unique Batch is starting.'),
    'progress_message' => t('Processed @current out of @total (multiples of @limit files per batch).', array(
      '@limit' => $limit
      )
    ),
    'error_message' => t('Media Unique generate sha1 Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'media_unique') . '/includes/unique.batch.inc',
  );
  batch_set($batch);

  // If this function was called from a form submit handler, stop here,
  // FAPI will handle calling batch_process().

  // If not called from a submit handler, add the following,
  // noting the url the user should be sent to once the batch
  // is finished.
  // IMPORTANT: 
  // If you set a blank parameter, the batch_process() will cause an infinite loop

  //batch_process('media-unique/batch');
}

/*            $DEBUG = TRUE;
            if ($DEBUG) {
              $replacements = array(
               '@max' => print_r( $context['sandbox']['max'], TRUE),
              );
              watchdog('media_unique', 'Debug record <pre>@max</pre>', $replacements, WATCHDOG_NOTICE);
            }
*/

/**
 * Batch 'finished' callback
 */
function media_unique_batch_finished($success, $results, $operations) {
  if ($success) {
    global $user;
    $count_ops = $_SESSION['media_unique_' . $user->name];
    $count_deletes = $_SESSION['media_unique_' . $user->name . '_deletes'];
            $DEBUG = TRUE;
            if ($DEBUG) {
              $results = array(
               '@max' => print_r( $count_ops, TRUE),
              );
              $operations = array(
               '@max' => print_r( $count_deletes, TRUE),
              );
              watchdog('media_unique', 'Debug ops <pre>@max</pre>', $count_ops, WATCHDOG_NOTICE);
              watchdog('media_unique', 'Debug deletes <pre>@max</pre>', $count_deletes, WATCHDOG_NOTICE);
            }

    // Here we do something meaningful with the results.
    $message_arr['title'] = t('Media unique processing summary');
    $message_arr['items'][0] = t('@count items successfully processed:', array('@count' => $count_ops)); 
    $message_arr['items'][1] = t('@count_deletes successfully deleted.', array('@count_deletes' => $count_deletes));
    // $message .= theme('item_list', $results);  // D6 syntax
    $message = theme('item_list', $message_arr);
    drupal_set_message($message);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
  
}


/**
 * Implements the media_unique entityid page rendering.
 */
function media_unique_batch_page() {
  $sha1_status_message = '';
  if (!user_access('access media unique')) {
    drupal_set_message(t('You do not have access to media unique batch page, check your permissions, contact the administrator.', FALSE, 'error', TRUE));
    drupal_goto('<front>');
  }
  $args = array(
    ':type' => variable_get('media_unique_bundle_to_process', 'image'),
  );
  $queryHashes = db_query('SELECT count(entity_id) FROM {media_unique} where entity_id in (select fid from {file_managed} where type = :type)', $args);
  $result_count = $queryHashes->fetchColumn(0);
  if ($result_count == 0 ) {
    $sha1_status_message = t('Media Unique has not yet processed sha1 for your files, recommend batch operation to do this.'); 
    //  media_unique_batch_generate_sha1(NULL,NULL,NULL,NULL);
  } else {
    $args = array(
      ':type' => 'image',
    );
    //$test = db_query('SELECT COUNT(DISTINCT fid) FROM {file_managed} where type = :type', $args)->fetchField();
    $sha1_status_message = t('Media Unique has already processed sha1 values for !count files.  See your options below or go to the configuration page:',
      array('!count' => $result_count)) . l('/admin/config/media/media_unique', 'admin/config/media/media_unique');
  }

  $batch_form = drupal_get_form('media_unique_batch_sha1_form');
  $batch_form = drupal_render($batch_form);
  return theme('media_unique_batch', array(
    'sha1_status_message' => $sha1_status_message,
    'batch_form' => $batch_form,
    // Variables go here, you can add more variables like json_data.
    // These variables are used in templates/media_unique_batch.tpl.php.
  ));
}


/**
 * Returns the render array for the form.
 */
function media_unique_batch_sha1_form($form, &$form_state) {
  $bundle = variable_get('media_unique_bundle_to_process', 'image');
  $queryImages = db_query('SELECT count(fid) FROM {file_managed} where type = :file_type', array(
    ':file_type' => $bundle)
  );
  $number_of_images = $queryImages->fetchColumn(0);

  $queryHashes = db_query('SELECT count(entity_id) FROM {media_unique}');
  $number_of_files_with_sha1_hash = $queryHashes->fetchColumn(0);

  $status_msg = '';
  $status_msg_checkbox = '';
  $do_sha1_processing = FALSE;
  if ($number_of_images > $number_of_files_with_sha1_hash) {
    $status_msg = t(':totale :type out of :remaining images have sha1 hash keys.', array(
      ':totale' => $number_of_files_with_sha1_hash,
      ':remaining' => ($number_of_images - $number_of_files_with_sha1_hash),
      ':type' => $bundle . 's',
    ));
    $status_msg_checkbox = t('If checked :remaining :type will be processed when <submit> is pressed.', array(
      ':remaining' => ($number_of_images - $number_of_files_with_sha1_hash),
      ':type' => $bundle . 's',
    ));
  }
  if ($number_of_images == $number_of_files_with_sha1_hash) {
    $do_sha1_processing = FALSE;
    $status_msg = t(':totale :type have sha1 keys processed.', array(
      ':totale' => $number_of_images,
      ':remaining' => ($number_of_images - $number_of_files_with_sha1_hash),
      ':type' => $bundle . 's',
    ));
    $status_msg_checkbox = t('If you check this box and press <submit>, all :totale images will be re-processed.', array(
      ':totale' => ($number_of_images),
    ));
  }

  $form['batch'] = array(
    '#type' => 'fieldset',
    '#title' => $status_msg,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['batch']['do_sha1_processing'] = array(
    '#type' => 'checkbox',
    '#title' => ($do_sha1_processing) ? t('Do sha1 processing?') : t('Reprocess all sha1 values?'),
    '#default_value' => $do_sha1_processing,
    '#description' => $status_msg_checkbox,
  );
  $form['batch']['process_sha1_keys_and_delete_dupes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make your media/files unique. WARNING, THIS WILL DELETE DUPLICATE COPIES OF FILES, YOU WILL LOSE FIELD VALUES ON THOSE FILES.'),
    '#default_value' => FALSE,
    '#description' => t('Find and then delete extra copies of files using the sha1 keys.  Duplicate keys indicate that the image is a copy.'),
  );
  $form['batch']['media_unique_bundle_to_process'] = array(
    '#type' => 'textfield',
    '#title' => t('Specify a specific file entity bundle to process.'),
    '#default_value' => variable_get('media_unique_bundle_to_process', 'image'),
    '#description' => t('Process files of the specified file entity type; for example "image" , "video" , "document" etc.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Add a submit handler/function to the form.
 *
 * This will add a completion message to the screen when the
 * form successfully processes
 */
function media_unique_batch_sha1_form_submit($form, &$form_state) {
  $do_sha1 = FALSE;
  $do_deletes = FALSE;
  if ($form_state['values']['process_sha1_keys_and_delete_dupes']) {
    $do_deletes = TRUE;
  }
  if ($form_state['values']['do_sha1_processing']) {
    $do_sha1 = TRUE;
  }
  if (isset($form_state['values']['media_unique_bundle_to_process'])) {
    variable_set('media_unique_bundle_to_process', $form_state['values']['media_unique_bundle_to_process']);
  }
  //drupal_set_message(t('This will take some time.'));
  if ($do_sha1 || $do_deletes) {
    $options3 = array();
    batch_set(media_unique_batch_generate_sha1($do_sha1,$do_deletes,$options3,NULL));
  }
}


/**
 * Find a duplicate for fid using sha1 key.
 */
function __detect_and_prepare_dupes() {
  $queryCopies = db_query('select GROUP_CONCAT(entity_id) eids, sha1, count(*) cnt FROM {media_unique} GROUP BY sha1 HAVING cnt > 1');
  $resultCopies = $queryCopies->fetchAll();
  return $resultCopies;
}

function __delete_dupes($copies_entityids_sha1_count) {
  if (!user_access('access media unique')) {
    drupal_set_message(t('You do not have access to media unique, check your permissions, contact the administrator.', FALSE, 'error', TRUE));
    drupal_goto('<front>');
  }
  global $user;
  $fid_pointer = 0;
  $uri = '';
  $file_path = '';

  $count = 0;
  $report_deletes = 0;
  foreach($copies_entityids_sha1_count as $entityids_sha1_count) {
    $cnt = $entityids_sha1_count->cnt;
    $eids = $entityids_sha1_count->eids;
    $entityids = explode(',', $eids);
    $sha1 = $entityids_sha1_count->sha1;
    $del_cnt = 0;
    while($cnt > $del_cnt ) {
      if (isset($entityids[$del_cnt]) && ($del_cnt+1) < $cnt) {
        $entity_id = $entityids[$del_cnt];
        $file_to_delete = file_load($entity_id);
        if ($file_to_delete !== FALSE) {
          $is_deleted = file_delete($file_to_delete);
          if ($is_deleted) {
            $_SESSION['media_unique_' . $user->name . '_deletes']++;
            // Remove the corresponding media_unique record.
            $delete_ok = db_delete('media_unique')
              ->condition('entity_id', $entity_id, '=')
              ->execute();
          }
        } else {
          $is_deleted = FALSE;
        }
        if (isset($is_deleted['file']['node'])) {
          $entity_id = $entityids[$del_cnt];
          $replacements = array('!file_entity_id' => $entity_id);
          drupal_set_message(t('File fid !fid was not deleted because it is in use by a node.', array('!fid' => $entity_id)),'error',TRUE);
          $del_cnt++; // Try next dupe.
        } else if (isset($is_deleted['file']['node'])) {
          $num_deleted = db_delete('media_unique')
            ->condition('entity_id', $entity_id)
            ->condition('sha1', $sha1)
            ->execute();
          if ($num_deleted == 1) {
            $del_cnt++; // Try next dupe.
            $report_deletes++;
            $DEBUG = FALSE;
            if ($DEBUG) {
              $replacements = array(
               '!fid' => $entity_id,
               '!sha1' => $sha1
              );
              watchdog('media_unique', 'Successfully deleted !fid with sha1 hash !sha1', $replacements, WATCHDOG_NOTICE);
            }
          }
        } else {
          $del_cnt++; // Try next dupe.
          $replacements = array(
           '!fid' => $entity_id,
           '!sha1' => $sha1
          );
          watchdog('media_unique', 'Unable to delete !fid with sha1 hash !sha1', $replacements, WATCHDOG_ERROR);
        }
      } else {
        $del_cnt = $cnt;
      }
    }
  }

  if ($report_deletes > 0) {
    $report_deletes = t('Deleted !deleted files.', array('!deleted' => $report_deletes));
  } else {
    $report_deletes = t('No deletions, !deleted files', array('!deleted' => $report_deletes));
  }
}
