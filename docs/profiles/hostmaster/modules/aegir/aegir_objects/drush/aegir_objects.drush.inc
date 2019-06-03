<?php

/**
 * @file
 * Provision/Drush hooks for Aegir API commands.
 */

/**
 * Register our directory as a place to find provision classes.
 */
function aegir_objects_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_drush_init().
 */
function aegir_objects_drush_init() {
  aegir_objects_register_autoload();
}

