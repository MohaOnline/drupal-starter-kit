<?php
/**
 * @file
 * dvg_file_category_type.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function dvg_file_category_type_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_file_category'.
  $field_bases['field_file_category'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_file_category',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'file_category',
          'parent' => 0,
        ),
      ),
      'options_list_callback' => NULL,
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  return $field_bases;
}
