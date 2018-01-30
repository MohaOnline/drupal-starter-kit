<?php

/**
 * @file
 * Provides the Easychart Wysiwyg plugin.
 */

/**
 * Implements hook_wysiwyg_plugin().
 */
function easychart_wysiwyg_easychart_wysiwyg_plugin() {

  // add necessary jQuery ui libs
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'ui.dialog');

  // Make path to module available to plugin.js
  drupal_add_js(array('easychart' => array('module_path' => drupal_get_path('module', 'easychart_wysiwyg'))), 'setting');

  // Make all easychart configs available to plugin.js so that we can render a preview in the WYSIWYG.
  $query = db_select('node', 'n');
  $query->join('field_data_easychart', 'ec', 'ec.entity_id = n.nid');
  $query->fields('n', array('nid'));
  $query->fields('ec', array('easychart_config'));
  $query->condition('n.type', 'easychart');
  $query->condition('n.status', 1);

  $result = $query->execute();

  $existingCharts = array();
  while($record = $result->fetchAssoc()) {
    $existingCharts[$record['nid']] = $record['easychart_config'];
  }
  drupal_add_js(array('easychart' => array('existingCharts' => $existingCharts)), 'setting');

  // Make all view modes available to plugin.js
  $entity_info = entity_get_info('node');
  $view_modes = array_keys($entity_info['view modes']);
  drupal_add_js(array('easychart' => array('view_modes' => $view_modes)), 'setting');

  $plugins['easychart'] = array(
    'title' => t('Easychart'),
    'vendor url' => 'http://drupal.org/project/easychart',
    'icon path' => drupal_get_path('module', 'easychart_wysiwyg') . '/plugins/images',
    'icon file' => 'icon.png',
    'icon title' => t('Add a chart'),
    'js path' => drupal_get_path('module', 'easychart_wysiwyg') . '/plugins/js',
    'js file' => 'plugin.js',
    'css path' => drupal_get_path('module', 'easychart_wysiwyg') . '/plugins/css',
    'css file' => 'plugin.css',
    'settings' => array(),
  );
  return $plugins;
}
