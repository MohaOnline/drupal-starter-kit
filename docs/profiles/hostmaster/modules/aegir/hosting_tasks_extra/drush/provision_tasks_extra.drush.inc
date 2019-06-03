<?php

/**
 * @file
 * Provision/Drush hooks for the provision_tasks_extra module.
 *
 * These are the hooks that will be executed by the drush_invoke function.
 */

/**
 * Implementation of hook_drush_command().
 */
function provision_tasks_extra_drush_command() {
  $items['provision-flush_cache'] = array(
    'description' => 'Flushes all caches on a site',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['provision-rebuild_registry'] = array(
    'description' => 'Rebuilds the registry on a site',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['provision-run_cron'] = array(
    'description' => 'Runs cron on a site',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['provision-update'] = array(
    'description' => dt('Runs db updates on a site'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
  );
  $items['provision-update_translations'] = array(
    'description' => dt('Update translations on a site'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['provision-flush_drush_cache'] = array(
    'description' => 'Flushes the Drush cache on the server',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['provision-update_uri'] = array(
    'description' => 'Update a site database to reflect a changed URI. This does NOT rename the whole site, use a provision-migrate task for that.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'old_uri' => 'Previous name of subdirectory within /sites or a site-alias.',
      'uri' => 'New name of subdirectory within /sites or a site-alias.',
    ),
    'examples' => array(
      'drush provision-update_uri www.example.com dev.example.com'
        => 'Update many references to the www.example.com uri in the site database'
    ),
  );
  $items['provision-features_revert_all'] = array(
    'description' => 'Revert all Features modules overrides on this site.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'features' => 'Optional: List of features to revert.  If left empty, all features will be reverted.',
    ),
  );
  $items['provision-features_update_all'] = array(
    'description' => 'Update all Features modules on this site: write overrides to files.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'features' => 'Optional: List of features to update.  If left empty, all features will be updated.',
      'revert' => 'Optional: Run a features-revert-all --force command after updating.',
    ),
  );

  return $items;
}

/**
 * Implements the provision-flush_cache command.
 */
function drush_provision_tasks_extra_provision_flush_cache() {
  drush_errors_on();
  if (drush_drupal_major_version(d()->root) == 8) {
    provision_backend_invoke(d()->name, 'cache-rebuild');
  }
  else {
    provision_backend_invoke(d()->name, 'cache-clear all');
  }
  drush_log(dt('All caches cleared'));
}

/**
 * Implements the provision-rebuild_registry command.
 */
function drush_provision_tasks_extra_provision_rebuild_registry() {
  drush_errors_on();
  if (d()->type === 'site') {
    if (drush_drupal_major_version(d()->root) == 7) {
      provision_backend_invoke(d()->name, 'registry-rebuild --fire-bazooka');
      drush_log(dt('Registry rebuilt with --fire-bazooka plus all caches cleared'));
    }
    else {
      provision_backend_invoke(d()->name, 'registry-rebuild');
      drush_log(dt('Registry rebuilt w/o --fire-bazooka plus all caches cleared'));
    }
  }
}

/**
 * Implements the provision-run_cron command.
 */
function drush_provision_tasks_extra_provision_run_cron() {
  drush_errors_on();
  provision_backend_invoke(d()->name, 'core-cron');
  drush_log(dt('Drush core-cron task completed'));
}

/**
 * Implements the provision-update command.
 */
function drush_provision_tasks_extra_provision_update() {
  drush_errors_on();
  $options = d()->options;
  if (isset($options['entity_updates']) && $options['entity_updates'] && drush_drupal_major_version(d()->root) >= 8) {
    provision_backend_invoke(d()->name, 'updatedb',  array('--entity-updates'));
    drush_log(dt('Drush updatedb task completed with --entity-updates'));
  }
  else {
    provision_backend_invoke(d()->name, 'updatedb');
    drush_log(dt('Drush updatedb task completed w/o --entity-updates'));
  }
}

/**
 * Implements the provision-update_translations command.
 */
function drush_provision_tasks_extra_provision_update_translations() {
  drush_errors_on();
  if (drush_drupal_major_version(d()->root) == 8) {
    provision_backend_invoke(d()->name, 'locale-check');
    drush_log(dt('Drush locale-check task completed'));
    provision_backend_invoke(d()->name, 'locale-update');
    drush_log(dt('Drush locale-update task completed'));
  }
  else {
    provision_backend_invoke(d()->name, 'l10n-update-refresh');
    drush_log(dt('Drush l10n-update-refresh task completed'));
    provision_backend_invoke(d()->name, 'l10n-update');
    drush_log(dt('Drush l10n-update task completed'));
  }
}

/**
 * Implements drush_hook_COMMAND_pre_validate().
 *
 * Make a backup of the site before we mess with it.
 */
function drush_provision_tasks_extra_provision_update_pre_validate() {
  /* Set offline mode to true and re-generate the settings.php. This will write a
   * $conf['site_offline'] = 1; to the settings.php
   */

  $list = drush_commandfile_list();
  $provision_dir = dirname($list['provision']);
  include_once($provision_dir . '/platform/backup.provision.inc');

  drush_log(dt("Putting the site under maintenance"));
  d()->site_enabled = FALSE;
  _provision_drupal_create_settings_file();
  provision_drupal_push_site();

  $backup_file = drush_provision_drupal_provision_backup_suggest_filename();
  drush_set_option('pre_update_backup_file', $backup_file);

  drush_set_option('force', true);
  $success = drush_invoke_process('@self', 'provision-backup', array($backup_file));
  if (!$success) {
    // If the backup failed, we don't want to try restoring it in the rollback.
    drush_set_option('pre_update_backup_file', FALSE);
  }
}

/**
 * Implements drush_hook_COMMAND_rollback().
 */
function drush_provision_tasks_extra_provision_update_rollback() {
  $restore_file = drush_get_option('pre_update_backup_file', FALSE);

  if ($restore_file) {
    drush_set_option('force', true);
    drush_invoke_process('@self', 'provision-restore', array($restore_file));
    drush_unset_option('force');
  }

  // Set site_offline to false and regenerate the config
  drush_log(dt("Bringing the site out of maintenance"));
  d()->site_enabled = TRUE;
  _provision_drupal_create_settings_file();
  provision_drupal_push_site();
}

/**
 * Implements drush_hook_post_COMMAND().
 */
function drush_provision_tasks_extra_post_provision_update() {
  // Set site_offline to false and regenerate the config
  drush_log(dt("Bringing the site out of maintenance"));
  d()->site_enabled = TRUE;
  _provision_drupal_create_settings_file();
  provision_drupal_push_site();
}

/**
 * Implements the provision-flush_drush_cache command.
 */
function drush_provision_tasks_extra_provision_flush_drush_cache() {
  drush_errors_on();
  provision_backend_invoke(d()->name, 'cache-clear drush');
  drush_log(dt('Drush cache on the server cleared'));
}

/**
 * Implements the provision-update_uri command.
 *
 * Expects site uri's as the arguments.
 */
function drush_provision_tasks_extra_provision_update_uri($old_uri, $new_uri) {
  // Delegate to provision's deploy engine, found in provision/platform/drupal/deploy*.
  // This handles path and uri changes
  drush_set_option('uri', $new_uri);
  drush_set_option('old_uri', $old_uri);

  drush_include_engine('drupal', 'deploy', drush_drupal_major_version());
}

/**
 * Implements the provision-features_revert_all command.
 */
function drush_provision_tasks_extra_provision_features_revert_all() {
  $drush_alias = d()->name;
  $features = trim(drush_get_option('features', ''));
  $options = "-y";

  if (drush_get_option('force_revert')) {
    $options .= " --force";
  }

  // Clear drush cache, just in case it is needed.
  provision_process("drush {$drush_alias} cc drush", NULL, dt('Clearing drush cache...'));

  // If features were specified...
  if (!empty($features)) {
    provision_process("drush {$drush_alias} features-revert {$features} {$options}", NULL, dt('Revert Features: !feature', [
      '!feature' => $features
    ]));
  }
  else {
    provision_process("drush {$drush_alias} features-revert-all {$options}", NULL, dt('Revert all Features'));
  }
}

/**
 * Implements the provision-features_update_all command.
 */
function drush_provision_tasks_extra_provision_features_update_all() {
  if (d()->type == 'site') {
    $drush_alias = d()->name;
    $features = trim(drush_get_option('features', ''));
    $revert = drush_get_option('revert', FALSE);

    // Clear drush cache, just in case it is needed.
    provision_process("drush {$drush_alias} cc drush", NULL, dt('Clearing drush cache...'));

    // If features were specified...
    if (!empty($features)) {
      provision_process("drush {$drush_alias} features-update {$features} -y", NULL, dt('Update Features: !feature', [
        '!feature' => $features
      ]));
    }
    else {
      provision_process("drush {$drush_alias} features-update-all -y", NULL, dt('Update All Features'));
    }

    // Revert force revert the features if the user asked.
    if ($revert) {
      if (!empty($features)) {
        provision_process("drush {$drush_alias} features-revert {$features} --force -y", NULL, dt('Revert Features'));
      }
      else {
        provision_process("drush {$drush_alias} features-revert-all --force -y", NULL, dt('Revert Features'));
      }
    }
  }
}

/**
 * Implements drush_HOOK_pre_COMMAND().
 *
 * This runs for each tasks during the command
 *    drush @hostmaster hosting-tasks
 *
 * NOTE: This ONLY runs when being called from a hostmaster task.
 * This hook should ONLY be used to pass Options from a hostmaster task form to
 * the $task object, or if you don't need this functionality from the command
 * line.
 */
function drush_provision_tasks_extra_pre_hosting_task() {
  $task = &drush_get_context('HOSTING_TASK');

  // Update.
  if ($task->ref->type == 'site' && $task->task_type == 'update' && isset($task->task_args['entity_updates'])) {
    $task->options['entity_updates'] = $task->task_args['entity_updates'];
  }
}
