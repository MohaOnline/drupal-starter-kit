<?php

/**
 * @file
 * Declares metadata for configuration variables.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_source_tags_variable_info($options) {
  $variables['campaignion_source_tags_new_tags'] = [
    'type' => 'boolean',
    'title' => t('Allow new tags to be added using URL-parameters.', [], $options),
    'default' => TRUE,
  ];
  return $variables;
}
