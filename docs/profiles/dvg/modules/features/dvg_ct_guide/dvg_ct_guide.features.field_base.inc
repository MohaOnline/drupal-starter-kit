<?php
/**
 * @file
 * dvg_ct_guide.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function dvg_ct_guide_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_guide_pages'.
  $field_bases['field_guide_pages'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_guide_pages',
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'views',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'view' => array(
          'args' => array(),
          'display_name' => 'entityreference_guide',
          'view_name' => 'guide_pages',
        ),
      ),
      'target_type' => 'node',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}
