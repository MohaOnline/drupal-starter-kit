<?php

require_once(__DIR__ . '/filter_form.inc');
require_once(__DIR__ . '/presenters.inc');

/*
 * Render course statistic page
 *
 * @param object $account
 */
function opigno_statistics_app_user_statistics_page($account, $user_page) {
  global $user;
  $is_own_page = $user->uid == $account->uid;
  $access = user_access('view all opigno user statistics') || ($is_own_page && user_access('view own opigno user statistics'));
  if ($user_page && (!$access)) {
    return user_view_page($account);
  }
  $module_path = drupal_get_path('module', 'opigno_statistics_app');
  drupal_add_css("{$module_path}/css/opigno_statistics_app_morris.css");
  drupal_add_css("{$module_path}/css/opigno_statistics_app_styles.css");
  drupal_add_js("{$module_path}/js/raphael.min.js");
  drupal_add_js("{$module_path}/js/morris.min.js");
  drupal_add_js("{$module_path}/js/circles.min.js");

  $opigno_statistics_app_user_month_year = variable_get('opigno_statistics_app_user_month_year');
  $opigno_statistics_app_user_has_month = variable_get('opigno_statistics_app_user_has_month');
  $month_year = isset($opigno_statistics_app_user_month_year[request_uri()]) ? $opigno_statistics_app_user_month_year[request_uri()] : mktime(0, 0, 0, 1, 1);
  $filter_month = isset($opigno_statistics_app_user_has_month[request_uri()]) ? $opigno_statistics_app_user_has_month[request_uri()] : FALSE;

  $statistics = theme('opigno_statistics_app_user', array(
    'general_informations' => opigno_statistics_app_present_user_general_informations($account->uid),
    'badges_earned' => opigno_statistics_app_present_user_badges_earned($account->uid),
    'total_number_of_page_view' => opigno_statistics_app_present_user_total_number_of_page_view($account->uid, $month_year, $filter_month),
    'courses_results' => opigno_statistics_app_present_user_courses_results($account->uid)
  ));
  if ($user_page) {
    $user_page = user_view_page($account);
    unset($user_page['user_picture']);
    $statistics .= drupal_render($user_page);
  }
  return $statistics;
}
