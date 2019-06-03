<?php
/**
 * @file
 * dvg_town_council.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_town_council_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'functional_content_config__town_council__block';
  $strongarm->value = 1;
  $export['functional_content_config__town_council__block'] = $strongarm;

  return $export;
}
