<?php
/**
 * @file
 * dvg_stuf_bg.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_stuf_bg_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer piwik'.
  $permissions['administer dvg_stuf_bg'] = array(
    'name' => 'administer dvg_stuf_bg',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'dvg_stuf_bg',
  );

  return $permissions;
}
