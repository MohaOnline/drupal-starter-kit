<?php

require_once(__DIR__ . '/queries.inc');


/**
 * Present class total number of page view
 *
 * Output example
 *
 *  array(
 *    'graph_config' => array(
 *      'element' => 'opigno-statistics-app-class-widget-page-view-chart',
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
 * @param int $class_id
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return array
 */
function opigno_statistics_app_present_class_total_number_of_page_view($class_id, $month_year, $filter_month) {
  $data = opigno_statistics_app_query_class_total_number_of_page_view($class_id, $month_year, $filter_month);
  if(empty($data)) $data = array(array('day' => gmdate("Y-m-d", $month_year + 86400), 'value' => 0));
  $color_pallete=color_get_palette('platon');
  return array(
    'graph_config' => array(
      'element' => 'opigno-statistics-app-class-widget-page-view-chart',
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
 * Present class general statistics
 *
 * Output example:
 * array(
 *    'class_progress_percentage' => 22
 *    'quizz_completion_percentage' => 15
 *  );
 *
 * @param int $class_nid
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return array
 */
function opigno_statistics_app_present_class_general_statistics($class_nid, $month_year, $filter_month) {
  return array(
    'course_progress_percentage' => round(opigno_statistics_app_query_class_course_progress_percentage($class_nid, $month_year, $filter_month) * 100),
    'quizz_completion_percentage' => round(opigno_statistics_app_query_class_quiz_completion_percentage($class_nid, $month_year, $filter_month) * 100)
  );
}

/**
 * Present number of interaction graph for class
 *
 * Output example
 *
 *  array(
 *    'graph_config' => array(
 *      'element' => 'opigno-statistics-app-class-widget-page-view-chart',
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
 * @param int $class_id
 *
 * @return array
 */
function opigno_statistics_app_present_class_number_of_interactions($class_id) {
  $data = opigno_statistics_app_query_class_number_of_interactions($class_id);
  if(empty($data)) $data = array(array('score' => 0, 'number_of_interactions' => 0));
  $color_palette=color_get_palette('platon');
  return array(
    'graph_config' => array(
      'element' => 'opigno-statistics-app-class-widget-number-of-interactions-chart',
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
 * Present student results statistics for a class
 *
 * @param int $class_nid
 *
 * @return array
 */
function opigno_statistics_app_present_class_students_results($class_nid) {
  return opigno_statistics_app_query_class_students_results($class_nid);
}