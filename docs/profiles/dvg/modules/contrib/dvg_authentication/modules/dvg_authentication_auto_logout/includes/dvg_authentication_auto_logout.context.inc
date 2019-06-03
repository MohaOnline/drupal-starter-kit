<?php

/**
 * @file
 * Provide a context condition based on the active status of auto logout.
 *
 * Also creates a default context using the new condition.
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_authentication_auto_logout_ctools_plugin_api($module, $api) {
  if ($module === 'context' && ($api === 'plugins' || $api === 'context')) {
    return ['version' => 3];
  }
}

/**
 * Implements hook_context_plugins().
 */
function dvg_authentication_auto_logout_context_plugins() {
  $plugins = [];
  $plugins['dvg_authentication_auto_logout_active_context_condition'] = [
    'handler' => [
      'path' => drupal_get_path('module', 'dvg_authentication_auto_logout') . '/includes',
      'file' => 'AutoLogoutActiveContextCondition.inc',
      'class' => 'AutoLogoutActiveContextCondition',
      'parent' => 'context_condition',
    ],
  ];
  return $plugins;
}

/**
 * Implements hook_context_registry().
 */
function dvg_authentication_auto_logout_context_registry() {
  return [
    'conditions' => [
      'auto_logout_active' => [
        'title' => t('Auto logout status'),
        'description' => t('Use this context to react on the active status of auto logout for the logged in user.'),
        'plugin' => 'dvg_authentication_auto_logout_active_context_condition',
      ],
    ],
  ];
}

/**
 * Implements hook_context_page_condition().
 */
function dvg_authentication_auto_logout_context_page_condition() {
  /** @var \AutoLogoutActiveContextCondition $plugin */
  if ($plugin = context_get_plugin('condition', 'auto_logout_active')) {
    global $user;
    $plugin->execute($user);
  }
}

/**
 * Implements hook_context_default_contexts().
 */
function dvg_authentication_auto_logout_context_default_contexts() {
  $export = [];

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'auto_logout_info_bar';
  $context->description = 'Displays an auto logout bar to the logged in user.';
  $context->tag = 'authentication';
  $context->conditions = [
    'auto_logout_active' => [
      'values' => [
        'active' => 'active',
      ],
    ],
  ];
  $context->reactions = [
    'block' => [
      'blocks' => [
        'dvg_authentication_auto_logout-auto_logout_info' => [
          'module' => 'dvg_authentication_auto_logout',
          'delta' => 'auto_logout_info',
          'region' => 'top',
          'weight' => '-10',
        ],
      ],
    ],
  ];
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Displays an auto logout bar to the logged in user.');
  t('authentication');

  $export['auto_logout_info_bar'] = $context;
  return $export;
}
