<?php

/**
 * @file
 *   An remote_import of the provision service API.
 *
 *  Declares a new service type and a basic implementation of it.
 *  It matches the same service definition in the hosting front end.
 */

/**
 * Implements hook_drush_init().
 */
function remote_import_drush_init() {
  remote_import_register_autoload();
}

/**
 * Register our directory as a place to find provision classes.
 */
function remote_import_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Expose the service type this extension defines to provision.
 *
 * Implementations are then conditionally loaded when the option
 * "--$service_service_type=$type" is passed to the provision-save
 * command of a server.
 *
 * Implementations are automatically loaded from :
 * provision/$service/$type/$type_service.inc.
 *
 * @return
 *   An array with the service type the key, and the default implementation the value.
 */
function remote_import_provision_services() {
  remote_import_register_autoload();
  return array('remote_import' => NULL);
}

/**
 * Implements hook_drush_command().
 */
function remote_import_drush_command() {
  $items = array();

  $items['provision-remote_import_list_sites'] = array(
    'description' => "List all sites on the remote server.",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
  );

  $items['provision-remote_import'] = array(
    'description' => "Imports a site from the remote server.",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
    'options' => array(
      'old_url' => dt('Old URL.'),
      'new_url' => dt('New URL.'),
      'platform' => dt('Drush alias of the platform to deploy on.'),
      'db_server' => dt('Drush alias of teh database server to use.'),
    ),
  );

  return $items;
}

function drush_remote_import_provision_remote_import_list_sites() {
  $sites = d()->service('remote_import')->list_sites();
  if (is_array($sites)) {
    drush_set_option('remote_sites_list', $sites);
  }
  else {
    return drush_set_error('REMOTE_IMPORT_NO_SITES', 'No sites were found on the remote server.');
  }
}

function drush_remote_import_provision_remote_import() {
  $old_url = drush_get_option('old_url');

  // Go do a backup of that site.
  $backup_file = d()->service('remote_import')->fetch_site($old_url);

  // Now we want to deploy that backup with the given options.
  $new_alias = d()->service('remote_import')->deploy($backup_file, drush_get_option('old_url'), drush_get_option('new_url'), drush_get_option('platform'), drush_get_option('db_server'));

  // And finally, we should import this back into Hostmaster.
  provision_backend_invoke('@hostmaster', 'hosting-import', array($new_alias), array('remote_import_import' => TRUE, 'strict' => 0));
}
