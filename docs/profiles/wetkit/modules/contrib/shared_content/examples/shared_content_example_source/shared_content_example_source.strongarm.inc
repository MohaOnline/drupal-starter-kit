<?php
/**
 * @file
 * shared_content_example_source.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function shared_content_example_source_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workbench_moderation_default_state_complex_content';
  $strongarm->value = 'draft';
  $export['workbench_moderation_default_state_complex_content'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workbench_moderation_default_state_simple_content';
  $strongarm->value = 'draft';
  $export['workbench_moderation_default_state_simple_content'] = $strongarm;

  return $export;
}
