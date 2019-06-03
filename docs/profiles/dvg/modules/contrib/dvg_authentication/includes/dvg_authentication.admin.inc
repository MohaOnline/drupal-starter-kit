<?php

/**
 * @file
 * Provides administration forms for DvG Authentication.
 */

use Drupal\dvg_authentication\AuthenticationManager;

/**
 * @file
 * Provide admin screens to manage the Authentication Manager settings.
 */

/**
 * Callback to build the configuration form for an AuthenticationProvider.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   The current state of the form.
 * @param string $provider_id
 *   The machine name of the AuthenticationProvider.
 *
 * @return array
 *   The provider-specific configuration form.
 */
function dvg_authentication_provider_configuration_form(array $form, array &$form_state, $provider_id) {
  $manager = AuthenticationManager::getInstance();
  $provider = $manager->getAuthenticationProvider($provider_id);

  if (!$provider) {
    drupal_set_message(t('Authentication Provider %provider_id not found.', ['%provider_id' => $provider_id]), 'error');
    return [];
  }

  // Get the configuration form for this setting.
  $form = $provider->getConfigurationForm($form, $form_state);
  $form['#validate'][] = 'dvg_authentication_provider_configuration_form_validate';
  $form['#submit'][] = 'dvg_authentication_provider_configuration_form_submit';
  return system_settings_form($form);
}

/**
 * Validate handler for the AuthenticationProvider configuration form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The current form state.
 */
function dvg_authentication_provider_configuration_form_validate(array $form, array &$form_state) {
  if ($provider_id = $form_state['values']['provider_id']) {
    $provider = AuthenticationManager::getInstance()
      ->getAuthenticationProvider($provider_id);
    if ($provider) {
      $provider->configurationFormValidate($form, $form_state);
    }
  }
}

/**
 * Submit handler for the AuthenticationProvider configuration form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The current form state.
 */
function dvg_authentication_provider_configuration_form_submit(array $form, array &$form_state) {
  if ($provider_id = $form_state['values']['provider_id']) {
    $provider = AuthenticationManager::getInstance()
      ->getAuthenticationProvider($provider_id);

    // Unset the value because we don't want it to be stored in the database.
    unset($form_state['values']['provider_id']);
    if ($provider) {
      $provider->configurationFormSubmit($form, $form_state);
    }
  }
}
