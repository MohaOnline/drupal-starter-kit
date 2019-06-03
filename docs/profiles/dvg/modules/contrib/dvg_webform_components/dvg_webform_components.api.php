<?php

/**
 * @file
 * Hooks provided by the DVG Webform Components module.
 */

/**
 * Hook to add a custom service to the dvg address component
 *
 * @param array $options
 *  Services.
 */
function hook_dvg_webform_components_address_options_alter(&$options) {
  $options['dvg-external-service'] = t('DVG external service address check');
}

/**
 * Hook to autocomplete dvg address component by external services.
 *
 * @param array $data
 *  Data containing the street and city.
 * @param $address
 *  The current address containing postal_code, house_number, house_letter and house_number_addition.
 * @param $service
 *  The selected service
 */
function hook_dvg_webform_components_address_autofill_alter(&$data, $address, $service) {
  if ($service === 'dvg-external-service') {
    $data['street'] = 'Street';
    $data['city'] = 'City';
  }
}
