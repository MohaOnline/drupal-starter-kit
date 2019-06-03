<?php
/**
 * @file
 * dvg_ct_guide.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_guide_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create guide content'.
  $permissions['create guide content'] = array(
    'name' => 'create guide content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any guide content'.
  $permissions['delete any guide content'] = array(
    'name' => 'delete any guide content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own guide content'.
  $permissions['delete own guide content'] = array(
    'name' => 'delete own guide content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any guide content'.
  $permissions['edit any guide content'] = array(
    'name' => 'edit any guide content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own guide content'.
  $permissions['edit own guide content'] = array(
    'name' => 'edit own guide content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
