<?php

/**
 * @file
 *   A Certificate service type for the Provision API.
 */

/**
 * Implements hook_drush_init().
 */
function https_drush_init() {
  https_provision_register_autoload();
}

/**
 * Register our directory as a place to find provision classes.
 */
function https_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

