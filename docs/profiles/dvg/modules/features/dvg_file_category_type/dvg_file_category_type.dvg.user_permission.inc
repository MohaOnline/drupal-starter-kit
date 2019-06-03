<?php
/**
 * @file
 * dvg_file_category_type.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_file_category_type_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'delete any file_category_item files'.
  $permissions['delete any file_category_item files'] = array(
    'name' => 'delete any file_category_item files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete own file_category_item files'.
  $permissions['delete own file_category_item files'] = array(
    'name' => 'delete own file_category_item files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download any file_category_item files'.
  $permissions['download any file_category_item files'] = array(
    'name' => 'download any file_category_item files',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'editor' => 'editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download own file_category_item files'.
  $permissions['download own file_category_item files'] = array(
    'name' => 'download own file_category_item files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit any file_category_item files'.
  $permissions['edit any file_category_item files'] = array(
    'name' => 'edit any file_category_item files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit own file_category_item files'.
  $permissions['edit own file_category_item files'] = array(
    'name' => 'edit own file_category_item files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  return $permissions;
}
