<?php
/**
 * @file
 * dvg_ct_webform.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_webform_component_collections_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create webform collection content'.
  $permissions['create webform_collection content'] = array(
    'name' => 'create webform_collection content',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own webform collection content'.
  $permissions['edit own webform_collection content'] = array(
    'name' => 'edit own webform_collection content',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any webform collection content'.
  $permissions['edit any webform_collection content'] = array(
    'name' => 'edit any webform_collection content',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any webform collection content'.
  $permissions['delete any webform_collection content'] = array(
    'name' => 'delete any webform_collection content',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own webform collection content'.
  $permissions['delete own webform_collection content'] = array(
    'name' => 'delete own webform_collection content',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
