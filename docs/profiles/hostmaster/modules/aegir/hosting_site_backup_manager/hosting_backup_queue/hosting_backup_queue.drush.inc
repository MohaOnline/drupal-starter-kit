<?php

/**
 * @file
 * Drush hook implementations.
 */

/**
 * Implements hook_hosting_post_backup().
 *
 * Changes the last backup timestamp of the site to the task time.
 */
function hosting_backup_queue_post_hosting_backup_task($task, $data) {
  if ($data['context']['backup_file'] && $task->ref->type == 'site') {
    hosting_backup_queue_backup_time_update($task->ref->nid, $task->executed);
  }
}

/**
 * Implements hook_hosting_post_backup_delete().
 *
 * Changes the last backup timestamp of the site to the task time.
 */
function hosting_backup_queue_post_hosting_backup_delete_task($task, $data) {
  if ($task->ref->type == 'site') {
    hosting_backup_queue_backup_time_clear($task->ref->nid);
  }
}
