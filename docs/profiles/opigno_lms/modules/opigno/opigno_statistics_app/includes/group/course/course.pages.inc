<?php

require_once(__DIR__ . '/../filter_form.inc');
require_once(__DIR__ . '/presenters.inc');

/*
 * Render course statistic page
 */
function opigno_statistics_app_course_statistics_page($course_nid){
  $module_path = drupal_get_path('module', 'opigno_statistics_app');
  drupal_add_css("{$module_path}/css/opigno_statistics_app_morris.css");
  drupal_add_css("{$module_path}/css/opigno_statistics_app_styles.css");
  drupal_add_js("{$module_path}/js/raphael.min.js");
  drupal_add_js("{$module_path}/js/morris.min.js");
  drupal_add_js("{$module_path}/js/circles.min.js");
  $opigno_statistics_app_group_month_year=variable_get('opigno_statistics_app_group_month_year');
  $opigno_statistics_app_group_has_month=variable_get('opigno_statistics_app_group_has_month');
  $month_year = isset($opigno_statistics_app_group_month_year[request_uri()])? $opigno_statistics_app_group_month_year[request_uri()] : mktime(0, 0, 0, date('m', time()), 1);
  $filter_month = isset($opigno_statistics_app_group_has_month[request_uri()])? $opigno_statistics_app_group_has_month[request_uri()] : TRUE;
  return theme('opigno_statistics_app_course', array(
    'general_statistics' => opigno_statistics_app_present_course_general_statistics($course_nid, $month_year, $filter_month),
    'total_number_of_page_view' => opigno_statistics_app_present_course_total_number_of_page_view($course_nid, $month_year, $filter_month),
    'number_of_interactions' => opigno_statistics_app_present_course_number_of_interactions($course_nid, $month_year, $filter_month),
    'course_lessons' => opigno_statistics_app_present_course_course_lessons($course_nid),
    'students_results' => opigno_statistics_app_present_course_students_results($course_nid)
  ));
}