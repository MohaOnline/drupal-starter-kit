<?php

/**
 * Product catalog configuration form.
 */
function product_catalog_admin_form() {
  $form = array();

  $form['product_catalog'] = array(
    '#markup' => t('Configuration page for the OWMS related settings.'),
  );

  $form['product_catalog_spatial'] = array(
    '#title' => t('dcterms:spatial value'),
    '#description' => t('Example: Vught'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('product_catalog_spatial', ''),
  );
  $form['product_catalog_spatial_id'] = array(
    '#title' => t('dcterms:spatial "resourceIdentifier" attribute'),
    '#description' => t('Example: http://standaarden.overheid.nl/owms/terms/Vught'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('product_catalog_spatial_id', ''),
  );

  $form = system_settings_form($form);

  return $form;
}
