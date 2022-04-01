<?php

/**
 * @file
 * Drush integration.
 */
/**
 * The Masked Input library URI.
 */
define (MASKEDINPUT_DOWNLOAD_URI, 'http://cloud.github.com/downloads/digitalBush/jquery.maskedinput/jquery.maskedinput-1.3.js');

/**
 * Implements hook_drush_command().
 */
function masked_input_drush_command() {
  $items['masked_input-library'] = array(
    'callback' => 'masked_input_drush_library',
    'description' => dt('Downloads maskedinput.js library.'),
    'aliases' => array('mil'),
    'examples' => array(
      'drush masked_input library' => dt('Download maskedinput.js to the libraries directory.'),
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function masked_input_drush_help($section) {
  switch ($section) {
    case 'drush:masked_input-library':
      return dt("Download Masked Input's 3rd party dependencies into the libraries directory.");
  }
}

/**
 * Drush command callback for "masked_input library".
 */
function masked_input_drush_library() {
  // Setting download location.
  $args = func_get_args();
  if ($args[0]) {
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

  $filename = basename(MASKEDINPUT_DOWNLOAD_URI);
  $dirname = 'maskedinput';

  if (is_dir($dirname)) {
    chdir($dirname);
    // Remove any existing Masked Input library
    if (is_file($filename)) {
      drush_log(dt('An existing Masked Input library was overwritten at @path', array('@path' => $path)), 'notice');
      drush_op('unlink', $filename);
    }
  }
  else {
    drush_op('mkdir', $dirname);
    chdir($dirname);
  }

  // Download the Masked Input library
  if (!drush_shell_exec('wget '. MASKEDINPUT_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O '. MASKEDINPUT_DOWNLOAD_URI);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path .'/'. $dirname)) {
    drush_log(dt('Masked Input library has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download Masked Input library to @path', array('@path' => $path)), 'error');
  }
}
