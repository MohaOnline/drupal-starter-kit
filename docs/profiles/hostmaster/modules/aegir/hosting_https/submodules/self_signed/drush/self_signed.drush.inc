<?php

/**
 * @file
 *   A SelfSigned implementation of the Certificate service for the Provision API.
 */

/**
 * Implements hook_drush_init().
 */
function self_signed_drush_init() {
  self_signed_provision_register_autoload();
}

/**
 * Register our directory as a place to find provision classes.
 */
function self_signed_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_provision_services().
 *
 * Ensure our classes are loaded early enough to instantiate the Provision
 * services.
 */
function self_signed_provision_services() {
  self_signed_provision_register_autoload();
}

