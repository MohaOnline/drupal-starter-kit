<?php
/**
 * @file
 * shared_content_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function shared_content_example_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function shared_content_example_node_info() {
  $items = array(
    'complex_content' => array(
      'name' => t('Complex content'),
      'base' => 'node_content',
      'description' => t('A complex content type with taxonomy and image fields, and with Workbench Moderation enabled.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'simple_content' => array(
      'name' => t('Simple content'),
      'base' => 'node_content',
      'description' => t('A very simple content type, with just a body field.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
