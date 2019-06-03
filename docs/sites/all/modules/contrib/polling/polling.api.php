<?php

/**
 * Register a field type based polling-plugin.
 *
 * @return array
 *   An array keyed by field types. Values are either one or a list of fully
 *   qualified class names. All classes must implement the
 *   @see \Drupal\polling\FieldTypePluginInterface
 */
function hook_polling_field_type_plugin_info() {
  $plugins['myfieldtype'] = '\\Drupal\\my_module\\MyPollingPlugin';
  $plugins['other'] = [
    '\\Drupal\\my_module\\Plugin1',
    '\\Drupal\\my_module\\Plugin2',
  ];
  return $plugins;
}

/**
 * Register a plugin for the global polling page.
 *
 * @return array
 *   An array with fullý qualified class names as values. All classes must
 *   implement the @see \Drupal\polling\GlobalPluginInterface .
 */
function hook_polling_global_plugin_info() {
  $plugins[] = '\\Drupal\\my_module\\MyPollingPlugin';
  return $plugins;
}
