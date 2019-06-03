<?php
/**
 * @file
 * dvg_voc_image_license.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_voc_image_license_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'add terms in image_license'.
  $permissions['add terms in image_license'] = array(
    'name' => 'add terms in image_license',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'delete terms in image_license'.
  $permissions['delete terms in image_license'] = array(
    'name' => 'delete terms in image_license',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in image_license'.
  $permissions['edit terms in image_license'] = array(
    'name' => 'edit terms in image_license',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'view term page in image_license'.
  $permissions['view term page in image_license'] = array(
    'name' => 'view term page in image_license',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  return $permissions;
}
