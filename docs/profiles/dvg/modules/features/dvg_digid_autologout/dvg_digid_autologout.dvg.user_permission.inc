<?php
/**
 * @file
 * dvg_digid_autologout.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_digid_autologout_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'assign digid role'.
  $permissions['administer dvg_digid_autologout'] = array(
    'name' => 'administer dvg_digid_autologout',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'dvg_digid_autologout',
  );

  return $permissions;
}
