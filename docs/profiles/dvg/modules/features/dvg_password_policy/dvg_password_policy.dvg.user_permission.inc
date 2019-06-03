<?php
/**
 * @file
 * dvg_password_policy.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_password_policy_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer password policy'.
  $permissions['administer password policy'] = array(
    'name' => 'administer password policy',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'password_policy',
  );

  return $permissions;
}
