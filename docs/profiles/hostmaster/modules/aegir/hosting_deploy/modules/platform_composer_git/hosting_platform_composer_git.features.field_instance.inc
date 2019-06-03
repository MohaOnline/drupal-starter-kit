<?php
/**
 * @file
 * hosting_platform_composer_git.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function hosting_platform_composer_git_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-platform-field_composer_git_docroot'.
  $field_instances['node-platform-field_composer_git_docroot'] = array(
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
    'field_name' => 'field_composer_git_docroot',
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

  // Exported field_instance: 'node-platform-field_composer_git_project_url'.
  $field_instances['node-platform-field_composer_git_project_url'] = array(
    'bundle' => 'platform',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The Git repository URL of the project to be installed.',
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
    'field_name' => 'field_composer_git_project_url',
    'label' => 'Git URL',
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

  // Exported field_instance: 'node-platform-field_composer_git_path'.
  $field_instances['node-platform-field_composer_git_path'] = array(
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
    'field_name' => 'field_composer_git_path',
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

  // Exported field_instance: 'node-platform-field_composer_git_version'.
  $field_instances['node-platform-field_composer_git_version'] = array(
    'bundle' => 'platform',
    'default_value' => 'master',
    'deleted' => 0,
    'description' => 'Optionally specify a Git branch or tag. Defaults to the `master` branch.',
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
    'field_name' => 'field_composer_git_version',
    'label' => 'Branch/tag',
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
  t('Git URL');
  t('The Git repository URL of the project to be installed.');
  t('Path');
  t('Optionally specify the version of the package. Defaults to the latest');
  t('Branch/tag');

  return $field_instances;
}
