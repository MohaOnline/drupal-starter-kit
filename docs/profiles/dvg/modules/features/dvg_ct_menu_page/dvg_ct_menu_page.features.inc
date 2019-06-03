<?php
/**
 * @file
 * dvg_ct_menu_page.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_ct_menu_page_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function dvg_ct_menu_page_node_info() {
  $items = array(
    'menu_page' => array(
      'name' => t('Menu page'),
      'base' => 'node_content',
      'description' => t('Use <em>Menu page</em> for a page with menu items.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
