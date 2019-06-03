<?php

/**
 * @file
 * DNS provisioning module.
 *
 * The goal of this module is to manage DNS zonefiles and Resource Records
 * (RRs), for sites that are about to be created.  It uses the provision API to
 * tie into the right places in the site creation work flow.
 */

/**
 * Implements hook_drush_init().
 */
function hosting_dns_drush_init() {
  hosting_dns_provision_register_autoload();
}

/**
 *  Implements hook_provision_services().
 */
function hosting_dns_provision_services() {
  hosting_dns_provision_register_autoload();
  return array('dns' => NULL);
}

/**
 * Register our directory as a place to find provision classes.
 */
function hosting_dns_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_drush_command().
 */
function hosting_dns_drush_help($section) {
  switch ($section) {
    case 'error:HOSTING_DNS_BACKUP_FAILED' :
      return dt('Failed to backup DNS settings.');
    case 'error:HOSTING_DNS_UNKNOWN_ERROR' :
      return dt('An unknown error occurred.');  }
}

/**
 * Implements hook_hosting_TASK_OBJECT_context_options().
 *
 * Makes sure the DNS records are saved persistent in the backend.
 */
function hosting_dns_hosting_site_context_options(&$task) {
  $task->context_options['dns_records'] = $task->ref->dns_records;
}

/**
 * Implements hook_drush_context_import().
 *
 * If we're importing a site, then import the DNS records.
 */
function hosting_dns_drush_context_import($context, &$node) {
  if ($context->type == 'site') {
    $node->dns_records = $context->dns_records;
  }
}

/**
 * Implementation of hook_drush_command().
 */
function hosting_dns_drush_command() {
  $items['provision-dns-deploy'] = array(
    'description' => dt('Deploy a zonefile, based on all DNS nodes associated with a given zone.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    'examples' => array(
      'drush @site provision-dns-deploy' => dt('Deploys a zonefile for @site.'),
    ),
  );

  return $items;
}

/**
 * Implements drush_hook_COMMAND_validate().
 *
 * Make sure the site is installed and enabled.
 */
function drush_hosting_dns_provision_dns_deploy_validate() {
  if (!@drush_bootstrap(DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION)) {
    if (drush_get_option('force', FALSE)) {
      drush_log("clearing error");
      drush_set_context('DRUSH_ERROR_CODE', DRUSH_SUCCESS);
    }
  }
  if (!drush_get_option('installed') && !drush_get_option('force', FALSE)) {
    drush_set_error('PROVISION_DRUPAL_SITE_NOT_FOUND');
  }
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * Backup zone files.
 */
function drush_hosting_dns_pre_provision_dns_deploy() {
  $dns_records = d()->dns_records;

  // Also process aliases.
  foreach ($dns_records as $zone => $records) {
    if (!d()->service('dns')->backup($zone)) {
      // If backup failed, leave now and return FALSE.
      return drush_set_error('HOSTING_DNS_BACKUP_FAILED');
    }
  }
}

/**
 * Implements drush_hook_COMMAND().
 *
 * Function for generating a zonefile based on DNS nodes.
 *
 * @return bool
 */
function drush_hosting_dns_provision_dns_deploy() {
  $dns_records = d()->dns_records;

  // Also process aliases.
  foreach ($dns_records as $zone => $records) {

    // Todo: This should become a property of the context, not the sever.
    d()->server->setProperty('deploy_zone', $zone);

    d()->service('dns')->create_host($zone);
    foreach ($records as $name => $record) {

      // Transform the hostmaster records into provision records.
      foreach ($record as $real_record) {

        list($type, $ttl, $destination) = array_values($real_record);

        if (empty($record[$type]) || !is_array($record[$type])) {
          $record[$type] = array();
        }

        $record[$type] = array_merge($record[$type], array($destination => $ttl));

        $status = d()->service('dns')->config('zone', $zone)->record_set($zone, $record);

        // TODO: Display statuses?
      }
    }

    d()->service('dns')->config('zone', $zone)->write();
    drush_log(dt('Done writing DNS entries for %zone.', array('%zone' => $zone)), 'success');
  }

  // Check configuration.
  d()->service('dns')->check_config();
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * Final deployment.
 */
function drush_hosting_dns_post_provision_dns_deploy() {
  d()->service('dns')->parse_configs();
}

/**
 * Implements drush_hook_COMMAND_rollback().
 *
 * Allows restoring previous configurations in the backend.
 */
function drush_hosting_dns_provision_dns_deploy_rollback() {
  $dns_records = d()->dns_records;

  foreach ($dns_records as $zone => $records) {
    d()->service('dns')->restore_backup($zone);
  }
  d()->service('dns')->parse_configs();

  // TODO: From here the database is no longer in sync with the config. Is that desirable or do we have to revert DB settings as well?
  // In that case, I think we require hosting_dns_hosting_dns_deploy_task_rollback() to revert the site node.
}

/**
 * Implements hook_hosting_TASK_TYPE_task_rollback().
 *
 * Restore the site node's DNS values.
 */
function hosting_dns_hosting_dns_deploy_task_rollback($task, $data) {
  // TODO: No idea if this is required, but keep it here so I don't forget the hook.

  // $task->ref->dns_records = $old_dns_records;  // No idea how to obtain this.
  // node_save($task->ref);
}
