<?php
/**
 * @file
 * wetkit_test.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wetkit_test_ctools_plugin_api($module = NULL, $api = NULL) {
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
function wetkit_test_node_info() {
  $items = array(
    'wetkit_test_page' => array(
      'name' => t('Test Page'),
      'base' => 'node_content',
      'description' => t('Used for the automated tests.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
