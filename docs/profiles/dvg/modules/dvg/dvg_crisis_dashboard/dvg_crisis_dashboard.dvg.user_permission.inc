<?php
/**
 * @file
 * dvg_crisis_dashboard.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_crisis_dashboard_dvg_default_permissions() {
  $permissions = array();
  
  // Exported permission: 'edit webform digid settings'.
  $permissions['access dvg crisis dashboard'] = array(
    'name' => 'access dvg crisis dashboard',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_crisis_dashboard',
  );

  return $permissions;
}
