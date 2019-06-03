<?php
/**
 * @file
 * Functions and options specific for Safety region Midden- en West-Brabant
 */

/**
 * dvg_crisis_safety_region_middenwestbrabant().
 */
function dvg_crisis_safety_form_options_region_middenwestbrabant() {
  $form_options = array();
  $form_options ['dvg_crisis_safety_region_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('dvg_crisis_safety_region_api_key', ''),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  // Choose Version.
  $options = array(
    'V2' => t('Version 2'),
  );
  $form_options['dvg_crisis_safety_region']['dvg_crisis_safety_region_service_version'] = array(
    '#type' => 'select',
    '#title' => t('Select the service version'),
    '#options' => $options,
    '#default_value' => variable_get('dvg_crisis_safety_region_service_version', 0),
  );
  // Override Safety region banner! (turn off)
  $form_options['dvg_crisis_safety_region']['dvg_crisis_safety_region_debugging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Turn on debugging'),
    '#default_value' => variable_get('dvg_crisis_safety_region_debugging', FALSE),
    '#description' => t('Debugging will load an alert from V2 with key: F6AEAB39-BF56-4364-A865-8BB654D90742 (Demo)'),
  );

  return $form_options;
}

/**
 * dvg_crisis_safety_region_middenwestbrabant_include().
 */
function dvg_crisis_safety_region_middenwestbrabant_include() {
  $debugging = variable_get('dvg_crisis_safety_region_debugging', FALSE);
  $api_key = ($debugging ? "F6AEAB39-BF56-4364-A865-8BB654D90742" : variable_get('dvg_crisis_safety_region_api_key', ''));
  $version = ($debugging ? 'V2' : variable_get('dvg_crisis_safety_region_service_version', 'V2') );
  // We load the script async, we do not want to wait on external servers.
  $async_script = "// Async script loading
  jQuery(document).ready(function () {
    var s = document.createElement('script');
    s.type = 'text/javascript';
    s.async = true;
    s.src = 'https://www.vrmwb.nl/Gemeentebanners" . $version . "?c=" . $api_key . "';
    var x = document.getElementsByTagName('script')[0];
    x.parentNode.insertBefore(s, x);
  });";
  drupal_add_js($async_script, array('type' => 'inline', 'scope' => 'footer'));
  // Forcing the banner to be in the page not on top of it.
  drupal_add_css('#vhrmwb_container{position:relative!important;z-index:998!important}', 'inline');
}
