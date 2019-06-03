<?php

/**
 * @file
 * theme functions for morris.js (JSON)
 * @see http://morrisjs.github.io/morris.js
 */

define('OPIGNO_LRS_STATS_DASHBOARD_CONTAINER', 'lrs-stats-widget-dashboard-page-view-chart');

function theme_opigno_lrs_stats_dashboard_widget_total_number_of_page_view_current_week_aggregate_day_json($variables) {
  $palette = color_get_palette('platon');
  drupal_json_output(array(
    'element' => OPIGNO_LRS_STATS_DASHBOARD_CONTAINER,
    'data' => $variables['data'],
    'resize' => TRUE,
    'xLabels' => array('day'),
    'xkey' => 'day',
    'ykeys' => array('value'),
    'labels' => array(t('Number of view per day')),
    'lineColors' => array($palette['dark_blue'])
  ));
}

function theme_opigno_lrs_stats_dashboard_widget_total_number_of_page_view_current_month_aggregate_day_json($variables) {
  $palette = color_get_palette('platon');
  drupal_json_output(array(
    'element' => OPIGNO_LRS_STATS_DASHBOARD_CONTAINER,
    'data' => $variables['data'],
    'resize' => TRUE,
    'xLabels' => array('day'),
    'xkey' => 'day',
    'ykeys' => array('value'),
    'labels' => array(t('Number of view per day')),
    'lineColors' => array($palette['dark_blue'])
  ));
}

function theme_opigno_lrs_stats_dashboard_widget_total_number_of_page_view_current_year_aggregate_day_json($variables) {
  $palette = color_get_palette('platon');
  drupal_json_output(array(
    'element' => OPIGNO_LRS_STATS_DASHBOARD_CONTAINER,
    'data' => $variables['data'],
    'resize' => TRUE,
    'xLabels' => array('day'),
    'xkey' => 'day',
    'ykeys' => array('value'),
    'labels' => array(t('Number of view per day')),
    'lineColors' => array($palette['dark_blue'])
  ));
}

function theme_opigno_lrs_stats_dashboard_widget_total_number_of_page_view_current_year_aggregate_month_json($variables) {
  $palette = color_get_palette('platon');
  drupal_json_output(array(
    'element' => OPIGNO_LRS_STATS_DASHBOARD_CONTAINER,
    'data' => $variables['data'],
    'resize' => TRUE,
    'xLabels' => array('month'),
    'xkey' => 'month',
    'ykeys' => array('value'),
    'labels' => array(t('Number of view per month')),
    'lineColors' => array($palette['dark_blue'])
  ));
}

function theme_opigno_lrs_stats_dashboard_widget_total_number_of_page_view_forever_aggregate_day_json($variables) {
  $palette = color_get_palette('platon');
  drupal_json_output(array(
    'element' => OPIGNO_LRS_STATS_DASHBOARD_CONTAINER,
    'data' => $variables['data'],
    'resize' => TRUE,
    'xLabels' => array('day'),
    'xkey' => 'day',
    'ykeys' => array('value'),
    'labels' => array(t('Number of view per day')),
    'lineColors' => array($palette['dark_blue'])
  ));
}

function theme_opigno_lrs_stats_dashboard_widget_total_number_of_page_view_forever_aggregate_month_json($variables) {
  $palette = color_get_palette('platon');
  drupal_json_output(array(
    'element' => OPIGNO_LRS_STATS_DASHBOARD_CONTAINER,
    'data' => $variables['data'],
    'resize' => TRUE,
    'xLabels' => array('month'),
    'xkey' => 'month',
    'ykeys' => array('value'),
    'labels' => array(t('Number of view per month')),
    'lineColors' => array($palette['dark_blue'])
  ));
}

function theme_opigno_lrs_stats_dashboard_widget_total_number_of_page_view_forever_aggregate_year_json($variables) {
  $palette = color_get_palette('platon');
  drupal_json_output(array(
    'element' => OPIGNO_LRS_STATS_DASHBOARD_CONTAINER,
    'data' => $variables['data'],
    'resize' => TRUE,
    'xLabels' => array('year'),
    'xkey' => 'year',
    'ykeys' => array('value'),
    'labels' => array(t('Number of view per year')),
    'lineColors' => array($palette['dark_blue'])
  ));
}

