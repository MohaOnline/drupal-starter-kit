<?php

/**
 * @file
 * Administrative page callbacks for Media: Skyfish.
 */

/**
 * Callback for skyfish settings form.
 *
 * @param string $arg
 *   If argument is set to user, it means it's users settings form,
 *   otherwise it's system global settings form.
 */
function media_skyfish_settings_form($form, $form_state, $arg = NULL) {
  $form = array();

  $key_name = media_skyfish_variable_name('api_key');
  $secret_key_name = media_skyfish_variable_name('api_secret');

  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Skyfish settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['api'][$key_name] = array(
    '#type' => 'textfield',
    '#title' => t('Skyfish API Key'),
    '#default_value' => media_skyfish_variable_get('api_key'),
    '#description' => t('Please enter Skyfish API Key here.'),
  );
  $form['api'][$secret_key_name] = array(
    '#type' => 'textfield',
    '#title' => t('Skyfish API Secret'),
    '#default_value' => media_skyfish_variable_get('api_secret'),
    '#description' => t('Please enter Skyfish API secret key.'),
  );

  return system_settings_form($form);
}
