<?php
/**
 * @file
 * pushtape_admin.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function pushtape_admin_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-pushtape.
  $menus['menu-pushtape'] = array(
    'menu_name' => 'menu-pushtape',
    'title' => 'Pushtape',
    'description' => 'Menu for Pushtape tasks.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Menu for Pushtape tasks.');
  t('Pushtape');

  return $menus;
}
