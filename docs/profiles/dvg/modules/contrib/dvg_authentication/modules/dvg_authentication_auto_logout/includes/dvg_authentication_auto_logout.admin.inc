<?php

/**
 * @file
 * Handles admin configuration for auto logout settings.
 */

use Drupal\dvg_authentication_auto_logout\AutoLogout;

/**
 * Callback for the auto logout settings form.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   The current state of the form.
 *
 * @return array
 *   The auto logout configuration form.
 */
function dvg_authentication_auto_logout_settings_form(array $form, array &$form_state) {
  $form['enabled_checkboxes'] = [
    '#type' => 'fieldset',
    '#title' => t('Enabled profiles for auto logout'),
    '#collapsible' => TRUE,
  ];

  $profiles = AutoLogout::getInstance()->getProfiles();
  foreach ($profiles as $profile => $details) {
    $form['enabled_checkboxes']['dvg_authentication_auto_logout_enabled_' . $profile] = [
      '#type' => 'checkbox',
      '#title' => $details['title'],
      '#default_value' => variable_get('dvg_authentication_auto_logout_enabled_' . $profile, $details['default_enabled'] ?? FALSE),
    ];
  }

  $settings = variable_get('dvg_authentication_auto_logout', DVG_AUTHENTICATION_AUTO_LOGOUT_DEFAULTS);
  $form['base_config'] = [
    '#type' => 'fieldset',
    '#title' => t('Base configuration'),
    '#collapsible' => TRUE,
    'dvg_authentication_auto_logout' => _dvg_authentication_auto_logout_build_settings_group($settings),
  ];

  if ($profiles) {
    $form['profile_config'] = [
      '#type' => 'fieldset',
      '#title' => t('Custom config for each profile'),
      '#description' => t('Base configuration will be used for any field that is left empty.'),
      '#collapsible' => TRUE,
    ];
  }

  foreach ($profiles as $profile_name => $details) {
    $setting_name = 'dvg_authentication_auto_logout_' . $profile_name;
    $settings = variable_get($setting_name, []);
    $deviates = array_filter($settings) ? ' * ' . t('deviates from base') : '';
    $form['profile_config'][$profile_name] = [
      '#type' => 'fieldset',
      '#title' => $details['title'] . $deviates,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#states' => [
        'visible' => [
          'input[name="dvg_authentication_auto_logout_enabled_' . $profile_name . '"]' => [
            'checked' => TRUE,
          ],
        ],
      ],
      $setting_name => _dvg_authentication_auto_logout_build_settings_group($settings),
    ];
  }

  $form['#validate'][] = 'dvg_authentication_auto_logout_settings_validate';

  return system_settings_form($form);
}

/**
 * Auto logout settings validation.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   The current state of the form.
 */
function dvg_authentication_auto_logout_settings_validate(array $form, array &$form_state) {
  $max_session_time = $form_state['values']['dvg_authentication_auto_logout']['max_session_time'];
  $timeout = $form_state['values']['dvg_authentication_auto_logout']['timeout'];
  $min_timeout = DVG_AUTHENTICATION_AUTO_LOGOUT_MIN_TIMEOUT;

  // Validate base config.
  if (
    !is_numeric($timeout) ||
    $timeout < $min_timeout ||
    $timeout > $max_session_time
  ) {
    form_set_error(
      'dvg_authentication_auto_logout][timeout',
      t('The timeout must be an integer greater than %min and less than %max.', ['%min' => $min_timeout, '%max' => $max_session_time])
    );
  }
  if (
    !is_numeric($max_session_time) ||
    $max_session_time < $timeout ||
    $max_session_time < $min_timeout
  ) {
    form_set_error(
      'dvg_authentication_auto_logout][max_session_time',
      t('The max session time must be an integer greater than both minimum %min and timeout %min_timeout.', ['%min_timeout' => $timeout])
    );
  }

  // Validate each profile.
  $auto_logout = AutoLogout::getInstance();
  foreach ($auto_logout->getProfiles() as $profile_name => $details) {
    _dvg_authentication_auto_logout_profile_settings_validate($form_state, $profile_name);
  }
}

/**
 * Validate helper for custom settings for each profile.
 *
 * @param array $form_state
 *   The current state of the form.
 * @param string $profile_name
 *   Profile name that is validated.
 */
function _dvg_authentication_auto_logout_profile_settings_validate(array &$form_state, $profile_name) {
  $min_timeout = DVG_AUTHENTICATION_AUTO_LOGOUT_MIN_TIMEOUT;
  $max_session_time = $form_state['values']['dvg_authentication_auto_logout']['max_session_time'];
  $profile_settings_var = 'dvg_authentication_auto_logout_' . $profile_name;

  if (!empty($form_state['values'][$profile_settings_var]['timeout'])) {
    $custom_timeout = $form_state['values'][$profile_settings_var]['timeout'];

    if (!empty($form_state['values'][$profile_settings_var]['max_session_time'])) {
      $custom_max_session = $form_state['values'][$profile_settings_var]['max_session_time'];
      if (
        !is_numeric($custom_max_session) ||
        $custom_max_session < $custom_timeout ||
        $custom_max_session < $min_timeout
      ) {
        form_set_error(
          $profile_settings_var . '][max_session_time',
          t('The max session time must be an integer greater than both minimum %min and timeout %min_timeout.', ['%min_timeout' => $custom_timeout])
        );
      }

    }
    else {
      $custom_max_session = $max_session_time;
    }

    if (
      !is_numeric($custom_timeout) ||
      $custom_timeout < $min_timeout ||
      $custom_timeout > $custom_max_session
    ) {
      form_set_error(
        $profile_settings_var . '][timeout',
        t('The timeout must be an integer greater than %min and less than %max.', [
          '%min' => $min_timeout,
          '%max' => $custom_max_session,
        ])
      );
    }
  }
}

/**
 * Build a form for a settings group.
 *
 * @param array $settings
 *   Current settings to set default values.
 *
 * @return array
 *   Partial form build.
 */
function _dvg_authentication_auto_logout_build_settings_group(array $settings) {
  $min_timeout = ['%min_timeout' => DVG_AUTHENTICATION_AUTO_LOGOUT_MIN_TIMEOUT];
  $group['#tree'] = TRUE;
  $group['timeout'] = [
    '#type' => 'textfield',
    '#title' => t('Timeout value in seconds'),
    '#default_value' => $settings['timeout'] ?? NULL,
    '#size' => 8,
    '#weight' => -10,
    '#description' => t('The length of inactivity time, in seconds, before automated log out. Must be %min_timeout seconds or greater.', $min_timeout),
  ];

  $group['max_session_time'] = [
    '#type' => 'textfield',
    '#title' => t('Max session time in seconds'),
    '#default_value' => $settings['max_session_time'] ?? NULL,
    '#size' => 8,
    '#weight' => -10,
    '#description' => t('The maximum length of session time, in seconds, before automated log out. Must be %min_timeout seconds or greater and greater than or equal to the timeout value.', $min_timeout),
  ];

  $group['inactivity_message'] = [
    '#type' => 'textarea',
    '#title' => t('Message to display to the user after they are logged out.'),
    '#default_value' => $settings['inactivity_message'] ?? NULL,
    '#size' => 40,
    '#description' => t('This message is displayed after the user was logged out due to inactivity. You can leave this blank to show no message to the user.'),
  ];

  $group['redirect_url'] = [
    '#type' => 'textfield',
    '#title' => t('Redirect URL at logout'),
    '#default_value' => $settings['redirect_url'] ?? NULL,
    '#size' => 40,
    '#description' => t('Send users to this internal page when they are logged out.'),
  ];

  $group['use_watchdog'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable watchdog logging'),
    '#default_value' => $settings['use_watchdog'] ?? NULL,
    '#description' => t('Enable logging of automatically logged out users'),
  ];
  return $group;
}
