<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_auth_variable_info($options) {
  $v['campaignion_auth_api_key'] = [
    'title' => t('API key'),
    'description' => t('Public & secret key for authenticating against backend apps.'),
    'type' => 'properties',
    'default' => [],
    'localize' => FALSE,
  ];
  $v['campaignion_auth_app_url'] = [
    'title' => t('Auth app URL'),
    'description' => t('URL for the auth app excluding the version prefix.'),
    'type' => 'url',
    'default' => '',
    'localize' => FALSE,
  ];
  $v['campaignion_organization'] = [
    'title' => t('Impact-stack organization'),
    'description' => t('Machine name of the impact-stack organization owning the data of this installation.'),
    'type' => 'string',
    'default' => basename(conf_path()),
    'localize' => FALSE,
  ];
  return $v;
}
