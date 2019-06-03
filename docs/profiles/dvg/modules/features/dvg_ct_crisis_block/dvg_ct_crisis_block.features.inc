<?php
/**
 * @file
 * dvg_ct_crisis_block.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_ct_crisis_block_ctools_plugin_api($module = NULL, $api = NULL) {
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
function dvg_ct_crisis_block_node_info() {
  $items = array(
    'crisis_block' => array(
      'name' => t('Crisis block'),
      'base' => 'node_content',
      'description' => t('Use <em>Crisis block</em> to display a block on the front page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
