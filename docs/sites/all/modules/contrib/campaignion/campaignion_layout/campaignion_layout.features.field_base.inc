<?php

/**
 * @file
 * campaignion_layout.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function campaignion_layout_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'layout'.
  $field_bases['layout'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'layout',
    'global_block_settings' => 1,
    'indexes' => array(),
    'locked' => 0,
    'module' => 'campaignion_layout',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'layout_selection',
  );

  // Exported field_base: 'layout_background_image'.
  $field_bases['layout_background_image'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'layout_background_image',
    'global_block_settings' => 1,
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'image',
    'settings' => array(
      'default_image' => 0,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'image',
  );

  // Exported field_base: 'layout_headline'.
  $field_bases['layout_headline'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'layout_headline',
    'global_block_settings' => 2,
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 150,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  return $field_bases;
}
