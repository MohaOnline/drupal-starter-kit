<?php
/**
 * @file
 * dvg_webform_component_collections.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_webform_component_collections_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function dvg_webform_component_collections_node_info() {
  $items = array(
    'webform_collection' => array(
      'name' => t('Webform collection'),
      'base' => 'node_content',
      'description' => t('Collection of pre-defined webform fields and conditonals.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
