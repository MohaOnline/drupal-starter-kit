<?php
/**
 * @file
 * dvg_ct_profile_group.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_profile_group_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create profile_group content'.
  $permissions['create profile_group content'] = array(
    'name' => 'create profile_group content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any profile_group content'.
  $permissions['delete any profile_group content'] = array(
    'name' => 'delete any profile_group content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own profile_group content'.
  $permissions['delete own profile_group content'] = array(
    'name' => 'delete own profile_group content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any profile_group content'.
  $permissions['edit any profile_group content'] = array(
    'name' => 'edit any profile_group content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own profile_group content'.
  $permissions['edit own profile_group content'] = array(
    'name' => 'edit own profile_group content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
