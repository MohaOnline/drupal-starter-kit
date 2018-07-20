<?php

/**
 * @file
 * Definition of variables for Variable API module integration with Baidu Map.
 */

/**
 * Implements hook_variable_info().
 */
function baidu_map_variable_info($options) {
  // Baidu Map defaults settings.
  $defaults = array(
    'group' => 'baidu_map',
    'localize' => TRUE,
    'multidomain' => TRUE,
  );
  // A Baidu Map API Key has exactly 24 or 32 alphanumeric characters.
  $variables['baidu_map_api_key'] = array(
    'type' => 'string',
    'title' => t('Baidu Map API Key', array(), $options),
    'default' => NULL,
    'element' => array(
      '#maxlength' => 32,
      '#size' => 32,
    ),
    'description' => t('Configure the <em>Baidu Map API Key</em> which should be in the form of <em>0123456789abcdefghijklmn</em> with exactly 24 or 32 alphanumeric characters (only allowed: 0 to 9 and a to z lower or upper case).<br/>A Key could be obtained by applying for an account on the <a href="@link_baidu_api" target="_blank">Baidu API</a> website.', array('@link_baidu_api' => 'http://lbsyun.baidu.com/apiconsole/key')),
    'required' => TRUE,
    'validate callback' => 'baidu_map_validate_baidu_map_api_key',
  ) + $defaults;

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function baidu_map_variable_group_info() {
  $groups['baidu_map'] = array(
    'title' => t('Baidu Map'),
    'description' => t('The Baidu Map module provides integration with the <a href="@bmapi_link">Baidu Map API</a> to allow display of geographic and location information on maps.<br/>Please configure module\'s settings by filling the form below:', array('@bmapi_link' => 'http://lbsyun.baidu.com/apiconsole/key')),
    'access' => 'administer baidu map',
    'path' => array('admin/config/services/baidu_map'),
  );

  return $groups;
}

/**
 * Validate Baidu Map API Key variable.
 */
function baidu_map_validate_baidu_map_api_key($variable) {
  $variable['value'] = trim($variable['value']);
  // Ensure the tracker Key contains exactly 24 or 32 alphanumeric characters.
  if (!preg_match(BAIDU_MAP_API_KEY_VALIDATE_REGEX, $variable['value'])) {
    return t('The value provided for the <em>Baidu Map API Key</em> is <strong>incorrect</strong>.<br/>Please provide a valid API Key which should contain exactly 24 or 32 alphanumeric characters (only allowed: 0 to 9 and a to z lower or upper case).');
  }
}
