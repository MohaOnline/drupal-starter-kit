<?php
/**
 * @file
 * dvg_ct_live_blog.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_ct_live_blog_ctools_plugin_api($module = NULL, $api = NULL) {
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
 * Implements hook_views_api().
 */
function dvg_ct_live_blog_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function dvg_ct_live_blog_node_info() {
  $items = array(
    'live_blog' => array(
      'name' => t('Live blog'),
      'base' => 'node_content',
      'description' => t('Use <em>Live blog</em> to provide a rolling textual coverage of an ongoing event.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
