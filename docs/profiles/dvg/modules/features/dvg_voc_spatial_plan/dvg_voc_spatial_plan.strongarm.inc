<?php
/**
 * @file
 * dvg_voc_spatial_plan.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_voc_spatial_plan_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_description__spatial_plan';
  $strongarm->value = TRUE;
  $export['dvg_vocabulary__hide_description__spatial_plan'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_relations__spatial_plan';
  $strongarm->value = TRUE;
  $export['dvg_vocabulary__hide_relations__spatial_plan'] = $strongarm;

  return $export;
}
