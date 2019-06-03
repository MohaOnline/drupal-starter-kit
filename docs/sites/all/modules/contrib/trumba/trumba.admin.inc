<?php
/**
 * @file
 * Administrative form for Trumba module.
 */

/**
 * Admin settings form.
 */
function trumba_admin_settings_form($form, &$form_state) {
  $form = array();

  // Collect the webname, used to identify the organization/account that the
  // spuds belong to.
  $form['trumba_webname'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Default Web Name'),
    '#description' => t('This is the default unique identifier for your account on Trumba.'),
    '#default_value' => variable_get('trumba_webname', ''),
  );

  return system_settings_form($form);
}

/**
 * Validates the values that have been set using the settings form.
 */
function trumba_admin_settings_form_validate($form, &$form_state) {
  // Make sure this not an attack.
  $form['trumba_webname']['#value'] = check_plain($form['trumba_webname']['#value']);
  // If it is, complain.
  if (empty($form['trumba_webname']['#value'])) {
    form_set_error('trumba_webname', 'Please enter a valid Trumba webname');
  }
}
