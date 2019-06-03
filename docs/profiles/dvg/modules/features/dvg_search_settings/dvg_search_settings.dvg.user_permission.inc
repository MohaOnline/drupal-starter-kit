<?php
/**
 * @file
 * dvg_search_settings.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_search_settings_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'change noindex value'.
  $permissions['change noindex value'] = array(
    'name' => 'change noindex value',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'search_api_noindex',
  );

  return $permissions;
}
