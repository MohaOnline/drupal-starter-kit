<?php
/**
 * @file
 * hosting_deploy.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function hosting_deploy_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-platform-field_deployment_strategy'.
  $field_instances['node-platform-field_deployment_strategy'] = array(
    'bundle' => 'platform',
    'default_value' => array(
      0 => array(
        'value' => 'frommakefile',
      ),
    ),
    'deleted' => 0,
    'description' => 'Choose the mechanism by which to deploy this platform.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_deployment_strategy',
    'label' => 'Deployment strategy',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Choose the mechanism by which to deploy this platform.');
  t('Deployment strategy');

  return $field_instances;
}
