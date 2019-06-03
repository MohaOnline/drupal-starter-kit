<?php

/**
 * @file
 * Provide an admin screen for the ServiceCatalog download interface.
 */

use Drupal\dvg_authentication_service_catalog\ServiceCatalog;

/**
 * Callback for the Service Catalog form.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   The current state of the form.
 *
 * @return array
 *   The auto logout configuration form.
 */
function dvg_authentication_service_catalog_form(array $form, array &$form_state) {
  $catalog = new ServiceCatalog();
  $options = $catalog->getServiceTypes();

  if (empty($options)) {
    drupal_set_message(t('Unable to generate the Service Catalog, no SAML services found.'), 'error');
    return [];
  }

  $form += $catalog->getInfo();
  $form['service_types'] = [
    '#type' => 'checkboxes',
    '#title' => t('Service types'),
    '#description' => t('Select the service types to add in the catalog file.'),
    '#options' => drupal_map_assoc($options),
    '#default_value' => $options,
  ];
  $form['#submit'][] = 'dvg_authentication_service_catalog_form_submit';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download XML file'),
  );
  return $form;
}

/**
 * Submit handler for the service catalog.
 *
 * Generates and forces the ServiceCatalog XML file as a download.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The Form state.
 *
 * @throws \Exception
 */
function dvg_authentication_service_catalog_form_submit(array $form, array &$form_state) {
  $catalog = new ServiceCatalog();
  $services = array_filter($form_state['values']['service_types']);
  $catalog->downloadCatalogXml($services);
}

/**
 * Provides a simple download for simplesaml metadata.
 *
 * Adds the correct headers for filename and file extension.
 *
 * @param string $service
 *   Name of the service to get the metadata for.
 *
 * @see ServiceCatalog::downloadMetadata()
 */
function dvg_authentication_service_catalog_metadata_download($service) {
  $catalog = new ServiceCatalog();
  $catalog->downloadMetadata($service);
}
