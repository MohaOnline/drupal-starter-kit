<?php
/**
 * @file
 * hosting_platform_git.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function hosting_platform_git_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-platform-field_git_docroot'.
  $field_instances['node-platform-field_git_docroot'] = array(
    'bundle' => 'platform',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'If Drupal resides in a sub-directory of your Git repository, specify it here. This path should be relative to the root of the git repository.',
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
    'field_name' => 'field_git_docroot',
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

  // Exported field_instance: 'node-platform-field_git_reference'.
  $field_instances['node-platform-field_git_reference'] = array(
    'bundle' => 'platform',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A branch, tag or commit hash to checkout once the repository is cloned. Note that specifying a commit hash can take significantly longer, as the entire Git history must be fetched.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_git_reference',
    'label' => 'Git reference',
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
      'weight' => -2,
    ),
  );

  // Exported field_instance: 'node-platform-field_git_repository_path'.
  $field_instances['node-platform-field_git_repository_path'] = array(
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
    'field_name' => 'field_git_repository_path',
    'label' => 'Git repository path',
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

  // Exported field_instance: 'node-platform-field_git_repository_url'.
  $field_instances['node-platform-field_git_repository_url'] = array(
    'bundle' => 'platform',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The URL of the Git repository that will be cloned to deploy this platform. Note that this repository must be accessible by the `aegir` system user.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_git_repository_url',
    'label' => 'Git repository URL',
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
      'weight' => -3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('If Drupal resides in a sub-directory of your Git repository, specify it here. This path should be relative to the root of the git repository.');
  t('Docroot');
  t('A branch, tag or commit hash to checkout once the repository is cloned. Note that specifying a commit hash can take significantly longer, as the entire Git history must be fetched.');
  t('Git reference');
  t('The URL of the Git repository that will be cloned to deploy this platform. Note that this repository must be accessible by the `aegir` system user.');
  t('Git repository URL');
  t('Git repository path');

  return $field_instances;
}
