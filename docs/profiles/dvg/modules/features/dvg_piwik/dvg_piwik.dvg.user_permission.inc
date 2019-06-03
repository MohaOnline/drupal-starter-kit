<?php
/**
 * @file
 * dvg_piwik.dvg.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function dvg_piwik_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer piwik'.
  $permissions['administer piwik'] = array(
    'name' => 'administer piwik',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'piwik',
  );

  // Exported permission: 'opt-in or out of tracking'.
  $permissions['opt-in or out of tracking'] = array(
    'name' => 'opt-in or out of tracking',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'piwik',
  );

  // Exported permission: 'use PHP for tracking visibility'.
  $permissions['use PHP for tracking visibility'] = array(
    'name' => 'use PHP for tracking visibility',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'piwik',
  );

  return $permissions;
}
