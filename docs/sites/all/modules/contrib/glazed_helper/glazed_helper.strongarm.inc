<?php
/**
 * @file
 * glazed_helper.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function glazed_helper_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'uuid_features_entity_field_collection_item_field_glazed_content_design';
  $strongarm->value = 'field_glazed_content_design';
  $export['uuid_features_entity_field_collection_item_field_glazed_content_design'] = $strongarm;

  return $export;
}
