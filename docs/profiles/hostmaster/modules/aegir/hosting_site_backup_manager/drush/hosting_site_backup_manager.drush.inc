<?php

/**
 * @file
 * Provision/Drush hooks for the provision-export_backup command.
 *
 * These are the hooks that will be executed by the drush_invoke function
 * when asking for an export backup command
 */

/**
 * Implements hook_drush_command().
 */
function hosting_site_backup_manager_drush_command() {
  $items['provision-export_backup'] = array(
    'description' => 'Make a site backup available for download.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
    'arguments' => array(
      'backup' => dt('The complete filepath of the backup to export.'),
    ),
  );

  $items['provision-remove_export_backup'] = array(
    'description' => 'Make an exported site backup unavailable for download.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
    'arguments' => array(
      'export' => dt('The complete filepath of the exported backup to remove.'),
    ),
  );

  return $items;
}

/**
 * Map values of site node into command line arguments.
 */
function drush_hosting_site_backup_manager_pre_hosting_task($task) {
  $task =& drush_get_context('HOSTING_TASK');

  // Pass the dialog entries to the drush provision script.
  if ($task->task_type == 'export_backup') {
    $task->args[1] = $task->task_args['backup'];
  }

  // Pass the task arguments to the drush provision script.
  if ($task->task_type == 'remove_export_backup') {
    $task->args[1] = $task->task_args['export'];
  }

}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_hosting_site_backup_manager_provision_export_backup($backup) {
  drush_log(dt('Start export backup command'), 'ok');

  // Path for exported backups. See also the frontend variable aegir_backup_export_path.
  $default_backup_exports = d('@server_master')->aegir_root . "/backup-exports";
  $backupdir = drush_get_option('provision_backup_export_path', $default_backup_exports);
  if (!is_dir($backupdir)) {
    provision_file()->create_dir($backupdir, dt('General backup export directory'), 0751);
  }

  if (drush_shell_cd_and_exec($backupdir, 'ln %s .', $backup)) {
    drush_log(dt('Backup exported', array()), 'ok');
  }
  else {
    // The command failed.
    return drush_set_error('DRUSH_PROVISION_EXPORT_BACKUP_FAILED',
      dt("Drush was unable to export the backup.\nThe specific errors are below:\n!errors",
      array(
        '!path' => $backupdir,
        '!errors' => implode("\n", drush_shell_exec_output()))
      )
    );
  }
}

/**
 * Implements the provision-remove_export_backup command.
 */
function drush_hosting_site_backup_manager_provision_remove_export_backup($export) {
  drush_log(dt('Start remove export backup command'), 'ok');

  // Path for exported backups. See also the frontend variable aegir_backup_export_path.
  $default_backup_exports = d('@server_master')->aegir_root . "/backup-exports";
  $backupdir = drush_get_option('provision_backup_export_path', $default_backup_exports);

  if (drush_shell_cd_and_exec($backupdir, 'rm %s', $export)) {
    drush_log(dt('Backup export removed', array()), 'ok');
  }
  else {
    // The command failed.
    return drush_set_error('DRUSH_PROVISION_REMOVE_EXPORT_BACKUP_FAILED',
      dt("Drush was unable to remove the export of the backup.\nThe specific errors are below:\n!errors",
      array(
        '!path' => $backupdir,
        '!errors' => implode("\n", drush_shell_exec_output()))
      )
    );
  }
}
