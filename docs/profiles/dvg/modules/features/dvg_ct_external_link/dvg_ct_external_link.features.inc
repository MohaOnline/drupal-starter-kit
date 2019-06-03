<?php
/**
 * @file
 * dvg_ct_external_link.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_ct_external_link_ctools_plugin_api($module = NULL, $api = NULL) {
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
function dvg_ct_external_link_node_info() {
  $items = array(
    'external_link' => array(
      'name' => t('External link'),
      'base' => 'node_content',
      'description' => t('Use <em>External link</em> for linking to external content.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
