<?php

/**
 * @file
 * Course content statistics pages
 */

require_once(__DIR__ . '/general_statistics_form.inc');
require_once(__DIR__ . '/model.inc');

/**
 * Display courses statistic page
 */
function opigno_lrs_stats_course_content_page() {
  $module_path = drupal_get_path('module', 'opigno_tincan_api_stats');
  drupal_add_css("{$module_path}/css/styles.css", 'theme');
  $opigno_lrs=variable_get('opigno_lrs_stats_course_content_general_statistics_form_values');
  switch($opigno_lrs['display_period']) {
    case OPIGNO_LRS_STATS_CURRENT_WEEK:
      $statement_filter_function = 'opigno_lrs_stats_filter_current_week_statements';
    break;
    case OPIGNO_LRS_STATS_CURRENT_MONTH:
      $statement_filter_function = 'opigno_lrs_stats_filter_current_month_statements';
    break;
    case OPIGNO_LRS_STATS_CURRENT_YEAR:
      $statement_filter_function = 'opigno_lrs_stats_filter_current_year_statements';
    break;
    case OPIGNO_LRS_STATS_FOREVER:
    default:
      $statement_filter_function = NULL;
    break;
  }

  return theme('opigno_lrs_stats_course_content', array(
    'general_statistics' => opigno_lrs_stats_course_content_general_statistics($statement_filter_function),
    'course_contexts_statistics' => opigno_lrs_stats_course_content_course_contexts_statistics($statement_filter_function)
  ));
}