<?php

/**
 * @file
 * Dashboard pages
 */

require_once(__DIR__ . '/total_number_of_page_view_form.inc');
require_once(__DIR__ . '/model.inc');
require_once(__DIR__ . '/theme.inc');

/**
 * Display general statistics page
 */
function opigno_lrs_stats_dashboard_page() {
  $module_path = drupal_get_path('module', 'opigno_tincan_api_stats');
  drupal_add_css("{$module_path}/css/morris.css", 'theme');
  drupal_add_css("{$module_path}/css/styles.css", 'theme');
  drupal_add_js("{$module_path}/js/raphael.min.js");
  drupal_add_js("{$module_path}/js/morris.min.js");

  return theme('opigno_lrs_stats_dashboard', array(
    'last_statements' => array_slice(opigno_lrs_stats_all_statements(), 0, 10),
    'top_10_viewed_pages' => opigno_lrs_stats_dashboard_top_ten_viewed_pages(),
    'most_active_users' => opigno_lrs_stats_dashboard_present_most_active_users()
  ));
}

/**
 * Display total number of page view in json (morris.js format)
 */
function opigno_lrs_stats_dashboard_total_number_of_page_view_json_page() {
  drupal_add_http_header('Content-Type', 'application/json');
  $opigno_lrs=variable_get('opigno_lrs_stats_dashboard_total_number_of_page_view_form_values');
  switch($opigno_lrs['display_period'])
  {
    case OPIGNO_LRS_STATS_CURRENT_WEEK_AGGREGATE_DAY:
      $theme = 'opigno_lrs_stats_dashboard_widget_total_number_of_page_view_current_week_aggregate_day_json';
      $data = opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_day_between(
        opigno_lrs_stats_start_of_current_week(),
        opigno_lrs_stats_end_of_current_week(),
        opigno_lrs_stats_current_week_viewed_statements()
      );
    break;
    case OPIGNO_LRS_STATS_CURRENT_MONTH_AGGREGATE_DAY:
      $theme = 'opigno_lrs_stats_dashboard_widget_total_number_of_page_view_current_month_aggregate_day_json';
      $data = opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_day_between(
        opigno_lrs_stats_start_of_current_month(),
        opigno_lrs_stats_end_of_current_month(),
        opigno_lrs_stats_current_month_viewed_statements()
      );
    break;
    case OPIGNO_LRS_STATS_CURRENT_YEAR_AGGREGATE_DAY:
      $theme = 'opigno_lrs_stats_dashboard_widget_total_number_of_page_view_current_year_aggregate_day_json';
      $data = opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_day_between(
        opigno_lrs_stats_start_of_current_year(),
        opigno_lrs_stats_end_of_current_year(),
        opigno_lrs_stats_current_year_viewed_statements()
      );
    break;
    case OPIGNO_LRS_STATS_CURRENT_YEAR_AGGREGATE_MONTH:
      $theme = 'opigno_lrs_stats_dashboard_widget_total_number_of_page_view_current_year_aggregate_month_json';
      $data = opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_month_between(
        opigno_lrs_stats_start_of_current_year(),
        opigno_lrs_stats_end_of_current_year(),
        opigno_lrs_stats_current_year_viewed_statements()
      );
    break;
    case OPIGNO_LRS_STATS_FOREVER_AGGREGATE_DAY:
      $theme = 'opigno_lrs_stats_dashboard_widget_total_number_of_page_view_forever_aggregate_day_json';
      $data = opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_day_between(
        opigno_lrs_stats_first_statement_timestamp(),
        opigno_lrs_stats_end_of_current_year(),
        opigno_lrs_stats_current_year_viewed_statements()
      );
    break;
    case OPIGNO_LRS_STATS_FOREVER_AGGREGATE_MONTH:
      $theme = 'opigno_lrs_stats_dashboard_widget_total_number_of_page_view_forever_aggregate_month_json';
      $data = opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_month_between(
        opigno_lrs_stats_first_statement_timestamp(),
        opigno_lrs_stats_end_of_current_year(),
        opigno_lrs_stats_current_year_viewed_statements()
      );
    break;
    case OPIGNO_LRS_STATS_FOREVER_AGGREGATE_YEAR:
    default:
      //Forever aggregate by year is default
      $theme = 'opigno_lrs_stats_dashboard_widget_total_number_of_page_view_forever_aggregate_year_json';
      $data = opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_year_between(
        opigno_lrs_stats_first_statement_timestamp(),
        opigno_lrs_stats_end_of_current_year(),
        opigno_lrs_stats_all_viewed_statements()
      );
    break;
  }
  print theme($theme, array('data' => $data));
  return NULL;
}




