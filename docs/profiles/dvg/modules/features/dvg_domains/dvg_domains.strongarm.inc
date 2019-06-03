<?php
/**
 * @file
 * dvg_domains.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_domains_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'domain_entity_bypass_access_conditions_for_cron';
  $strongarm->value = 1;
  $export['domain_entity_bypass_access_conditions_for_cron'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'domain_force_admin';
  $strongarm->value = '1';
  $export['domain_force_admin'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'domain_settings_behavior';
  $strongarm->value = '1';
  $export['domain_settings_behavior'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'domain_settings_forms';
  $strongarm->value = 'functional_content_admin
dvg_authentication_provider_configuration_form
dvg_authentication_auto_logout_settings_form
dvg_global_texts
product_catalog_admin_form
dvg_payment_webform_config
piwik_admin_settings_form
webform_appointment_admin_form
dvg_digid_admin_form
dvg_datab_settings
dvg_crisis_dashboard_form';
  $export['domain_settings_forms'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'domain_settings_form_visibility';
  $strongarm->value = '1';
  $export['domain_settings_form_visibility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'mailsystem_theme';
  $strongarm->value = 'domain';
  $export['mailsystem_theme'] = $strongarm;

  return $export;
}
