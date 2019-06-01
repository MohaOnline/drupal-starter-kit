<?php
/**
 * @file
 * wetkit_wetboew.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function wetkit_wetboew_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: main-menu.
  $menus['main-menu'] = array(
    'menu_name' => 'main-menu',
    'title' => 'Mega Menu',
    'description' => 'The <em>Main</em> megamenu.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Exported menu: menu-wet-footer.
  $menus['menu-wet-footer'] = array(
    'menu_name' => 'menu-wet-footer',
    'title' => 'Footer',
    'description' => 'Footer Menu.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Exported menu: menu-wet-header.
  $menus['menu-wet-header'] = array(
    'menu_name' => 'menu-wet-header',
    'title' => 'Header',
    'description' => 'Header Menu.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Exported menu: menu-wet-mid-footer.
  $menus['menu-wet-mid-footer'] = array(
    'menu_name' => 'menu-wet-mid-footer',
    'title' => 'Mid Footer',
    'description' => 'Mid Footer Menu.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Exported menu: menu-wet-terms.
  $menus['menu-wet-terms'] = array(
    'menu_name' => 'menu-wet-terms',
    'title' => 'Terms',
    'description' => 'Terms Menu.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Footer');
  t('Footer Menu.');
  t('Header');
  t('Header Menu.');
  t('Mega Menu');
  t('Mid Footer');
  t('Mid Footer Menu.');
  t('Terms');
  t('Terms Menu.');
  t('The <em>Main</em> megamenu.');

  return $menus;
}
