<?php
/**
 * @file
 * dvg_dashboard.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function dvg_dashboard_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-dashboard-menu.
  $menus['menu-dashboard-menu'] = array(
    'menu_name' => 'menu-dashboard-menu',
    'title' => 'Dashboard menu',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Dashboard menu');
  t('');
  return $menus;
}
