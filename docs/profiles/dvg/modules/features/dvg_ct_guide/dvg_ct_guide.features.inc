<?php
/**
 * @file
 * dvg_ct_guide.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_ct_guide_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function dvg_ct_guide_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function dvg_ct_guide_node_info() {
  $items = array(
    'guide' => array(
      'name' => t('Guide'),
      'base' => 'node_content',
      'description' => t('Use <em>Guide</em> for a collection of pages with a content summary at the top of the page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
