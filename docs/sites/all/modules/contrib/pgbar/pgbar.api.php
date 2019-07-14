<?php

/**
 * @file
 * Example hook implementations. There is no actual executed code in this file.
 */

/**
 * Register a pgbar source plugin.
 *
 * @return array
 *   An array of fully-qualified class-names keyed by a machine name. Each
 *   plugin must implement the \Drupal\pgbar\Source\PluginInterface.
 */
function hook_pgbar_source_plugin_info() {
  $plugins['webform_submission_count'] = '\\Drupal\\pgbar\\Source\\WebformSubmissionCount';
  return $plugins;
}
