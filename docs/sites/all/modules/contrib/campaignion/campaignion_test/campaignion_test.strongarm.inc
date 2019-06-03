<?php
/**
 * @file
 * campaignion_test.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function campaignion_test_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'webform_node_email_to_target';
  $strongarm->value = 1;
  $export['webform_node_email_to_target'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'webform_node_petition';
  $strongarm->value = 1;
  $export['webform_node_petition'] = $strongarm;

  return $export;
}
