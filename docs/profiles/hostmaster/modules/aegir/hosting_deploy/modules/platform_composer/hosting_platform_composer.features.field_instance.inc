<?php
/**
 * @file
 * hosting_platform_composer.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function hosting_platform_composer_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-platform-field_composer_project_docroot'.
  $field_instances['node-platform-field_composer_project_docroot'] = array(
    'bundle' => 'platform',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'For Composer projects, Drupal resides in a sub-directory. Specify it here. Common values include: `docroot`, `html` and `web`.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_composer_project_docroot',
    'label' => 'Docroot',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => -1,
    ),
  );

  // Exported field_instance: 'node-platform-field_composer_project_package'.
  $field_instances['node-platform-field_composer_project_package'] = array(
    'bundle' => 'platform',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The package name to be installed.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_composer_project_package',
    'label' => 'Repository name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'node-platform-field_composer_project_path'.
  $field_instances['node-platform-field_composer_project_path'] = array(
    'bundle' => 'platform',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_composer_project_path',
    'label' => 'Path',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'node-platform-field_composer_project_version'.
  $field_instances['node-platform-field_composer_project_version'] = array(
    'bundle' => 'platform',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Optionally specify the version of the package. Defaults to the latest',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_composer_project_version',
    'label' => 'Version',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 0,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('For Composer projects, Drupal resides in a sub-directory. Specify it here. Common values include: `docroot`, `html` and `web`.');
  t('Docroot');
  t('Repository name');
  t('Path');
  t('The package name to be installed.');
  t('Optionally specify the version of the package. Defaults to the latest');
  t('Version');

  return $field_instances;
}
