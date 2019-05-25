<?php

/**
 * @file
 * Helper functions provided by the Custom Menu Toolbar module.
 *
 * Created by: Topsitemakers
 * http://www.topsitemakers.com/
 */

/**
 * Helper function - get toolbar menu for current user.
 */
function _custom_menu_toolbar_get_menu_for_current_user() {
  // Sanity check - stop if the user does not have sufficient permissions.
  if (!user_access(CUSTOM_MENU_TOOLBAR_PERMISSION_ACCESS_TOOLBAR)) return FALSE;

  global $user;
  $user_roles = array_keys($user->roles);

  $menu = db_select('custom_menu_toolbar_role_menus', 'm')
    ->fields('m')
    ->condition('rid', $user_roles, 'IN')
    ->orderBy('weight', 'DESC')
    ->range(0, 1)
    ->execute()
    ->fetchObject();

  if ($menu && $menu->menu_name != '_none') {
    return $menu;
  }

  return FALSE;
}
