<?php

/**
 * @file
 *   A Certificate service type for the Provision API.
 */

/**
 * Implements hook_drush_init().
 */
function certificate_drush_init() {
  certificate_provision_register_autoload();
}

/**
 * Register our directory as a place to find provision classes.
 */
function certificate_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_provision_services().
 *
 * Expose the service type this extension defines to provision.
 *
 * @return
 *   An array with the service type as key, and the default implementation as value.
 */
function certificate_provision_services() {
  certificate_provision_register_autoload();
  return array('Certificate' => NULL);
}

/**
 * Implements drush_HOOK_COMMAND().
 */
function drush_certificate_provision_verify() {
  if (array_key_exists('Certificate', d()->get_services())) {
    d()->service('Certificate')->verify();
  }
}

