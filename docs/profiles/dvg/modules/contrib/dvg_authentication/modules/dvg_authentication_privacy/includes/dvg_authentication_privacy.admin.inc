<?php

/**
 * @file
 * Provides administration form for DvG Authentication Privacy.
 */

use Drupal\dvg_authentication_privacy\Privacy;

/**
 * Callback for the privacy settings form.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   The current state of the form.
 *
 * @return array
 *   The privacy configuration form.
 */
function dvg_authentication_privacy_settings_form(array $form, array &$form_state) {
  $privacy = new Privacy();
  $form = $privacy->getConfigurationForm($form, $form_state);
  return system_settings_form($form);
}
