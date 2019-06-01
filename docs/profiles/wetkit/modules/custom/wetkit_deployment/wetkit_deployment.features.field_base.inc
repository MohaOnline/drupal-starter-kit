<?php
/**
 * @file
 * wetkit_deployment.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function wetkit_deployment_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_deploy_to_destination_site'.
  $field_bases['field_deploy_to_destination_site'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_deploy_to_destination_site',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => '',
        1 => '',
      ),
      'allowed_values_function' => '',
      'entity_translation_sync' => FALSE,
    ),
    'shared_content_plan' => 'destination_site_plan',
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_shared_content'.
  $field_bases['field_shared_content'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_shared_content',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => '',
        1 => '',
      ),
      'allowed_values_function' => '',
      'entity_translation_sync' => FALSE,
    ),
    'shared_content_plan' => '',
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  return $field_bases;
}
