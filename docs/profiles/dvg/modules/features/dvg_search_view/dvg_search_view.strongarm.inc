<?php
/**
 * @file
 * dvg_search_view.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_search_view_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'functional_content_config__search__block';
  $strongarm->value = 1;
  $export['functional_content_config__search__block'] = $strongarm;

  return $export;
}
