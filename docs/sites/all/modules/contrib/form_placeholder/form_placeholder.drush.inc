<?php

/**
 * @file
 *   Drush integration for Form Placeholder.
 */

/**
 * Implements hook_drush_command().
 */
function form_placeholder_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['download-placeholder-library'] = array(
    'callback' => 'drush_download_placeholder_library',
    'description' => dt('Download and install the jquery.placeholder plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the jQuery Placeholder plugin. If omitted Drush will use the default location.'),
    ),
  );

  return $items;
}

/**
 * Command to download the jQuery Placeholder plugin.
 */
function drush_download_placeholder_library() {
  if (!module_exists('libraries')) {
    return drush_log(dt("Libraries module doesn't exists."), 'error');
  }

  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  $placeholder_library = libraries_info('jquery.placeholder');
  if ($filepath = drush_download_file($placeholder_library['download url'])) {
    $filename = basename($filepath);
    $dirname =  'jquery-placeholder-' . $placeholder_library['version'];

    // Remove any existing jQuery Placeholder plugin directory
    if (is_dir($dirname) || is_dir('jquery.placeholder')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('jquery.placeholder', TRUE);
      drush_log(dt('A existing jQuery Placeholder plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive
    drush_tarball_extract($filename);

    // Change the directory name to "jquery.placeholder" if needed.
    if ($dirname != 'jquery.placeholder') {
      drush_move_dir($dirname, 'jquery.placeholder', TRUE);
      $dirname = 'jquery.placeholder';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('jQuery Placeholder plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the jQuery Placeholder plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
