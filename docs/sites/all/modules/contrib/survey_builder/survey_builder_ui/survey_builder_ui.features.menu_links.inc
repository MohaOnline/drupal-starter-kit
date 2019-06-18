<?php
/**
 * @file
 * survey_builder_ui.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function survey_builder_ui_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: navigation:survey/new/edit
  $menu_links['navigation:survey/new/edit'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'survey/new/edit',
    'router_path' => 'survey/%/edit',
    'link_title' => 'Add survey',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '5',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Add survey');


  return $menu_links;
}
