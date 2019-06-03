<?php
/**
 * @file
 * pushtape_page.features.inc
 */

/**
 * Implements hook_node_info().
 */
function pushtape_page_node_info() {
  $items = array(
    'page' => array(
      'name' => t('Page'),
      'base' => 'node_content',
      'description' => t('A simple static page, i.e. an "About Us" page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
