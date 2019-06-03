<?php


/**
 * @file
 * The hosting feature definition for the backup queue.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return
 *   associative array indexed by feature key.
 */
function hosting_backup_queue_hosting_feature() {
  $features['backup_queue'] = array(
    // title to display in form
    'title' => t('Backup scheduling/queue'),
    // description
    'description' => t('Allows backups to be scheduled per site.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_backup_queue',
    // associate with a specific node type.
    //  'node' => 'nodetype',
    // which group to display in ( null , experimental )
    'group' => 'experimental',
  );
  return $features;
}
