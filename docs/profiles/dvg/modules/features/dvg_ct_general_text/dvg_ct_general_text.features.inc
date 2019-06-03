<?php
/**
 * @file
 * dvg_ct_general_text.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_ct_general_text_ctools_plugin_api($module = NULL, $api = NULL) {
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
function dvg_ct_general_text_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function dvg_ct_general_text_node_info() {
  $items = array(
    'general_text' => array(
      'name' => t('General text'),
      'base' => 'node_content',
      'description' => t('Use <em>General text</em> for your reusable texts.'),
      'has_title' => '1',
      'title_label' => t('Administrative title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
