<?php
/**
 * @file
 * dvg_ct_block.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_block_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create block content'.
  $permissions['create block content'] = array(
    'name' => 'create block content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any block content'.
  $permissions['delete any block content'] = array(
    'name' => 'delete any block content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own block content'.
  $permissions['delete own block content'] = array(
    'name' => 'delete own block content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any block content'.
  $permissions['edit any block content'] = array(
    'name' => 'edit any block content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own block content'.
  $permissions['edit own block content'] = array(
    'name' => 'edit own block content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
