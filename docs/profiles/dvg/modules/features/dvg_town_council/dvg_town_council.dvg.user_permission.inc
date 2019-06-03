<?php
/**
 * @file
 * dvg_town_council.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_town_council_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'dvg administer menu-town-council'.
  $permissions['dvg administer menu-town-council'] = array(
    'name' => 'dvg administer menu-town-council',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_ct_menu_page',
  );

  return $permissions;
}
