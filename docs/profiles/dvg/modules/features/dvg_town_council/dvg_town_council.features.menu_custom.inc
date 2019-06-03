<?php
/**
 * @file
 * dvg_town_council.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function dvg_town_council_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-town-council.
  $menus['menu-town-council'] = array(
    'menu_name' => 'menu-town-council',
    'title' => 'Town council',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Town council');
  t('');
  return $menus;
}
