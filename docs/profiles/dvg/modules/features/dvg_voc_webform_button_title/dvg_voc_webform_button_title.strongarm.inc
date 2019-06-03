<?php
/**
 * @file
 * dvg_voc_webform_button_title.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_voc_webform_button_title_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_description__webform_button_title';
  $strongarm->value = TRUE;
  $export['dvg_vocabulary__hide_description__webform_button_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'dvg_vocabulary__hide_relations__webform_button_title';
  $strongarm->value = TRUE;
  $export['dvg_vocabulary__hide_relations__webform_button_title'] = $strongarm;

  return $export;
}
