<?php

/**
 * @file
 * Register with Provision autoloader.
 */

/**
 * Implements hook_drush_init().
 */
function https_nginx_drush_init() {
  https_nginx_provision_register_autoload();
}

/**
 * Register our directory as a place to find provision classes.
 */
function https_nginx_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_provision_nginx_vhost_config().
 *
 * Adds HTTPS client authentication to sites that require it.
 */
function https_nginx_provision_nginx_vhost_config($uri, $data) {
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
  $lines[] = "  ssl_verify_client optional_no_ca;";
  $lines[] = '  fastcgi_param CLIENT_CERTIFICATE $ssl_client_raw_cert;';
  $lines[] = "\n";
  return implode("\n", $lines);
}
