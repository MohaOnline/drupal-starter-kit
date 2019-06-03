<?php

/**
 * @file
 * Drush commands for Provision Sync
 */

/**
 * Implementation of hook_drush_command().
 * Provides provision commands.
 */
function provision_sync_drush_command() {
  $items = array();
  $items['provision-sync'] = array(
    'description' => 'Sync database and files from a chosen source site.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'drush dependencies' => array('sql', 'core'),
    'options' => array(
      'backup' => 'Create a backup of the destination before sync.',
      'updatedb' => 'Run update.php after content sync.',
      'registry-rebuild' => 'Run registry-rebuild after content sync.',
      'features-revert-all' => 'Revert all features after content sync.',
      'cache-clear' => 'Clear all caches after content sync.',
      'update-uri' => 'Update URIs inside the database content.',
      'database' => 'Sync site database.',
      'files' => 'Sync site files.',
      'sync-platform' => 'Sync site platform code. DANGEROUS when target platform has multiple sites',
      'modules' => 'Sync site modules.',
      'themes' => 'Sync site themes.',
      'libraries' => 'Sync site libraries.',
      'disable-rollback-backup' => 'Live dangerously... trade the rollback option for a time saving.',
    ),
    'arguments' => array(
      'from' => 'Name of subdirectory within /sites or a site-alias.',
      'to' => 'Name of subdirectory within /sites or a site-alias.',
    ),
    'examples' => array(
      'drush provision-sync @prod @dev' => 'Copies the database and files from @prod to @dev',
    ),
  );
  return $items;
}

/**
 * Includes the sync.provision.inc file.
 */
function provision_sync_command_include() {
  include_once(dirname(__FILE__) . '/sync.provision.inc');
}
