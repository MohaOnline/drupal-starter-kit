<?php
/**
 * @file
 * dvg_ct_block.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function dvg_ct_block_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_block_body'.
  $field_bases['field_block_body'] = array(
    'active' => 1,
    'cardinality' => 2,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_block_body',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  return $field_bases;
}
