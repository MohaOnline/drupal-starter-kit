<?php
/**
 * @file
 * dvg_ct_live_blog.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_live_blog_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create live_blog content'.
  $permissions['create live_blog content'] = array(
    'name' => 'create live_blog content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any live_blog content'.
  $permissions['delete any live_blog content'] = array(
    'name' => 'delete any live_blog content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own live_blog content'.
  $permissions['delete own live_blog content'] = array(
    'name' => 'delete own live_blog content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any live_blog content'.
  $permissions['edit any live_blog content'] = array(
    'name' => 'edit any live_blog content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own live_blog content'.
  $permissions['edit own live_blog content'] = array(
    'name' => 'edit own live_blog content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
