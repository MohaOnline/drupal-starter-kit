<?php
/**
 * @file
 * dvg_ct_menu_page.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function dvg_ct_menu_page_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-about.
  $menus['menu-about'] = array(
    'menu_name' => 'menu-about',
    'title' => 'About',
    'description' => 'The <em>About</em> menu contains links to general pages.',
  );
  // Exported menu: menu-organization.
  $menus['menu-organization'] = array(
    'menu_name' => 'menu-organization',
    'title' => 'Organization',
    'description' => 'The <em>Organization</em> menu contains links to pages related to the organization and administration.',
  );
  // Exported menu: menu-tasks.
  $menus['menu-tasks'] = array(
    'menu_name' => 'menu-tasks',
    'title' => 'Tasks',
    'description' => 'The <em>Tasks</em> menu contains links for Tasks and Guides.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('About');
  t('The <em>About</em> menu contains links to general pages.');
  t('Organization');
  t('The <em>Organization</em> menu contains links to pages related to the organization and administration.');
  t('Tasks');
  t('The <em>Tasks</em> menu contains links for Tasks and Guides.');
  return $menus;
}
