<?php

/**
 * @file
 * Plugin to provide access control/visibility based on existence of a
 * specified context
 */

$plugin = array(
  'title' => t("WSData"),
  'description' => t('Tests whether data is doing'),
  'callback' => 'wsconfig_wsdata_access_check',
  'settings form' => 'wsconfig_wsdata_access_settings',
  'summary' => 'wsconfig_wsdata_access_summary',
  'required context' => new ctools_context_required(t('WsConfig'), 'wsconfig'),
  'defaults' => array(
    'selector' => '',
    'value' => '',
  ),
);

/**
 * Settings form.
 */
function wsconfig_wsdata_access_settings($form, &$form_state, $conf) {
  $form['settings']['selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Data to select'),
    '#description' => t('Path to data'),
    '#default_value' => isset($conf['selector']) ? $conf['selector'] : '',
  );


  $form['settings']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#description' => t('is equal to'),
    '#default_value' => isset($conf['value']) ? $conf['value'] : '',
  );

  return $form;
}

/**
 * Check for access.
 */
function wsconfig_wsdata_access_check($conf, $context) {
  $data = ctools_context_convert_context($context, $conf['selector']);
  return $data == $conf['value'];
}

/**
 * Provide a summary description based upon the specified context.
 */
function wsconfig_wsdata_access_summary($conf, $context) {
  return t('@identifier: If "@selector" is equal to "@value"', array('@identifier' => $context->identifier, '@selector' => $conf['selector'], '@value' => $conf['value']));
}

