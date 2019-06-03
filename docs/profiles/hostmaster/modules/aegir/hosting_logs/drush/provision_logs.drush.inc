<?php
/**
 * @file
 * Separates sites error logs into individual files, and makes an Apache Alias
 * from /error.log to the error log path.
 *
 * Once downloaded into a drush command folder, simply re-verify any site.
 * Once verified, check http://yoursite/error.log to see the raw apache error
 * logs.
 */

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * Creates the /var/log/aegir folder if it does not exist
 * and we have write access.
 */
function drush_provision_logs_pre_provision_verify() {
  $log_path = drush_get_option('provision_logs_path', '/var/log/aegir');
  if (!file_exists($log_path) && is_writable(dirname($log_path))) {
    mkdir($log_path, 0775);
  }
}

/**
 * Implements hook_provision_apache_vhost_config().
 *
 * Sets each site's error log to be at /var/aegir/logs/$uri.error.log
 *
 * Optionally makes http://$uri/errors available through the provisioned site.
 * WARNING: This currently has no form of authentication.
 */
function provision_logs_provision_apache_vhost_config($uri, $data) {
  $log_file_path = drush_get_option('provision_logs_file_path', '/var/log/aegir');
  $log_url_path = drush_get_option('provision_logs_url_path', 'error.log');

  $logs_enabled = d()->logs_enabled;
  $logs_available = d()->logs_available;

  $error_log_path = "$log_file_path/$uri.error.log";
  $access_log_path = "$log_file_path/$uri.access.log";
  $vhost = array();

  $vhost[] = "";
  $vhost[] = "# PROVISION_LOGS";
  $vhost[] = "# ==============";
  if (!$logs_enabled) {
    $vhost[] = '# Site-specific logs have not been enabled.';
    return $vhost;
  }
  $vhost[] = "# Set error log to site-specific file.";
  $vhost[] = "ErrorLog $error_log_path";
  $vhost[] = "CustomLog $access_log_path combined";

  if ($logs_available) {
    $vhost[] = "";
    $vhost[] = "# Allow error log to be accessible from http://$uri/$log_url_path";
    $vhost[] = "Alias /$log_url_path $error_log_path";
  }
  $vhost[] = "# ==============";
  $vhost[] = "";
  return $vhost;
}

/**
 * Implements hook_provision_drupal_config().
 */
function provision_logs_provision_drupal_config($uri, $data) {
  if (drush_get_option('logs_visible', FALSE)) {
    return <<<PHP


  /**
   * PROVISION_LOGS
   * This site has "logs_visible" enabled, so errors are being output to the screen.
   * To change this, edit the site node or the drush alias.
   */

  ini_set('display_errors', 1);
  ini_set('display_startup_errors', 1);
  error_reporting(E_ALL);

  /**
   * End PROVISION_LOGS
   */

PHP;
  }
}


/**
 * Register our directory as a place to find provision classes.
 */
function provision_logs_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_drush_init().
 */
function provision_logs_drush_init() {
  provision_logs_register_autoload();
}

/**
 * Implements hook_provision_services().
 */
function provision_logs_provision_services() {
  provision_logs_register_autoload();
  return array('logs' => NULL);
}
