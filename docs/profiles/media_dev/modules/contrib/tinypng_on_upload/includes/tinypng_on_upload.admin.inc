<?php
/**
 * @file
 * Admin interface callbacks/handlers to configure TinyPNG On Upload.
 */

/**
 * TinyPNG Optimize form callback.
 */
function tinypng_on_upload_settings_form($form, &$form_state) {
  $form = array();

  $settings = variable_get('tinypng_on_upload_tinypng', array(
    'api_key' => '',
    'debug_mode_tinypng' => FALSE,
  ));

  $form['tinypng_on_upload_tinypng'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $form['tinypng_on_upload_tinypng']['intro'] = array(
    '#markup' => t('<p>TinyPNG uses smart lossy compression techniques to reduce the file size of your image files. By selectively decreasing the number of colors in the image, fewer bytes are required to store the data. The effect is nearly invisible but it makes a HUGE difference in file size!</p><p>TinyPNG compression only applies to PNG and JPG files.</p><p>Free licenses have a limit of 500 conversions per user and per month.</p>'),
  );

  $form['tinypng_on_upload_tinypng']['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter your API key'),
    '#default_value' => $settings['api_key'],
    '#size' => 33,
    '#maxlength' => 32,
    '#required' => FALSE,
    '#element_validate' => array('tinypng_on_upload_tinypng_validate_api_key'),
  );

  $form['tinypng_on_upload_tinypng']['debug_mode_tinypng'] = array(
    '#type' => 'checkbox',
    '#title' => t('<b>Enable debug mode</b>: show statistics (size reduction) from successful requests.'),
    '#default_value' => $settings['debug_mode_tinypng'],
  );

  $form['tinypng_on_upload_process_existing_images'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Optimize previous <b>PNG</b> and <b>JPG</b> images.'),
    '#description' => t('Process previously uploaded images through the tinyPNG service that were uploaded prior to activation of this module. <b>Note: If you have many images to process this could exhaust your monthly api limit if you do not have a paid tinyPNG subscription.</b><br /><br />'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
  );

  if ($settings['api_key']) {
    // Get list of image files and the number of un-optimized images.
    $checkbox_options = _tinypng_on_upload_get_image_field_checkboxes();

    $form['tinypng_on_upload_process_existing_images']['tinypng_on_upload_images_list'] = array(
      '#title' => t('Unprocessed images'),
      '#type' => 'checkboxes',
      '#description' => t('Select existing image fields that you would like to submit to tinyPNG to optimize. Note: images pending after emptying your queue may have been flagged as CMYK images which TinyPNG can not process.'),
      '#options' => $checkbox_options,
    );

    foreach ($checkbox_options as $option => $value) {

      // Check to see if images for this filed are already queued for processing
      // if so, disable the checkbox.
      $is_there_a_queue = _tinypng_on_upload_check_for_field_queue($option);

      if ($is_there_a_queue) {

        $form['tinypng_on_upload_process_existing_images']['tinypng_on_upload_images_list'][$option] = array(
          '#disabled' => TRUE,
        );
      }
    }

    $form['tinypng_on_upload_process_existing_images']['tinypng_on_upload_process_all_images_button'] = array(
      '#type'   => 'submit',
      '#value'  => t('Queue Images for cron processing'),
      '#submit' => array('_tinypng_on_upload_process_all_images_submit'),
    );

    $form['tinypng_on_upload_process_existing_images']['tinypng_on_upload_process_all_images_process_queue_button'] = array(
      '#type'   => 'submit',
      '#value'  => t('Manually batch process images'),
      '#submit' => array('_tinypng_on_upload_process_all_images_process_queue_button_submit'),
    );
  }

  return system_settings_form($form);
}


/**
 * Submit handler to queue images for update.
 */
function _tinypng_on_upload_process_all_images_submit($form, &$form_state) {

  // Get list of unprocessed images from the database.
  $stored_image_ids = _tinypng_on_upload_get_unprocessed_images();

  // Get the image field names from our submitted form.
  $image_fields_to_process = array_filter($form_state['values']['tinypng_on_upload_images_list']);

  if ($image_fields_to_process) {

    foreach ($image_fields_to_process as $field) {

      if ($stored_image_ids[$field]) {
        $message = "";
        $message = $field . t('_image_updates cron queue created.');
        drupal_set_message($message);

        foreach ($stored_image_ids[$field] as $image_field) {

          $queue = DrupalQueue::get($field . '_image_updates');

          $queue->createItem($image_field[$field . "_fid"]);

        }
      }
    }

  }

}

/**
 * Submit handler to queue images for update.
 */
function _tinypng_on_upload_process_all_images_process_queue_button_submit($form, &$form_state) {
  $image_fields_to_process = array();

  // Get list of unprocessed images from the database.
  $stored_image_ids = _tinypng_on_upload_get_unprocessed_images();

  // Get the image field names from our submitted form.
  $fields_from_submit = array_filter($form_state['values']['tinypng_on_upload_images_list']);

  if ($fields_from_submit) {

    $image_fields_to_process = $fields_from_submit;

  }
  else {

    $instances = _tinypng_on_upload_get_image_type_machine_names();

    // Loop through each machine name to check for a queue
    // if so, add it to the process queue.
    foreach ($instances as $instance) {

      if (_tinypng_on_upload_check_for_field_queue($instance['field_name'])) {

        $image_fields_to_process[] = $instance['field_name'];

      }
    }

  }

  if ($image_fields_to_process) {
    $batch = array(
      'operations' => array(),
      'finished' => 'tinypng_on_upload_process_all_images_process_queue',
      'title' => t('TinyPNG Batch Optimize'),
      'init_message' => t('TinyPNG Optimization is starting...'),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('TinyPNG Batch Optimize has encountered an error.'),
    );

    foreach ($image_fields_to_process as $field) {

      if ($stored_image_ids[$field]) {

        foreach ($stored_image_ids[$field] as $image_field) {

          $batch['operations'][] = array('_tinypng_on_upload_batch_file_process', array($image_field[$field . "_fid"]));
        }
      }
    }

    batch_set($batch);

    tinypng_on_upload_delete_pending_cron_queues();

    batch_process('admin/config/media/tinypng_on_upload');

  }

}

/**
 * Remove pending cron queues to reset the image count on the status page.
 */
function tinypng_on_upload_delete_pending_cron_queues() {

  $instances = _tinypng_on_upload_get_image_type_machine_names();

  // Loop through each machine name to check and make sure a queue has been
  // added and has items to process
  // if so, delete it.
  foreach ($instances as $instance) {

    $queue = DrupalQueue::get($instance['field_name'] . '_image_updates');
    $num_items = $queue->numberOfItems();

    if ($num_items > 0) {

      $queue->deleteQueue();

      watchdog('tinypng_on_upload', 'Queue %fieldname Deleted for manual processing.', array('%fieldname' => $instance['field_name']));

    }

  }

}

/**
 * Validation callback for tinyPNG API key.
 *
 * Just check that the key entered has 32-char length.
 */
function tinypng_on_upload_tinypng_validate_api_key($element, &$form_state, $form) {

  if (strlen($element['#value']) != 32) {
    form_set_error($element['#parents'][0], t('TinyPNG API key must have 32-char length.'));
    return FALSE;
  }
  return TRUE;
}

/**
 * Make a nicely formatted name from our machine name for the checkbox label.
 *
 * @param string $machine_name
 *    Machine name string.
 *
 * @return string
 *    Words capitalized and underscores replaced with spaces.
 */
function _tinypng_on_upload_make_readable_field_name($machine_name) {
  $name_pieces = explode("_", $machine_name);
  $nice_name = "";

  foreach ($name_pieces as $index) {
    // code...
    $nice_name .= $index . " ";
  }

  return ucwords(trim($nice_name));
}

/**
 * Function to popluate the form checkboxes with all declared image fields.
 *
 * Give the count of any un-optimized images.
 *
 * @return array
 *    Array of options for checkbox form
 */
function _tinypng_on_upload_get_image_field_checkboxes() {
  $check_box_array = array();

  $instances = _tinypng_on_upload_get_image_type_machine_names();
  $images_to_be_processed = _tinypng_on_upload_get_unprocessed_images();

  foreach ($instances as $instance) {

    $field_readable_name = _tinypng_on_upload_make_readable_field_name($instance['field_name']);

    $field_ids_count = count($images_to_be_processed[$instance['field_name']]);

    // Check to see if there is an existing queue for this image field.
    if (_tinypng_on_upload_check_for_field_queue($instance['field_name'])) {

      $check_box_array[$instance['field_name']] = t("@readable_name <b> - Image updates for this queue are pending in cron.</b>", array('@readable_name' => $field_readable_name));

    }
    else {

      $check_box_array[$instance['field_name']] = t("@readable_name <b>@image_count images to be processed.</b>", array('@readable_name' => $field_readable_name, '@image_count' => $field_ids_count));

    }
  }

  return $check_box_array;

}

/**
 * Check the database file_usage table for un-optimized image file id's.
 *
 * @return Drupal Static Variable
 *    ID's to be processed.
 */
function _tinypng_on_upload_get_unprocessed_images() {

  $images_to_be_processed = &drupal_static(__FUNCTION__);

  if (!isset($images_to_be_processed)) {

    // Query the file usage table for files that have not been ran through our
    // optimize function.
    $optimized_images = db_select('file_usage', 'fu')
      ->fields('fu', array('fid'))
      ->condition('fu.module', "tinypng_on_upload", "=")
      ->execute()
      ->fetchAll(PDO::FETCH_ASSOC);

    // Get the machine names for all image fields.
    $instances = _tinypng_on_upload_get_image_type_machine_names();

    // Check each custom image field database table against the array of already
    // optimized images.
    foreach ($instances as $instance) {

      // Image field database table name.
      $file_table = "field_data_" . $instance['field_name'];

      // Look for image ID's NOT IN the optimized images array.
      if ($optimized_images) {
        $images_to_be_processed[$instance['field_name']] = db_select($file_table)
          ->fields($file_table, array($instance['field_name'] . "_fid"))
          ->condition($instance['field_name'] . "_fid", $optimized_images, 'NOT IN')
          ->execute()
          ->fetchAll(PDO::FETCH_ASSOC);
      }
      else {
        $images_to_be_processed[$instance['field_name']] = db_select($file_table)
          ->fields($file_table, array($instance['field_name'] . "_fid"))
          ->execute()
          ->fetchAll(PDO::FETCH_ASSOC);
      }
    }

  }
  return $images_to_be_processed;
}

/**
 * Get an array of all machine names for declared image type fields.
 *
 * Set as static variable and add to drupal cache.
 *
 * @return array
 *    Array of image field machine names
 */
function _tinypng_on_upload_get_image_type_machine_names() {
  $instances = &drupal_static(__FUNCTION__);
  if (!isset($instances)) {
    if ($cache = cache_get('tinypng_on_upload_image_types')) {
      $instances = $cache->data;
    }
    else {
      // Do your expensive calculations here, and populate $my_data
      // with the correct stuff..
      $instances = field_read_fields(array("type" => "image"));
      cache_set('tinypng_on_upload_image_types', $instances, 'cache');
    }
  }
  return $instances;
}

/**
 * Check to see if an image optimize queue exists for this image field.
 *
 * @param string $field_name
 *    Machine name of field.
 *
 * @return bool
 *   Returns true if a cron queue exists for this field.
 */
function _tinypng_on_upload_check_for_field_queue($field_name) {
  $is_there_a_queue = FALSE;

  $queue = DrupalQueue::get($field_name . '_image_updates');

  $num_items = $queue->numberOfItems();

  if ($num_items > 0) {

    $is_there_a_queue = TRUE;

  }

  return $is_there_a_queue;
}
