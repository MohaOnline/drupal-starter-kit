<?php

/**
 * Implements drush_hook_pre_hosting_task()
 *
 * Runs before the command "hosting-task" is run. Passes "task_args" from
 * the task node into "$task->options" which become drush options.
 */
function drush_hosting_tasks_extra_pre_hosting_task()
{
  $task =& drush_get_context('HOSTING_TASK');
  if ($task->task_type == 'features_update_all' || $task->task_type == 'features_revert_all') {
    $task->options['features'] = $task->task_args['features'];
    $task->options['revert'] = $task->task_args['revert'];
    $task->options['force_revert'] = $task->task_args['force_revert'];
  }
}
