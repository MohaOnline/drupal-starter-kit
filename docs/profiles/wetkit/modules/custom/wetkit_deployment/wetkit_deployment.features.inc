<?php
/**
 * @file
 * wetkit_deployment.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wetkit_deployment_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function wetkit_deployment_node_info() {
  $items = array(
    'deploy_content_moderated' => array(
      'name' => t('Deploy (moderated)'),
      'base' => 'node_content',
      'description' => t('Deploy a workbench moderated content type with additional fields.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'deploy_content_simple' => array(
      'name' => t('Deploy (simple)'),
      'base' => 'node_content',
      'description' => t('Deploy a very simple content type, with just a body field.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
