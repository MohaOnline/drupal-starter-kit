<?php
/**
 * @file
 * dvg_ct_external_link.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_external_link_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create external_link content'.
  $permissions['create external_link content'] = array(
    'name' => 'create external_link content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any external_link content'.
  $permissions['delete any external_link content'] = array(
    'name' => 'delete any external_link content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own external_link content'.
  $permissions['delete own external_link content'] = array(
    'name' => 'delete own external_link content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any external_link content'.
  $permissions['edit any external_link content'] = array(
    'name' => 'edit any external_link content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own external_link content'.
  $permissions['edit own external_link content'] = array(
    'name' => 'edit own external_link content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
