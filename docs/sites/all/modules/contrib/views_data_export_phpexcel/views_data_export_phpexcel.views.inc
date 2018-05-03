<?php
/**
 * @file
 * Views include file with views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function views_data_export_phpexcel_views_plugins() {
  $path = drupal_get_path('module', 'views_data_export_phpexcel');

  return array(
    'style' => array(
      'views_data_export_xlsx' => array(
        'title' => t('XLSX file'),
        'help' => t('Display the view as an xlsx file.'),
        'handler' => 'views_data_export_phpexcel_plugin_style_export',
        'export headers' => array('Content-Type' => 'application/vnd.ms-excel'),
        'export feed type' => 'xlsx',
        'export feed text' => 'XLSX',
        'export feed file' => '%view.xlsx',
        'export feed icon' => drupal_get_path('module', 'views_data_export_phpexcel') . '/images/xlsx.png',
        'additional themes' => array(
          'views_data_export_xlsx_body' => 'style',
        ),
        'additional themes base' => 'views_data_export_xlsx',
        'path' => $path . '/plugins',
        'parent' => 'views_data_export_xls',
        'theme' => 'views_data_export_xlsx',
        'theme file' => 'views_data_export_phpexcel.theme.inc',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'data_export',
      ),
    ),
  );
}
