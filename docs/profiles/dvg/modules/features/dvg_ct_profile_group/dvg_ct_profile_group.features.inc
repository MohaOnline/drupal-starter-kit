<?php
/**
 * @file
 * dvg_ct_profile_group.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_ct_profile_group_ctools_plugin_api($module = NULL, $api = NULL) {
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
function dvg_ct_profile_group_node_info() {
  $items = array(
    'profile_group' => array(
      'name' => t('Profile group'),
      'base' => 'node_content',
      'description' => t('Use <em>Profile group</em> to add a page that lists profiles.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
