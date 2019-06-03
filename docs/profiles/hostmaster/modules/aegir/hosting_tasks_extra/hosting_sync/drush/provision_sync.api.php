<?php

/**
 * @file
 * Contains documentation about the Provision Sync module's hooks.
 */

/**
 * @defgroup provision_sync Provision Sync
 * @{
 * Hooks from the Provision Sync module.
 */

/**
 * Called after backing up the destination but before performing the sync.
 *
 * Allows another module to prepare the source or destination to be sync'd
 * or abort the sync for any reason.
 *
 * @param string $source
 *   The drush alias of the source site (including the leading '@').
 * @param string $destination
 *   The drush alias of the destination site (including the leading '@').
 *
 * @return boolean
 *   Return FALSE if you wish to abort the sync.
 *
 * @see drush_provision_sync()
 */
function hook_provision_sync_before($source, $destination) {
  // Prepare the source or destination to be sync'd!

  if ($something_is_wrong) {
    return FALSE;
  }
}

/**
 * Called after the sync process is complete.
 *
 * @param string $source
 *   The drush alias of the source site (including the leading '@').
 * @param string $destination
 *   The drush alias of the destination site (including the leading '@').
 *
 * @see drush_provision_sync()
 */
function hook_provision_sync_after($source, $destination) {
  // Perform some clean up or other operation after the sync is complete.
}

/**
 * @}
 */
