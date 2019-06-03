<?php

/**
 * @file
 * Contains all admin pages, settings, and validate.
 */

/**
 * Settings form for menu callback
 */
function dvg_digid_autologout_settings() {
  $form = array();
  $timeout = variable_get('dvg_digid_autologout_timeout', 900);
  $max_session_time = variable_get('dvg_digid_autologout_max_session_time', 7200);

  $form['dvg_digid_autologout_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout value in seconds'),
    '#default_value' => $timeout,
    '#size' => 8,
    '#weight' => -10,
    '#description' => t('The length of inactivity time, in seconds, before automated log out. Must be %min_timeout seconds or greater. Will not be used if role timeout is activated.', array('%min_timeout' => DVG_DIGID_AUTOLOGOUT_MIN_TIMEOUT)),
  );

  $form['dvg_digid_autologout_max_session_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Max session time in seconds'),
    '#default_value' => $max_session_time,
    '#size' => 8,
    '#weight' => -10,
    '#description' => t('The maximum length of session time, in seconds, before automated log out. Must be %min_timeout seconds or greater and greater than or equal to the timeout value. Will not be used if role timeout is activated.', array('%min_timeout' => DVG_DIGID_AUTOLOGOUT_MIN_TIMEOUT)),
  );

  $form['dvg_digid_autologout_inactivity_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message to display to the user after they are logged out.'),
    '#default_value' => variable_get('dvg_digid_autologout_inactivity_message', 'You have been logged out due to inactivity.'),
    '#size' => 40,
    '#description' => t('This message is displayed after the user was logged out due to inactivity. You can leave this blank to show no message to the user.'),
  );

  $form['dvg_digid_autologout_redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URL at logout'),
    '#default_value' => variable_get('dvg_digid_autologout_redirect_url', 'user/login'),
    '#size' => 40,
    '#description' => t('Send users to this internal page when they are logged out.'),
  );

  $form['dvg_digid_autologout_use_watchdog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable watchdog Automated Logout logging'),
    '#default_value' => variable_get('dvg_digid_autologout_use_watchdog', ''),
    '#description' => t('Enable logging of automatically logged out users'),
  );
  return system_settings_form($form);
}

/**
 * Settings validation.
 */
function dvg_digid_autologout_settings_validate($form, &$form_state) {
  $max_session_timeout = $form_state['values']['dvg_digid_autologout_max_session_time'];

  $timeout = $form_state['values']['dvg_digid_autologout_timeout'];

  // Validate timeout.
  if (!is_numeric($timeout) || ((int) $timeout != $timeout) || $timeout < DVG_DIGID_AUTOLOGOUT_MIN_TIMEOUT || $timeout > $max_session_timeout) {
    form_set_error('autologout_timeout', t('The timeout must be an integer greater than %min and less then %max.', array('%min' => DVG_DIGID_AUTOLOGOUT_MIN_TIMEOUT, '%max' => $max_session_timeout)));
  }
}
