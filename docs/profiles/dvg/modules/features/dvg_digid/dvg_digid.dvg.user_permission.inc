<?php
/**
 * @file
 * dvg_digid.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_digid_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'assign digid role'.
  $permissions['assign digid role'] = array(
    'name' => 'assign digid role',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'role_delegation',
  );

  // Exported permission: 'edit webform digid settings'.
  $permissions['edit webform digid settings'] = array(
    'name' => 'edit webform digid settings',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_digid',
  );

  return $permissions;
}
