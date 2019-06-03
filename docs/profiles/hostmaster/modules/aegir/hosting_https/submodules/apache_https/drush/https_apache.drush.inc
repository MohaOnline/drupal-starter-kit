<?php

/**
 * @file
 *   Register with Provision autoloader.
 */

/**
 * Implements hook_drush_init().
 */
function https_apache_drush_init() {
  https_apache_provision_register_autoload();
}

/**
 * Register our directory as a place to find provision classes.
 */
function https_apache_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_provision_apache_vhost_config().
 *
 * Adds HTTPS client authentication to sites that require it.
 */
function https_apache_provision_apache_vhost_config($uri, $data) {
  $http_service = $data['server']->service('http');

  if ((d()->type != 'site') ||
      !isset($http_service->https_enabled) || !$http_service->https_enabled ||
      !isset(d()->https_enabled) || !d()->https_enabled ||
      !isset(d()->https_client_authentication_enabled) || !d()->https_client_authentication_enabled) {
    return '';
  }

  $lines = array();
  $lines[] = "  ###################################";
  $lines[] = "  ### Aegir HTTPS (hosting_https) ###";
  $lines[] = "  ###################################";
  $lines[] = "  # Enable HTTPS client authentication as per site settings.";

  if (!empty(d()->https_client_authentication_path)) {
    $lines[] = "  <Location '" . strip_tags(d()->https_client_authentication_path) . "'>";
  }

  $lines[] = "  SSLVerifyClient optional_no_ca";
  $lines[] = "  SSLVerifyDepth 1";
  $lines[] = "  SSLOptions +StdEnvVars +ExportCertData";

  if (!empty(d()->https_client_authentication_path)) {
    $lines[] = "  </Location>";
  }

  $lines[] = "\n";
  return implode("\n", $lines);
}
