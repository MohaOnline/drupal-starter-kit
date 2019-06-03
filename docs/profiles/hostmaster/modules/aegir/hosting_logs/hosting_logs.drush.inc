<?php

/**
 * Implements hook_hosting_TASK_OBJECT_context_options().
 */
function hosting_logs_hosting_site_context_options(&$task) {
  
  // If we're actually provisioning from Git, populate real data.
  if (isset($task->ref->logs_enabled)) {
    $task->context_options['logs_enabled'] = $task->ref->logs_enabled;
    $task->context_options['logs_available'] = $task->ref->logs_available;
    $task->context_options['logs_visible'] = $task->ref->logs_visible;
  }
}