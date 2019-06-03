<?php
/**
 * @file
 * dvg_voc_town_council.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_voc_town_council_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_description__town_council';
  $strongarm->value = FALSE;
  $export['dvg_vocabulary__hide_description__town_council'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_relations__town_council';
  $strongarm->value = TRUE;
  $export['dvg_vocabulary__hide_relations__town_council'] = $strongarm;

  return $export;
}
