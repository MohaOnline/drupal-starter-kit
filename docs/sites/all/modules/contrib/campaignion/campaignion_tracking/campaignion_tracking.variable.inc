<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_tracking_variable_info($options) {
  $v['campaignion_tracking_default_event'] = [
    'title' => t('Tracking default event'),
    'description' => t('If not specified otherwise in the code tracking scripts wait for this JS event before loading external scripts. If this is left empty (default) scripts will be loaded immediately.'),
    'type' => 'string',
    'default' => '',
    'localize' => FALSE,
  ];
  return $v;
}
