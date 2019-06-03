<?php

require_once(__DIR__ . '/queries.inc');

/**
 * Present total number of page view
 *
 * Output example
 *
 *  array(
 *    'graph_config' => array(
 *    'element' => 'opigno-statistics-app-dashboard-widget-page-view-chart',
 *    'data' => array(
 *        array('day' => '2015-01-01', 'value' => 4),
 *        array('day' => '2015-01-02', 'value' => 123),
 *        array('day' => '2015-01-03', 'value' => 0),
 *        array('day' => '2015-01-04', 'value' => 455),
 *        array('day' => '2015-01-05', 'value' => 20)
 *    ),
 *    'resize' => true,
 *    'xLabels' => array('day'),
 *    'xkey' => 'day',
 *    'ykeys' => array('value'),
 *    'labels' => array('Number of view per day'),
 *    'xLabelAngle' => -90
 *    )
 *  );
 *
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return array
 */
function opigno_statistics_app_present_total_number_of_page_view($month_year, $filter_month) {
  $data = opigno_statistics_app_query_total_number_of_page_view($month_year, $filter_month);
  if(empty($data)) $data = array(array('day' => gmdate("Y-m-d", $month_year + 86400), 'value' => 0));
  $color_pallete=color_get_palette('platon');
  return array(
    'graph_config' => array(
      'element' => 'opigno-statistics-app-dashboard-widget-page-view-chart',
      'data' => $data,
      'resize' => true,
      'xLabels' => $filter_month? array('day') : array('month'),
      'xkey' => 'day',
      'ykeys' => array('value'),
      'labels' => array(t('Number of view per day')),
      'xLabelAngle' => -90,
      'lineColors' => array($color_pallete['dark_blue'])
    )
  );
}

/**
 * Present general statistics
 *
 * Output example:
 * array(
 *    'course_progress_percentage' => 22
 *    'quizz_completion_percentage' => 15
 *  );
 *
 * @param int $month_year
 * @param boolean $filter_month
 * @param int $category_id
 *
 * @return array
 */
function opigno_statistics_app_present_general_statistics($month_year, $filter_month, $category_id) {
  return array(
    'course_progress_percentage' => round(opigno_statistics_app_query_general_course_progress_percentage($month_year, $filter_month, $category_id) * 100),
    'quizz_completion_percentage' => round(opigno_statistics_app_query_quiz_completion_percentage($month_year, $filter_month, $category_id) * 100)
  );
}

/**
 * Present top 10 courses
 *
 * @param int $month_year
 * @param boolean $filter_month
 * @param int $category_id
 *
 * @return array
 */
function opigno_statistics_app_present_top_10_courses($month_year, $filter_month, $category_id) {
  return opigno_statistics_app_query_top_10_groups('course', $month_year, $filter_month, $category_id);
}

/**
 * Present top 10 classes
 *
 * @param int $month_year
 * @param boolean $filter_month
 * @param int $category_id
 *
 * @return array
 */
function opigno_statistics_app_present_top_10_classes($month_year, $filter_month, $category_id) {
  return opigno_statistics_app_query_top_10_groups('class', $month_year, $filter_month, $category_id);
}

/**
 * Present active users last week percentage
 *
 * Output example:
 * array(
 *    'percentage' => 15
 * );
 *
 * @return array
 */
function opigno_statistics_app_present_active_users_last_week_percentage() {
  return array(
    'percentage' => round(opigno_statistics_app_query_active_users_last_week_percentage() * 100)
  );
}

/**
 * Present most active users
 *
 * Output example:
 * array(
 *    array(
 *      'uid' => 1,
 *      'last_visit' => 19955055
 *    ),
 *    array(
 *      'uid' => 10,
 *      'last_visit' => 20292599
 *    ),
 *  );
 *
 * @return array
 */
function opigno_statistics_app_present_most_active_users() {
  return opigno_statistics_app_query_most_active_users();
}