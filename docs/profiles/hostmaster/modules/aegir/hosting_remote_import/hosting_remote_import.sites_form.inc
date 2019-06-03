<?php

/**
 * Form to start the process of importing some sites.
 */
function hosting_remote_import_sites_form($form, &$form_state, $node) {
  $form = array();

  // Horrible, horrible way that we have to redirect after a batch.
  if (!empty($_SESSION['hosting_remote_import_redirect'])) {
    $url = $_SESSION['hosting_remote_import_redirect'];
    unset($_SESSION['hosting_remote_import_redirect']);
    drupal_goto($url);
  }

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Importing a site'),
    '#description' => t('<p>The following pages will take you through the following steps.<ol><li><p>We will fetch a list of the sites on the remote server.</p></li><li><p>You can then choose which of the remote sites that you want to import.</p></li><li><p>You will be given options for which platform you want to deploy this site into. Note that you must have a compatible platform setup before you start this process. So you may need to <a href="@add_platform">add a new platform</a>.</p></li><li><p>The site will then be imported.</p></li></ol><p>Click the button below to get started.</p>', array('@add_platform' => url('node/add/platform'))),
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Retrieve a list of remote sites >'),
  );

  return $form;
}

/**
 * Form submit callback.
 *
 * @see hosting_remote_import_sites_form()
 */
function hosting_remote_import_sites_form_submit($form, &$form_state) {
  // Set up a batch that will wait for the hosting task to complete.

  $batch = array(
    'operations' => array(
      array('hosting_remote_import_sites_form_submit_batch_wait_task', array($form_state['values']['nid'])),
    ),
    'finished' => 'hosting_remote_import_sites_form_submit_batch_finished',
    'file' => drupal_get_path('module', 'hosting_remote_import') . '/hosting_remote_import.sites_form.inc',
    'title' => t('Fetching sites'),
  );

  batch_set($batch);
}

/**
 * Batch callback.
 *
 * The first time we are called we add a new task to the task queue.
 *
 * On subsequent calls will poll to see if that task has completed yet.
 */
function hosting_remote_import_sites_form_submit_batch_wait_task($nid, &$context) {

  if (!isset($context['results']['vid'])) {
    $server = node_load($nid);

    // Create a new list remote sites task.
    $task = hosting_add_task($nid, 'remote_import_list_sites');

    // Save this task into the context, so we can keep track of it.
    $context['results']['vid'] = $task->vid;
    $context['results']['nid'] = $nid;
    $context['finished'] = 0.1;
    return;
  }


  $tasks = db_query("SELECT t.vid, n.nid
                       FROM {hosting_task} t
                 INNER JOIN {node} n
                         ON n.vid = t.vid
                      WHERE task_type = :task_type
                        AND t.rid = :trid
                        AND t.vid >= :tvid
                   ORDER BY t.vid ASC", array(
    ':task_type' => 'remote_import_list_sites',
    ':trid' => $nid,
    ':tvid' => $context['results']['vid'])
  );
  foreach ($tasks as $row) {
    $task = node_load($row->nid, $row->vid);

    switch ($task->task_status) {
      case HOSTING_TASK_PROCESSING:
        $context['finished'] = 0.8;
        $context['message'] = 'Fetching results from remote server';
        return;
      case HOSTING_TASK_QUEUED:
        break;
      case HOSTING_TASK_WARNING:
      case HOSTING_TASK_SUCCESS:
        $context['message'] = 'Complete';
        $context['results']['final_vid'] = $task->vid;
        return;
      case HOSTING_TASK_ERROR:
        return;
        // We should never get this.
      default:
        return;
    }

  }

  $context['finished'] = 0.2;
  $context['message'] = 'Task is queued, waiting for dispatcher to run.';
}

/**
 * Batch finished callback.
 *
 * If the task executed successfully then set up a redirect in the session for
 * the host form to redirect to.
 * This, is the nasty way we have to do things in D6.
 */
function hosting_remote_import_sites_form_submit_batch_finished($success, $results, $operations) {
  if (isset($results['final_vid'])) {
    $vid = $results['final_vid'];
    $nid = $results['nid'];
    // This is horrible, but this is the only way that Batch API lets me do
    // this.
    $_SESSION['hosting_remote_import_redirect'] = 'node/' . $nid . '/import-sites/chooser/' . $vid . '/' . drupal_get_token($vid);
  }
  else {
    drupal_set_message(t('An error occurred, review the task log and try-again.'), 'error');
  }

}
