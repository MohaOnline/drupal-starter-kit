<?php

/**
 * Helper function to validate site aliases.
 */
function _drush_provision_sync_validate_alias($alias) {
  // We can't use drush_sitealias_load_record() because it will overwrite values loaded from the
  // current site's drushrc.php with values from the $alias drushrc.php, and provision will 
  // overwrite the current site's drushrc.php with the new values. Effectively, this causes the
  // database credentials on the destination site to get overwritten with the credentials from
  // the source site. That is no good!
  //
  // To make a long story short, invoking a new drush process is the only safe way to read alias
  // records...

  if (DRUSH_VERSION >= 6) {
    $result = drush_invoke_process('@none', 'site-alias', array($alias), array('format' => 'json'), FALSE);
    drush_log($result['output'], 'debug');
    $result_arr = json_decode($result['output'], $assoc = TRUE );

    if ($result['error_status'] === DRUSH_SUCCESS && !empty($result_arr[trim($alias, '@')]['uri'])) {
      return TRUE;
    }

  }
  else {
    if (DRUSH_VERSION >= 5) {
      $result = drush_invoke_process('@none', 'site-alias', array($alias));
    }
    else {
      $result = drush_backend_invoke('site-alias', array(0 => $alias, 'component' => 'context_type'), 'GET', FALSE);
    }
    if ($result['error_status'] === DRUSH_SUCCESS && trim($result['output']) === 'site') {
      return TRUE;
    }
  }

  return drush_set_error('DRUSH_ALIAS_NOT_FOUND', dt('Error: no alias record could be found for @alias', array('@alias' => $alias)));
}

/**
 * Implements hook_TASK_NAME_validate().
 */
function drush_provision_sync_validate($source = NULL, $destination = NULL) {
  // Fail if no source
  if (empty($source) && !($source = drush_get_option('source'))) {
    return drush_set_error('DRUSH_TARGET_NOT_FOUND', dt('Source not found. You must enter a source alias to sync from'));
  }

  // Fail if no destination
  if (empty($destination) && !($destination = drush_get_option('destination'))) {
    // Default to the current site if no destination is given
    $destination = drush_get_option('#name');
    if (empty($destination) || !drush_get_option('db_name')) {
      return drush_set_error('DRUSH_TARGET_NOT_FOUND', dt('Destination not found. You must enter a destination alias to sync to.'));
    }
  }

  $source = '@' . ltrim($source, '@');
  $destination = '@' . ltrim($destination, '@');

  // Validate that these aliases exist and represent sites (as opposed to platforms, etc)
  foreach (array($source, $destination) as $alias) {
    if (!_drush_provision_sync_validate_alias($alias)) {
      return FALSE;
    }
  }

  drush_set_option('source', $source);
  drush_set_option('destination', $destination);
}

/**
 * Implements the provision-sync command.
 *
 * Expects site aliases as the arguments.
 */
function drush_provision_sync($source = NULL, $destination = NULL) {
  $source = drush_get_option('source', $source);
  $destination = drush_get_option('destination', $destination);

  $source_site = d($source)->uri;
  $destination_site = d($destination)->uri;

  if (drush_get_option('backup', FALSE) && !drush_get_option('disable-rollback-backup')) {
    // Make a backup of the destination site before making any changes.
    $result = provision_backend_invoke($destination, 'provision-backup');
    if ($result['error_status'] !== DRUSH_SUCCESS) {
      return drush_set_error('PROVISION_BACKUP_FAILED', dt("Unable to proceed with sync because backup of destination failed."));
    }
    drush_set_option('backup_file', $result['context']['backup_file']);
    drush_set_option('backup_file_size', $result['context']['backup_file_size']);
  }

  // Invoke hook_provision_sync_before().
  foreach (drush_command_implements('provision_sync_before') as $command) {
    $func = $command . '_provision_sync_before';
    if ($func($source, $destination) === FALSE) {
      // Allow the before hook to abort the sync!
      return;
    }
  }

  // Fetch the source site, to have the latest files on the master server.
  provision_drupal_fetch_site($source);

  if (drush_get_option('database')) {
    // Drop the database
    drush_log(dt('Dropping tables for destination database (@destination)', array('@destination' => $destination)), 'ok');
    provision_backend_invoke($destination, 'sql-drop');
    
    // Sync the databases
    drush_log(dt('Syncing databases...'), 'ok');
    if (DRUSH_VERSION >= 5) {
      $result = drush_invoke_process('@none', 'sql-sync', array($source, $destination), array('yes' => TRUE, 'structure-tables-key' => 'common'), TRUE);
    }
    else {
      $result = drush_backend_invoke('sql-sync', array($source, $destination));
    }
    if ($result['error_status'] !== DRUSH_SUCCESS) {
      return drush_set_error('PROVISION_SQL_SYNC_FAILED', dt("Unable to sync from @source to @destination.", array('@source' => $source, '@destination' => $destination)));
    }
  }
  else {
    drush_log(dt('Skipped syncing databases...'), 'ok');
  }

  $files = array();
  if (drush_get_option('files')) {
    $files[] = 'files';
    $files[] = 'private/files';
  }
  foreach (array('modules', 'themes', 'libraries') as $files_type) {
    if (drush_get_option($files_type)) {
      $files[] = $files_type;
    }
  }

  // Sync files via rsync
  if (!empty($files)) {
    foreach ($files as $path) {
      drush_log(dt('Syncing files in the %path directory from @source to @destination', array('%path' => $path, '@source' => $source, '@destination' => $destination)), 'ok');

      $default_options = array(
          'yes' => TRUE,
          'delete' => TRUE,
          'include-vcs' => TRUE,
          );
      $global_extra_options = drush_get_option('global_sync_options', array());
      $options = array_merge($default_options, $global_extra_options);
      if (DRUSH_VERSION >= 5) {
        $result = drush_invoke_process('@none', 'rsync',
          array("$source:sites/$source_site/$path/", "$destination:sites/$destination_site/$path/"),
          $options,
          TRUE);
      }
      else {
        $result = drush_backend_invoke('rsync', array("$source:sites/$source_site/$path/", "$destination:sites/$destination_site/$path/"));
      }
      if ($result['error_status'] !== DRUSH_SUCCESS) {
        return drush_set_error('PROVISION_RSYNC_FAILED', dt("Unable to rsync files from @source to @destination.", array('@source' => $source, '@destination' => $destination)));
      }
    }
  }
  else {
    drush_log(dt('Skipped syncing files...'), 'ok');
  }

  // Sync platform code via rsync
  if (drush_get_option('sync-platform')) {
    drush_log(dt('Syncing platform code from @source to @destination', array('@source' => $source, '@destination' => $destination)), 'ok');

    $default_options = array(
      'yes' => TRUE,
      'delete' => TRUE,
      'include-vcs' => TRUE,
       'exclude-sites' => TRUE,
    );
    $global_extra_options = drush_get_option('global_sync_options', array());
    $options = array_merge($default_options, $global_extra_options);

    $result = drush_invoke_process('@none', 'rsync',
      array("$source:%root", "$destination:%root"),
      $options,
      TRUE);

    if ($result['error_status'] !== DRUSH_SUCCESS) {
      return drush_set_error('PROVISION_RSYNC_FAILED', dt("Unable to rsync platform code from @source to @destination.", array('@source' => $source, '@destination' => $destination)));
    }
  }
  else {
    drush_log(dt('Skipped syncing platform code...'), 'ok');
  }

  // registry-rebuild, unless option is false.
  if (drush_drupal_major_version(d($destination)->root) == 7 && drush_get_option('registry-rebuild')) {
    drush_log(dt('Rebuilding registry...'), 'ok');
    provision_backend_invoke($destination, 'registry-rebuild');
  }
  else {
    drush_log(dt('Skipped registry-rebuild...'), 'ok');
  }

  // update db, unless option is false.
  if (drush_get_option('updatedb')) {
    drush_log(dt('Updating database...'), 'ok');
    provision_backend_invoke($destination, 'updatedb');
  }
  else {
    drush_log(dt('Skipped updating database...'), 'ok');
  }

  // Revert All Features, unless option is false
  if (drush_get_option('features-revert-all')) {
    drush_log(dt('Reverting all features...'), 'ok');
    provision_backend_invoke($destination, 'features-revert-all');
  }
  else {
    drush_log(dt('Skipped reverting all features...'), 'ok');
  }

  // Update URI in database content, unless option is false.
  if (drush_get_option('update-uri')) {
    provision_backend_invoke($destination, 'provision-update_uri', array($source_site, $destination_site));
  }
  else {
    drush_log(dt('Skipped updating URI in database content...'), 'ok');
  }

  // Clear the whole cache, unless option is false
  // Seriously, lets do this twice.  Go Drupal!
  if (drush_get_option('cache-clear')) {
    drush_log(dt('Clearing all caches...'), 'ok');

    if (drush_drupal_major_version(d($destination)->root) <= 7) {
      provision_backend_invoke($destination, 'cache-clear all');
      provision_backend_invoke($destination, 'cache-clear all');
    }
    elseif (drush_drupal_major_version(d($destination)->root) == 8) {
      provision_backend_invoke($destination, 'cache-rebuild');
    }
    else {
      drush_log(dt('Drupal version 8 or earlier not detected. Not clearing caches.'), 'warning');
    }
  }
  else {
    drush_log(dt('Skipped clearing all caches...'), 'ok');
  }

  // Invoke hook_provision_sync_after().
  drush_command_invoke_all('provision_sync_after', $source, $destination);

  provision_drupal_push_site(TRUE);
}

/**
 * Rollback in the case of failure.
 */
function drush_provision_sync_rollback($source = NULL, $destination = NULL) {
  $source = drush_get_option('source', $source);
  $destination = drush_get_option('destination', $destination);

  $backup_file = drush_get_option('backup_file');
  if (empty($backup_file)) {
    return drush_set_error('ROLLBACK_FAILED', dt('Unable to rollback because backup file could not be found.'));
  }

  // Attempt to rollback
  drush_log(dt('Restoring from backup made before sync.'));
  $result = provision_backend_invoke($destination, 'provision-restore', array($backup_file));

  // Remove the backup made before the restore -- we don't ever need it!
  unlink($result['context']['backup_file']);

  // Clean-up
  drush_provision_sync_post_provision_sync($source, $destination);

  if ($result['error_status'] !== DRUSH_SUCCESS) {
    return drush_set_error('ROLLBACK_FAILED', dt('Unable to rollback because we failed to restore from backup.'));
  }
}

/**
 * After the command is complete.
 */
function drush_provision_sync_post_provision_sync($source = NULL, $destination = NULL) {
  // If the user didn't want a backup, we discard it.
  if (!drush_get_option('backup') && !drush_get_option('disable-rollback-backup')) {
    unlink(drush_get_option('backup_file'));
    drush_set_option('backup_file', NULL);
    drush_set_option('backup_file_size', NULL);
  }
}

