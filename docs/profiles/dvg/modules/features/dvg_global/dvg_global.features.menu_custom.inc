<?php
/**
 * @file
 * dvg_global.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function dvg_global_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-footer-menu.
  $menus['menu-footer-menu'] = array(
    'menu_name' => 'menu-footer-menu',
    'title' => 'Footer Menu',
    'description' => '',
  );
  // Exported menu: menu-social-media-menu.
  $menus['menu-social-media-menu'] = array(
    'menu_name' => 'menu-social-media-menu',
    'title' => 'Social Media',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Footer Menu');
  t('');
  t('Social Media');
  t('');

  return $menus;
}
