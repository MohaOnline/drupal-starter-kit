<?php

/**
 * Implements hook_post_hosting_remote_import_list_sites_task().
 *
 * After our list sites task runs, capture the results.
 */
function hosting_remote_import_post_hosting_remote_import_list_sites_task($task, $data) {
  if ($task->ref->type == 'server') {
    if (isset($data['context']['remote_sites_list']) && is_array($data['context']['remote_sites_list'])) {
      foreach ($data['context']['remote_sites_list'] as $url => $platform) {
        $id = db_insert('remote_import_sites')->fields(array(
          'tid' => $task->vid,
          'site' => $url,
          'platform' => $platform,
        ))->execute();
      }
    }
  }
}

/**
 *  Implements drush_hook_pres_hosting_task().
 *
 *  Convert the arguments for a 'remote import' task from IDs into strings and
 *  assign them as options to the drush command.
 */
function drush_hosting_remote_import_pre_hosting_task($task) {
  $task = &drush_get_context('HOSTING_TASK');

  if ($task->task_type == 'remote_import') {
    $task->options['old_url'] = $task->task_args['old_url'];
    $task->options['new_url'] = $task->task_args['new_url'];
    if ($platform = node_load($task->task_args['platform'])) {
      $task->options['platform'] = $platform->hosting_name;
    }
    if ($db_server = node_load($task->task_args['db_server'])) {
      $task->options['db_server'] = $db_server->hosting_name;
    }
  }

}

/**
 * Implements hook_drush_context_import().
 *
 *  As a new site is imported using hosting-import check to see if it came from
 *  a remote system, and if it did, then don't try to install it!
 */
function hosting_remote_import_drush_context_import($context, &$node) {
  // From hosting_alias_drush_context_import().
  if ($context->type == 'site') {
    if (drush_get_option('remote_import_import', FALSE)) {
      $node->import = TRUE;
    }
  }
}
