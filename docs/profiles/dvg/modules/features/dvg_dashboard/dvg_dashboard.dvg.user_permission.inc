<?php
/**
 * @file
 * dvg_dashboard.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_dashboard_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'access dashboard'.
  $permissions['access dashboard'] = array(
    'name' => 'access dashboard',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dashboard',
  );

  return $permissions;
}
