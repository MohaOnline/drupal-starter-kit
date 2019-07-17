<?php
// $Id:

/**
 * Implements hook_install_tasks().
 */
function media_dev_install_tasks(&$install_state) {
  $tasks = array();

  // Add the media_dev theme selection to the installation process.
  //require_once drupal_get_path('module', 'media_dev_theme') . '/media_dev_theme.profile.inc';
  //$tasks = $tasks + media_dev_theme_profile_theme_selection_install_task($install_state);

  // Set up a task to include secondary language (fr).
  $tasks['media_dev_batch_processing'] = array(
    'display_name' => st('Import French Language'),
    'type' => 'batch',
  );
}

/**
 * Implements hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function media_dev_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'install_configure') {
    // Set default for site name field.
    $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
  }
}

/**
 * Batch Processing for French Language import.
 */
function media_dev_batch_processing(&$install_state) {
  // Import the additonal language po file and translate the interface.
  // Require once is only added here because too early in the bootstrap.
  require_once 'includes/locale.inc';
  require_once 'includes/form.inc';

  // Batch up the process + import existing po files.
  $batch = locale_batch_by_language('fr');
  return $batch;

}
