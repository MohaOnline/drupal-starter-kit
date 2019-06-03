<?php
/**
 * @file
 * dvg_voc_file_category.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_voc_file_category_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'add terms in file_category'.
  $permissions['add terms in file_category'] = array(
    'name' => 'add terms in file_category',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'delete terms in file_category'.
  $permissions['delete terms in file_category'] = array(
    'name' => 'delete terms in file_category',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in file_category'.
  $permissions['edit terms in file_category'] = array(
    'name' => 'edit terms in file_category',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'view term page in file_category'.
  $permissions['view term page in file_category'] = array(
    'name' => 'view term page in file_category',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  return $permissions;
}
