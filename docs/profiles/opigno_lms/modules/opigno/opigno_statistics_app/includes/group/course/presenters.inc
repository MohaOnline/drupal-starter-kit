<?php

require_once(__DIR__ . '/queries.inc');

/**
 * Present course total number of page view
 *
 * Output example
 *
 *  array(
 *    'graph_config' => array(
 *      'element' => 'opigno-statistics-app-course-widget-page-view-chart',
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
function opigno_statistics_app_present_course_total_number_of_page_view($course_id, $month_year, $filter_month) {
  $data = opigno_statistics_app_query_course_total_number_of_page_view($course_id, $month_year, $filter_month);
  if(empty($data)) $data = array(array('day' => gmdate("Y-m-d", $month_year + 86400), 'value' => 0));
  $color_palette=color_get_palette('platon');
  return array(
    'graph_config' => array(
      'element' => 'opigno-statistics-app-course-widget-page-view-chart',
      'data' => $data,
      'resize' => true,
      'xLabels' => array('day'),
      'xkey' => 'day',
      'ykeys' => array('value'),
      'labels' => array(t('Number of view per day')),
      'xLabelAngle' => -90,
      'lineColors' => array($color_palette['dark_blue'])
    )
  );
}

/**
 * Present course general statistics
 *
 * Output example:
 * array(
 *    'course_progress_percentage' => 22
 *    'quizz_completion_percentage' => 15
 *  );
 *
 * @param int $course_nid
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return array
 */
function opigno_statistics_app_present_course_general_statistics($course_nid, $month_year, $filter_month) {
  return array(
    'course_progress_percentage' => round(opigno_statistics_app_query_course_course_progress_percentage($course_nid, $month_year, $filter_month) * 100),
    'quizz_completion_percentage' => round(opigno_statistics_app_query_course_quiz_completion_percentage($course_nid, $month_year, $filter_month) * 100)
  );
}

/**
 * Present course lessons statistics
 *
 * @param int $course_nid
 *
 * @return array
 */
function opigno_statistics_app_present_course_course_lessons($course_nid) {
  return opigno_statistics_app_query_course_course_lessons($course_nid);
}

/**
 * Present number of interaction graph for course
 *
 * Output example
 *
 *  array(
 *    'graph_config' => array(
 *      'element' => 'opigno-statistics-app-course-widget-page-view-chart',
 *      'data' => array(
 *        array('score' => 100, 'number_of_interactions' => 12),
 *        array('score' => 30, 'number_of_interactions' => 45),
 *        array('score' => 50, 'number_of_interactions' => 2),
 *        array('score' => 5, 'number_of_interactions' => 123),
 *        array('score' => 75, 'number_of_interactions' => 23)
 *      ),
 *      'resize' => true,
 *      'xLabels' => array('Number of interactions'),
 *      'xkey' => 'number_of_interactions',
 *      'ykeys' => array('score'),
 *      'labels' => array('Score'),
 *      'parseTime' => false
 *    )
 *  );
 *
 * @param int $course_id
 *
 * @return array
 */
function opigno_statistics_app_present_course_number_of_interactions($course_id) {
  $data = opigno_statistics_app_query_course_number_of_interactions($course_id);
  if(empty($data)) $data = array(array('score' => 0, 'number_of_interactions' => 0));
  $color_palette=color_get_palette('platon');
  return array(
    'graph_config' => array(
      'element' => 'opigno-statistics-app-course-widget-number-of-interactions-chart',
      'data' => $data,
      'resize' => true,
      'xLabels' => array('Number of interactions'),
      'xkey' => 'number_of_interactions',
      'ykeys' => array('score'),
      'labels' => array('score'),
      'parseTime' => false,
      'xLabelAngle' => -90,
      'lineWidth' => 0,
      'lineColors' => array($color_palette['dark_blue'])
    )
  );
}

/**
 * Present student results statistics for a course
 *
 * @param int $course_nid
 *
 * @return array
 */
function opigno_statistics_app_present_course_students_results($course_nid) {
  return opigno_statistics_app_query_course_students_results($course_nid);
}