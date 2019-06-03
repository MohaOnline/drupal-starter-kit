<?php

/**
 * @file
 *   Initialize a sandboxed environment. Starts with call hosting_civicrm_tests_init() at bottom.
 */
// This code copied from drush_bootstrap_prepare() except for 1st line.
$local_vendor_path = dirname(__DIR__) . '/vendor/autoload.php';

// Check for a local composer install or a global composer install. Vendor dirs are in different spots.
if ((!@include $local_vendor_path)) {
  $msg = "Unable to load autoload.php. Drush now requires Composer in order to install its dependencies and autoload classes. Please see http://docs.drush.org/en/master/install/\n";
  fwrite(STDERR, $msg);
  return FALSE;
}

ini_set('error_reporting', E_ALL);
ini_set('display_errors', '1');
ini_set('display_startup_errors', '1');

hosting_civicrm_tests_init();

/**
 * Initialize our environment at the start of each run (i.e. suite).
 */
function hosting_civicrm_tests_init() {
}
