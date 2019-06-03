<?php
/**
 * @file
 * dvg_payment_webform.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_payment_webform_dvg_default_permissions() {
  $permissions = array();
  
  // Exported permission: 'edit webform digid settings'.
  $permissions['edit webform payment settings'] = array(
    'name' => 'edit webform payment settings',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_payment_webform',
  );

  return $permissions;
}
