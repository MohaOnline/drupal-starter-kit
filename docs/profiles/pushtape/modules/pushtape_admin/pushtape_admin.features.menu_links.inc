<?php
/**
 * @file
 * pushtape_admin.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function pushtape_admin_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-pushtape_-add:node/add.
  $menu_links['menu-pushtape_-add:node/add'] = array(
    'menu_name' => 'menu-pushtape',
    'link_path' => 'node/add',
    'router_path' => 'node/add',
    'link_title' => '✚ Add',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-pushtape_-add:node/add',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 0,
  );
  // Exported menu link: menu-pushtape_account:user.
  $menu_links['menu-pushtape_account:user'] = array(
    'menu_name' => 'menu-pushtape',
    'link_path' => 'user',
    'router_path' => 'user',
    'link_title' => 'Account',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-pushtape_account:user',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 0,
  );
  // Exported menu link: menu-pushtape_advanced:admin.
  $menu_links['menu-pushtape_advanced:admin'] = array(
    'menu_name' => 'menu-pushtape',
    'link_path' => 'admin',
    'router_path' => 'admin',
    'link_title' => 'Advanced',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-pushtape_advanced:admin',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 0,
  );
  // Exported menu link: menu-pushtape_dashboard:admin/pushtape.
  $menu_links['menu-pushtape_dashboard:admin/pushtape'] = array(
    'menu_name' => 'menu-pushtape',
    'link_path' => 'admin/pushtape',
    'router_path' => 'admin/pushtape',
    'link_title' => 'Dashboard',
    'options' => array(
      'attributes' => array(
        'title' => 'Dashboard',
      ),
      'identifier' => 'menu-pushtape_dashboard:admin/pushtape',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 0,
  );
  // Exported menu link: menu-pushtape_home:<front>.
  $menu_links['menu-pushtape_home:<front>'] = array(
    'menu_name' => 'menu-pushtape',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Home',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-pushtape_home:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 0,
  );
  // Exported menu link: menu-pushtape_new-track:node/add/track.
  $menu_links['menu-pushtape_new-track:node/add/track'] = array(
    'menu_name' => 'menu-pushtape',
    'link_path' => 'node/add/track',
    'router_path' => 'node/add',
    'link_title' => 'New track',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-pushtape_new-track:node/add/track',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 0,
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Account');
  t('Advanced');
  t('Dashboard');
  t('Home');
  t('New track');
  t('✚ Add');

  return $menu_links;
}
