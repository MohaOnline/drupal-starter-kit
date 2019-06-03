<?php
/**
 * @file
 * dvg_ct_menu_page.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_menu_page_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'create menu_page content'.
  $permissions['create menu_page content'] = array(
    'name' => 'create menu_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any menu_page content'.
  $permissions['delete any menu_page content'] = array(
    'name' => 'delete any menu_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own menu_page content'.
  $permissions['delete own menu_page content'] = array(
    'name' => 'delete own menu_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'dvg administer menu-about'.
  $permissions['dvg administer menu-about'] = array(
    'name' => 'dvg administer menu-about',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_ct_menu_page',
  );

  // Exported permission: 'dvg administer menu-organization'.
  $permissions['dvg administer menu-organization'] = array(
    'name' => 'dvg administer menu-organization',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_ct_menu_page',
  );

  // Exported permission: 'dvg administer menu-tasks'.
  $permissions['dvg administer menu-tasks'] = array(
    'name' => 'dvg administer menu-tasks',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_ct_menu_page',
  );

  // Exported permission: 'edit any menu_page content'.
  $permissions['edit any menu_page content'] = array(
    'name' => 'edit any menu_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own menu_page content'.
  $permissions['edit own menu_page content'] = array(
    'name' => 'edit own menu_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
