<?php

/**
 * @file
 * campaignion_action.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function campaignion_action_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'action_closed_text'.
  $field_bases['action_closed_text'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'action_closed_text',
    'global_block_settings' => 1,
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
