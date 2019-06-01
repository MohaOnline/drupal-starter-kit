<?php
/**
 * @file
 * wetkit_pages.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wetkit_pages_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "panelizer" && $api == "panelizer") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function wetkit_pages_node_info() {
  $items = array(
    'wetkit_page' => array(
      'name' => t('Basic Page'),
      'base' => 'node_content',
      'description' => t('An basic page to display content to users on your site.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
