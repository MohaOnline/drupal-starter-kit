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
