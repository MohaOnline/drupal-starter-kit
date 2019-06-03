<?php

/**
 * @file Provision hooks to store data in the platform context/alias.
 */

/**
 * Implements hook_hosting_TASK_OBJECT_context_options().
 */
function hosting_platform_composer_hosting_platform_context_options(&$task) {
  if ($task->ref->type == 'platform') {
    foreach (['composer_project_path', 'composer_project_package', 'composer_project_version'] as $option) {
      $field = 'field_' . $option;
      if (isset($task->ref->$field) && !empty($task->ref->{$field})) {
        $task->context_options[$option] = $task->ref->{$field}[$task->ref->language][0]['value'];
      }
    }
  }
}

/**
 * Implements hook_drush_context_import().
 */
function hosting_platform_composer_drush_context_import($context, &$node) {
  if ($context->type == 'platform') {
    foreach (['composer_project_path', 'composer_project_package', 'composer_project_version'] as $option) {
      if (isset($context->$option) && !empty($context->$option)) {
        $field = 'field_' . $option;
        $node->$field[$node->language][0]['value'] = $context->$option;
      }
    }
  }
}

