<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_email_to_target_variable_info($options) {
  $v['campaignion_email_to_target_credentials'] = [
    'title' => t('e2t api credentials'),
    'description' => t('API endpoint and API keys for the e2t-api service.'),
    'type' => 'properties',
    'default' => [],
    'localize' => FALSE,
  ];
  return $v;
}

