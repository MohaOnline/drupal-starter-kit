<?php

/**
 * @file
 * Drush hooks for the Hosting HTTPS module.
 */

/**
 * Implements hook_hosting_TASK_OBJECT_context_options().
 */
function hosting_https_hosting_site_context_options(&$task) {
  $node = $task->ref;

  if (isset($node->https_enabled)) {
    // We pass null to disable the option.
    $task->context_options['https_enabled'] = ($node->https_enabled) ? $node->https_enabled : 'null';

    if ($node->https_enabled) {
      $task->context_options['https_key'] = hosting_https_get_key($node);
    }
    else {
      $task->context_options['https_key'] = 'null';
    }
  }

  if (isset($node->https_client_authentication_enabled)) {
    $task->context_options['https_client_authentication_enabled'] = ($node->https_client_authentication_enabled) ? HOSTING_HTTPS_CLIENT_AUTHENTICATION_ENABLED : 'null';
    $task->context_options['https_client_authentication_path'] = ($node->https_client_authentication_path) ? $node->https_client_authentication_path : 'null';
  }
}

/**
 * Implements hook_drush_context_import().
 */
function https_drush_context_import($context, &$node) {
  if ($context->type == 'site') {
    $node->https_enabled = $context->https_enabled;
    $node->https_client_authentication_enabled = $context->https_client_authentication_enabled;
    $node->https_key = $context->https_key;
  }
}
