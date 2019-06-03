<?php
/**
 * @file
 * dvg_voc_owms_audience.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_voc_owms_audience_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_description__owms_audience';
  $strongarm->value = TRUE;
  $export['dvg_vocabulary__hide_description__owms_audience'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_relations__owms_audience';
  $strongarm->value = TRUE;
  $export['dvg_vocabulary__hide_relations__owms_audience'] = $strongarm;

  return $export;
}
