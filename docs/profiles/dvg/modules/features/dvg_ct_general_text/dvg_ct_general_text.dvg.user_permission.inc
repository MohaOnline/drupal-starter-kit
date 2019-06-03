<?php
/**
 * @file
 * dvg_ct_general_text.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_general_text_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create general_text content'.
  $permissions['create general_text content'] = array(
    'name' => 'create general_text content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any general_text content'.
  $permissions['delete any general_text content'] = array(
    'name' => 'delete any general_text content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own general_text content'.
  $permissions['delete own general_text content'] = array(
    'name' => 'delete own general_text content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any general_text content'.
  $permissions['edit any general_text content'] = array(
    'name' => 'edit any general_text content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own general_text content'.
  $permissions['edit own general_text content'] = array(
    'name' => 'edit own general_text content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
