<?php

/**
 * Form to choose a local platform for the imported remote site.
 */
function hosting_remote_import_sites_choose_platform_form($form, &$form_state, $server, $vid, $url) {
  $form = array();

  // Horrible, horrible way that we have to redirect after a batch.
  if (!empty($_SESSION['hosting_remote_import_redirect2'])) {
    $url = $_SESSION['hosting_remote_import_redirect2'];
    unset($_SESSION['hosting_remote_import_redirect2']);
    drupal_goto($url);
  }

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $server->nid,
  );

  $form['vid'] = array(
    '#type' => 'value',
    '#value' => $vid,
  );

  $form['site'] = array(
    '#type' => 'value',
    '#value' => $url,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('New URL'),
    '#description' => t('This will be used as the local site URL'),
    '#default_value' => $url,
    '#required' => TRUE,
    '#element_validate' => array(
      'hosting_remote_import_sites_choose_platform_form_title_validate',
    ),
  );

  $form['overwrite'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overwrite existing site?'),
    '#description' => t('Checking this option will delete the site with the same name (if it exists) before importing the new site.'),
    '#default_value' => FALSE,
  );

  $form['platform'] = array(
    '#type' => 'radios',
    '#title' => t('Platform'),
    '#description' => t('Choose the platform the site should be deployed on. This should be as close as possible to the platform on the remote server.'),
    '#options' => _hosting_get_platforms(),
    '#required' => TRUE,
  );

  $form['db_server'] = array(
    '#type' => 'radios',
    '#title' => t('Database server'),
    '#description' => t('The database server the site will use to host its content.'),
    '#options' => hosting_get_servers('db'),
    '#default_value' => HOSTING_DEFAULT_DB_SERVER,
    '#required' => TRUE,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('< Back'),
    '#submit' => array(
      'hosting_remote_import_sites_choose_platform_form_submit_back',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function hosting_remote_import_sites_choose_platform_form_title_validate($element, &$form_state) {
  $url = strtolower(trim($element['#value'])); // domain names are case-insensitive
  if (!_hosting_valid_fqdn($url)) {
    form_error($element, t("You have not specified a valid url for this site."));
  }

  # TODO: maybe we should allow creation of sites that conflict with HOSTING_SITE_DISABLED (which would then need to be renamed before being re-enabled)
  if (!hosting_domain_allowed($url) && !$form_state['values']['overwrite']) {
    form_error($element, t("The domain name you have specified is already in use. Check the box for overwriting if that's what you'd like to do."));
  }
}

/**
 * Form submit callback for the back button.
 *
 * @see hosting_remote_import_sites_choose_platform_form()
 */
function hosting_remote_import_sites_choose_platform_form_submit_back($form, &$form_state) {
  $values = $form_state['values'];
  // Just build a redirect
  $form_state['redirect'] = 'node/' . $values['nid'] . '/import-sites/chooser/' . $values['vid'] . '/' . drupal_get_token($values['vid']);
}

/**
 * Form submit callback.
 *
 * @see hosting_remote_import_sites_choose_platform_form()
 */
function hosting_remote_import_sites_choose_platform_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $args = array(
    'old_url' => $values['site'],
    'new_url' => $values['title'],
    'platform' => $values['platform'],
    'db_server' => $values['db_server'],
  );

  $batch = array(
    'operations' => array(
      array('hosting_remote_import_sites_choose_platform_form_submit_batch_create_task', array($values['nid'], $args)),
    ),
    'finished' => 'hosting_remote_import_sites_choose_platform_form_submit_batch_finished',
    'file' => drupal_get_path('module', 'hosting_remote_import') . '/hosting_remote_import.local_settings_form.inc',
    'title' => t('Importing'),
  );

  batch_set($batch);
}

/**
 * Batch callback.
 *
 * The first time we are called, create a new backend task to do all the work,
 * and then poll to see if it has completed.
 */
function hosting_remote_import_sites_choose_platform_form_submit_batch_create_task($nid, $args, &$context) {

  if (!isset($context['results']['nid'])) {
    $context['results']['nid'] = $nid;
  }

  if (!isset($context['results']['new_url'])) {
    $context['results']['new_url'] = $args['new_url'];
  }

  // On the first time through, create the task.
  if (!isset($context['results']['task_vid'])) {
    $task = hosting_add_task($nid, 'remote_import', $args);
    $context['results']['task_vid'] = $task->vid;
    $context['finished'] = 0.1;
    $context['message'] = t('Created task to import the remote site.');
    return;
  }

  // Now we wait for it to be created.
  $tasks = db_query("SELECT t.vid, n.nid FROM {hosting_task} t INNER JOIN {node} n ON n.vid = t.vid WHERE task_type = :task_type AND t.rid = :trid AND t.vid >= :tvid ORDER BY t.vid ASC", array(':task_type' => 'remote_import', ':trid' => $nid, ':tvid' => $context['results']['task_vid']));
  foreach ($tasks as $row) {
    $task = node_load($row->nid, $row->vid);

    switch ($task->task_status) {
      case HOSTING_TASK_PROCESSING:
        $context['finished'] = 0.5;
        $context['message'] = 'Importing. This may take a while.';
        return;
      case HOSTING_TASK_QUEUED:
        break;
      case HOSTING_TASK_SUCCESS:
        $context['message'] = 'Complete';
        $context['results']['final_vid'] = $task->vid;
        return;
      case HOSTING_TASK_ERROR:
        return;
        // We shouldn't get any other status.
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
 * Redirect to either the node that was just imported, or an error page.
 */
function hosting_remote_import_sites_choose_platform_form_submit_batch_finished($success, $results, $operations) {
  if (isset($results['new_url']) && ($node = hosting_context_load($results['new_url']))) {
    drupal_set_message(t('Site imported successfully.'));
    $_SESSION['hosting_remote_import_redirect2'] = 'node/' . $node->nid;
  }
  else {
    drupal_set_message(t('An error occurred on import, please check the task log and try again.'), 'error');
    $_SESSION['hosting_remote_import_redirect2'] = 'node/' . $results['nid'];
  }
}
