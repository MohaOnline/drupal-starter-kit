<?php

/**
 * @file
 * Baidu Map admin configuration functions.
 */

/**
 * Menu callback: Baidu Map Settings form.
 */
function baidu_map_settings($form, &$form_state) {

  // A Baidu Map API Key has exactly 24 or 32 alphanumeric characters.
  $form['baidu_map_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Baidu Map API Key'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#description' => t('Configure the <em>Baidu Map API Key</em> which should be in the form of <em>0123456789abcdefghijklmn</em> with exactly 24 or 32 alphanumeric characters (only allowed: 0 to 9 and a to z lower or upper case).<br/>A Key could be obtained by applying for an account on the <a href="@link_baidu_api" target="_blank">Baidu API</a> website.', array('@link_baidu_api' => 'http://lbsyun.baidu.com/apiconsole/key')),
    '#default_value' => variable_get('baidu_map_api_key'),
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_validate().
 *
 * Verify that the API key is a string of 24 or 32 alphanumeric characters.
 */
function baidu_map_settings_validate($form, &$form_state) {
  $api_key = &$form_state['values']['baidu_map_api_key'];
  $api_key = trim($api_key);
  // Ensure the tracker Key contains exactly 24 or 32 alphanumeric characters.
  if (!preg_match(BAIDU_MAP_API_KEY_VALIDATE_REGEX, $api_key)) {
    form_set_error('baidu_map_api_key', t('The value provided for the <em>Baidu Map API Key</em> is <strong>incorrect</strong>.<br/>Please provide a valid API Key which should contain exactly 24 or 32 alphanumeric characters (only allowed: 0 to 9 and a to z lower or upper case).'));
  }
}
