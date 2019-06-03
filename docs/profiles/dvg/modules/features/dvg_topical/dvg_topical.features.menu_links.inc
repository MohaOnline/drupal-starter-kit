<?php
/**
 * @file
 * dvg_topical.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function dvg_topical_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: management_topical:admin/topical
  $menu_links['management_topical:admin/topical'] = array(
    'menu_name' => 'management',
    'link_path' => 'admin/topical',
    'router_path' => 'admin/topical',
    'link_title' => 'Topical',
    'options' => array(
      'identifier' => 'management_topical:admin/topical',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
    'parent_identifier' => 'management_administration:admin',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Topical');

  return $menu_links;
}
