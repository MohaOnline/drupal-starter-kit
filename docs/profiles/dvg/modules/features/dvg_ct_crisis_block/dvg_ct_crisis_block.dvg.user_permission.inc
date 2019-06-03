<?php
/**
 * @file
 * dvg_ct_crisis_block.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_crisis_block_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create crisis_block content'.
  $permissions['create crisis_block content'] = array(
    'name' => 'create crisis_block content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any crisis_block content'.
  $permissions['delete any crisis_block content'] = array(
    'name' => 'delete any crisis_block content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own crisis_block content'.
  $permissions['delete own crisis_block content'] = array(
    'name' => 'delete own crisis_block content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any crisis_block content'.
  $permissions['edit any crisis_block content'] = array(
    'name' => 'edit any crisis_block content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own crisis_block content'.
  $permissions['edit own crisis_block content'] = array(
    'name' => 'edit own crisis_block content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
