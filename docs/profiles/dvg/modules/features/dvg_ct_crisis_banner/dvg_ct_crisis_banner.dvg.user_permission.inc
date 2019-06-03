<?php
/**
 * @file
 * dvg_ct_crisis_banner.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_crisis_banner_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create crisis_banner content'.
  $permissions['create crisis_banner content'] = array(
    'name' => 'create crisis_banner content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any crisis_banner content'.
  $permissions['delete any crisis_banner content'] = array(
    'name' => 'delete any crisis_banner content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own crisis_banner content'.
  $permissions['delete own crisis_banner content'] = array(
    'name' => 'delete own crisis_banner content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any crisis_banner content'.
  $permissions['edit any crisis_banner content'] = array(
    'name' => 'edit any crisis_banner content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own crisis_banner content'.
  $permissions['edit own crisis_banner content'] = array(
    'name' => 'edit own crisis_banner content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
