<?php
/**
 * @file
 * Set permissions for the access to the crisis banner.
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_crisis_safety_region_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'access dvg crisis safety region'.
  $permissions['access dvg crisis safety region'] = array(
    'name' => 'access dvg crisis safety region',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_crisis_safety_region',
  );

  return $permissions;
}
