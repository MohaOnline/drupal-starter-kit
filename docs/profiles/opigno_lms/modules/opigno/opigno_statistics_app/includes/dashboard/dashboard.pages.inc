<?php

require_once(__DIR__ . '/filter_form.inc');
require_once(__DIR__ . '/presenters.inc');

/*
 * Render dashboard page
 */
function opigno_statistics_app_dashboard_page() {
  $module_path = drupal_get_path('module', 'opigno_statistics_app');
  drupal_add_css("{$module_path}/css/opigno_statistics_app_morris.css");
  drupal_add_css("{$module_path}/css/opigno_statistics_app_styles.css");
  drupal_add_js("{$module_path}/js/raphael.min.js");
  drupal_add_js("{$module_path}/js/morris.min.js");
  drupal_add_js("{$module_path}/js/circles.min.js");

  $month_year = variable_get('opigno_statistics_app_dashboard_month_year', mktime(0, 0, 0, 1, 1));
  $filter_month = variable_get('opigno_statistics_app_dashboard_has_month', FALSE);
  $category_id = variable_get('opigno_statistics_app_dashboard_category', 0);
  return theme('opigno_statistics_app_dashboard', array(
    'general_statistics' => opigno_statistics_app_present_general_statistics($month_year, $filter_month, $category_id),
    'total_number_of_page_view' => opigno_statistics_app_present_total_number_of_page_view($month_year, $filter_month),
    'active_users_last_week' => opigno_statistics_app_present_active_users_last_week_percentage(),
    'most_active_users' => opigno_statistics_app_present_most_active_users(),
    'top_10_courses' => opigno_statistics_app_present_top_10_courses($month_year, $filter_month, $category_id),
    'top_10_classes' => opigno_statistics_app_present_top_10_classes($month_year, $filter_month, $category_id)
  ));
}