<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_email_to_target_variable_info($options) {
  $v['campaignion_email_to_target_e2t_api_url'] = [
    'title' => t('Auth app URL'),
    'description' => t('URL for the e2t-api excluding the version prefix.'),
    'type' => 'url',
    'default' => '',
    'localize' => FALSE,
  ];
  $v['campaignion_email_to_target_dataset_query'] = [
    'title' => t('Dataset GET-query string'),
    'description' => t('Default query parameters for the dataset listing including the leading question mark.'),
    'type' => 'string',
    'default' => '',
    'localize' => FALSE,
  ];
  return $v;
}
