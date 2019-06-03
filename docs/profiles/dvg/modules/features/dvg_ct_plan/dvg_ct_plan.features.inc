<?php
/**
 * @file
 * dvg_ct_plan.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_ct_plan_ctools_plugin_api($module = NULL, $api = NULL) {
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
function dvg_ct_plan_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function dvg_ct_plan_node_info() {
  $items = array(
    'plan' => array(
      'name' => t('Plan'),
      'base' => 'node_content',
      'description' => t('Use <em>Plan</em> for your static content, such as a \'Spatial plan\' page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
