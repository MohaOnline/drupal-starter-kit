<?php

/**
 * @file
 * Metadata for configuradion variables used by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_loqate_variable_info($options) {
  $v['campaignion_loqate_api_key'] = [
    'title' => t('Loqate API-key'),
    'type' => 'string ',
    'default' => '',
    'localize' => FALSE,
  ];
  return $v;
}
