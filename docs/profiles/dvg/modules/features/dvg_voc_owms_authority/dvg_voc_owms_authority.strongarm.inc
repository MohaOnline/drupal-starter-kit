<?php
/**
 * @file
 * dvg_voc_owms_authority.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_voc_owms_authority_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_description__owms_authority';
  $strongarm->value = TRUE;
  $export['dvg_vocabulary__hide_description__owms_authority'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_relations__owms_authority';
  $strongarm->value = TRUE;
  $export['dvg_vocabulary__hide_relations__owms_authority'] = $strongarm;

  return $export;
}
