<?php

/**
 * @file
 * Baidu Map integration with Views to implement a Display Style Plugin.
 */

/**
 * Implements hook_views_plugins().
 */
function baidu_map_geofield_views_plugins() {
  $plugins = array(
    'module' => 'baidu_map_geofield',
    'style' => array(
      'baidu_map_geofield_map' => array(
        'title' => t('Baidu Map Geofield'),
        'help' => t('Displays a View as a Geofield map through Baidu Map JS API.'),
        'handler' => 'baidu_map_geofield_plugin_style_map',
        'theme' => 'baidu_map_geofield_map',
        'theme path' => drupal_get_path('module', 'baidu_map_geofield') . '/includes',
        'path' => drupal_get_path('module', 'baidu_map_geofield') . '/includes',
        'uses fields' => TRUE,
        'uses row plugin' => FALSE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
  );

  return $plugins;
}
