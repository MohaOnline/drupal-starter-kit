<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_facebook_pixel_variable_info($options) {
  $v['campaignion_facebook_pixel_codes'] = [
    'title' => t('FB pixel codes'),
    'description' => t('Mapping of nids to Facebook pixel codes.'),
    'type' => 'properties',
    'default' => [],
    'localize' => FALSE,
  ];
  return $v;
}
