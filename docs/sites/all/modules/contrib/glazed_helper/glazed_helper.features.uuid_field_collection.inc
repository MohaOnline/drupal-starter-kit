<?php
/**
 * @file
 * glazed_helper.features.uuid_field_collection.inc
 */

/**
 * Implements hook_uuid_features_default_field_collections().
 */
function glazed_helper_uuid_features_default_field_collections() {
  $field_collections = array();

  $field_collections[] = array(
    'field_name' => 'field_glazed_content_design',
    'default_revision' => 1,
    'archived' => 0,
    'uuid' => '6df570f0-153b-40b2-b4fb-2e55e5197eca',
    'field_glazed_content_region' => array(),
    'field_glazed_header_image' => array(),
    'field_glazed_hide_regions' => array(
      'und' => array(
        0 => array(
          'value' => 'sidebar_first',
        ),
        1 => array(
          'value' => 'sidebar_second',
        ),
      ),
    ),
    'rdf_mapping' => array(),
    'url_alias' => array(),
  );
  return $field_collections;
}
