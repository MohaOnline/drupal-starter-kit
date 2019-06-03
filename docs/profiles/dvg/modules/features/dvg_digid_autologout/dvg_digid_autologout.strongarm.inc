<?php
/**
 * @file
 * dvg_digid.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_digid_autologout_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_digid_autologout_max_session_time';
  $strongarm->value = '7200';
  $export['dvg_digid_autologout_max_session_time'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_digid_autologout_redirect_url';
  $strongarm->value = '';
  $export['dvg_digid_autologout_redirect_url'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_digid_autologout_timeout';
  $strongarm->value = '900';
  $export['dvg_digid_autologout_timeout'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_digid_autologout_use_watchdog';
  $strongarm->value = 0;
  $export['dvg_digid_autologout_use_watchdog'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_digid_autologout_inactivity_message';
  $strongarm->value = 'U bent uitgelogd wegens inactiviteit.';
  $export['dvg_digid_autologout_inactivity_message'] = $strongarm;

  return $export;
}
