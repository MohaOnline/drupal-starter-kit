<?php

require_once(__DIR__ . '/queries.inc');

/**
 * Present user general info
 *
 * @param int $uid
 *
 * @return array
 */
function opigno_statistics_app_present_user_general_informations($uid) {
  return opigno_statistics_app_query_user_general_informations($uid);
}

/**
 * Present badges earned for a user
 *
 * @param int $uid
 *
 * @return array
 */
function opigno_statistics_app_present_user_badges_earned($uid) {
  return opigno_statistics_app_query_user_badges_earned($uid);
}


/**
 * Present user total number of page view
 *
 * Output example
 *
 *  array(
 *    'graph_config' => array(
 *      'element' => 'opigno-statistics-app-user-widget-page-view-chart',
 *      'data' => array(
 *          array('day' => '2015-01-01', 'value' => 4),
 *          array('day' => '2015-01-02', 'value' => 123),
 *          array('day' => '2015-01-03', 'value' => 0),
 *          array('day' => '2015-01-04', 'value' => 455),
 *          array('day' => '2015-01-05', 'value' => 20)
 *      ),
 *      'resize' => true,
 *      'xLabels' => array('day'),
 *      'xkey' => 'day',
 *      'ykeys' => array('value'),
 *      'labels' => array('Number of view per day'),
 *      'xLabelAngle' => -90
 *    )
 *  );
 *
 * @param int $course_id
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return array
 */
function opigno_statistics_app_present_user_total_number_of_page_view($uid, $month_year, $filter_month) {
  $data = opigno_statistics_app_query_user_total_number_of_page_view($uid, $month_year, $filter_month);
  if(empty($data)) $data = array(array('day' => gmdate("Y-m-d", $month_year + 86400), 'value' => 0));
  return array(
    'graph_config' => array(
      'element' => 'opigno-statistics-app-user-widget-page-view-chart',
      'data' => $data,
      'resize' => true,
      'xLabels' => $filter_month? array('day') : array('month'),
      'xkey' => 'day',
      'ykeys' => array('value'),
      'labels' => array(t('Number of view per day')),
      'xLabelAngle' => -90
    )
  );
}

/**
 * Present courses results statistics for a user
 *
 * @param int $uid
 *
 * @return array
 */
function opigno_statistics_app_present_user_courses_results($uid) {
  return opigno_statistics_app_query_user_courses_results($uid);
}