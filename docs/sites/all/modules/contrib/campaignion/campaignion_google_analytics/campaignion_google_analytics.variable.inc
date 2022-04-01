<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_google_analytics_variable_info($options) {
  $v['campaignion_google_analytics_id'] = [
    'title' => t('Google Analytics ID'),
    'description' => t('If an ID is set here the GA snippet for this ID is generated automatically.'),
    'type' => 'string',
    'default' => '',
    'localize' => FALSE,
  ];
  return $v;
}
