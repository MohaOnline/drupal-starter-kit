<?php

/**
 * Retrieve top 10 groups (cached for 1 day)
 *
 * Ouput example:
 * array(
 *    array(
 *      'nid' => 1,
 *      'title' => 'Course 1',
 *      'number_of_visits' => 123,
 *      'number_of_users' => 45,
 *      'number_passed' => 23,
 *    ),
 *    array(
 *      'nid' => 2,
 *      'title' => 'Course 2',
 *      'number_of_visits' => 300,
 *      'number_of_users' => 56,
 *      'number_passed' => 30,
 *    ),
 *    array(
 *      'nid' => 1,
 *      'title' => 'Course 3',
 *      'number_of_visits' => 140,
 *      'number_of_users' => 34,
 *      'number_passed' => 20,
 *    )
 *  )
 *
 * @param string $group_type
 * @param int $month_year
 * @param boolean $filter_month
 * @param int $category_id
 *
 * @return array
 */
function opigno_statistics_app_query_top_10_groups($group_type, $month_year, $filter_month, $category_id) {
  $cache_key = __FUNCTION__ . ':' . $group_type . ':' . $month_year . ':' . $filter_month .':'. $category_id;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $top_10_groups = $cached_object->data;
  } else {
    $top_10_groups = array();

    if($filter_month) {
      if($category_id=='') {
        $sql_filter_statistics_group = "WHERE group_type = :group_type AND month_year = :month_year";
        $sql_filter_values = array(
          ':month_year' => $month_year,
          ':group_type' => $group_type
        );
      } else {
        $sql_filter_statistics_group = "WHERE group_type = :group_type AND month_year = :month_year AND category_taxonomy_term_id = :category_id";
        $sql_filter_values = array(
          ':month_year' => $month_year,
          ':group_type' => $group_type,
          ':category_id' => $category_id
        );
      }
    } else {
      if($category_id=='') {
        $sql_filter_statistics_group = "WHERE group_type = :group_type AND month_year >= :month_year";
        $sql_filter_values = array(
          ':month_year' => $month_year,
          ':group_type' => $group_type
        );
      }
      else {
        $sql_filter_statistics_group = "WHERE group_type = :group_type AND month_year >= :month_year AND category_taxonomy_term_id = :category_id";
        $sql_filter_values = array(
          ':month_year' => $month_year,
          ':group_type' => $group_type,
          ':category_id' => $category_id
        );
      }
    }


    $result = db_query("
        SELECT g2.group_title, g2.page_views, g2.number_passed, COUNT(DISTINCT ug.uid) as nb_members, g2.group_nid
        FROM (
               SELECT group_title, SUM(page_views) as page_views, SUM(number_passed) as number_passed, group_nid
               FROM (
                      SELECT group_title, page_views, number_passed, group_nid
                      FROM {opigno_statistics_group}
                      ". $sql_filter_statistics_group ."
                      ORDER BY month_year DESC
                    ) g1
               GROUP BY group_nid
             ) g2
        INNER JOIN opigno_statistics_user_group ug ON g2.group_nid = ug.group_nid
        GROUP BY g2.group_nid
        ORDER BY g2.page_views DESC, g2.group_title
        LIMIT 10
      ",
      $sql_filter_values);


    while($record = $result->fetchAssoc()) {

      if (db_select('node', 't')->fields('t')->condition('nid', $record['group_nid'])->execute()->rowCount()) {
        $stats_link = 'node/'. $record['group_nid'] .'/opigno-statistics';
      }
      else {
        $stats_link = 'opigno-statistics/'. $record['group_nid'] .'/group';
      }
      $record['stats_link'] = $stats_link;

      $top_10_groups[] = $record;

    }
    cache_set($cache_key, $top_10_groups, 'cache', time() + 7200); // 7200s = 2h cache
  }

  return $top_10_groups;
}

/**
 * Retrieve general quiz completion percentage (cached for 1 day)
 *
 * @param int $month_year
 * @param boolean $filter_month
 * @param int $category_id
 *
 * @return int
 */
function opigno_statistics_app_query_quiz_completion_percentage($month_year, $filter_month, $category_id){
  $cache_key = __FUNCTION__ . ':' . $month_year . ':' . $filter_month . ':' . $category_id;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $percentage = $cached_object->data;
  } else {
    if($filter_month) {
      if($category_id=='') {
        $result = db_query('SELECT AVG(status) FROM {opigno_statistics_user_course_details}
                          WHERE type = :type
                          AND DATE_FORMAT(FROM_UNIXTIME(timestamp),\'%m-%Y\') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),\'%m-%Y\')', array(
          ':type' => 'quiz',
          ':timestamp' => $month_year
        ))->fetchCol();
      } else {
        $result = db_query('SELECT AVG(cd.status) FROM {opigno_statistics_user_course_details} cd
                          INNER JOIN {opigno_statistics_user_course} uc
                          ON cd.opigno_statistics_user_course_fk = uc.opigno_statistics_user_course_pk
                          INNER JOIN {opigno_statistics_group} g
                          ON uc.course_nid = g.group_nid
                          WHERE type = :type
                          AND DATE_FORMAT(FROM_UNIXTIME(cd.timestamp),\'%m-%Y\') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),\'%m-%Y\')
                          AND g.category_taxonomy_term_id = :category_id', array(
          ':type' => 'quiz',
          ':timestamp' => $month_year,
          ':category_id' => $category_id
        ))->fetchCol();
      }
    } else {
      if($category_id=='') {
        $result = db_query('SELECT AVG(status) FROM {opigno_statistics_user_course_details}
                          WHERE type = :type
                          AND DATE_FORMAT(FROM_UNIXTIME(timestamp),\'%Y\') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),\'%Y\')', array(
          ':type' => 'quiz',
          ':timestamp' => $month_year
        ))->fetchCol();
      } else {
        $result = db_query('SELECT AVG(cd.status) FROM {opigno_statistics_user_course_details} cd
                          INNER JOIN {opigno_statistics_user_course} uc
                          ON cd.opigno_statistics_user_course_fk = uc.opigno_statistics_user_course_pk
                          INNER JOIN {opigno_statistics_group} g
                          ON uc.course_nid = g.group_nid
                          WHERE type = :type
                          AND DATE_FORMAT(FROM_UNIXTIME(cd.timestamp),\'%Y\') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),\'%Y\')
                          AND g.category_taxonomy_term_id = :category_id', array(
          ':type' => 'quiz',
          ':timestamp' => $month_year,
          ':category_id' => $category_id
        ))->fetchCol();
      }
    }
    $percentage = $result[0];
    cache_set($cache_key, $percentage, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $percentage;
}

/**
 * Retrieve general course progress percentage (cached for 1 day)
 *
 * @param int $month_year
 * @param boolean $filter_month
 * @param int $category_id
 *
 * @return int
 */
function opigno_statistics_app_query_general_course_progress_percentage($month_year, $filter_month, $category_id){
  $cache_key = __FUNCTION__ . ':' . $month_year . ':' . $filter_month . ':' . $category_id;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $percentage = $cached_object->data;
  } else {
    if($filter_month){
      $result = db_query('SELECT AVG(g.number_passed / (SELECT
                            COUNT(DISTINCT ug.uid) FROM {opigno_statistics_user_group} ug WHERE ug.group_nid = g.group_nid
                          ))
                          FROM {opigno_statistics_group} g
                          WHERE g.month_year <= :month_year
                          AND g.group_type = :type
                          AND g.category_taxonomy_term_id = :category_id', array(
        ':month_year' => $month_year,
        ':category_id' => $category_id,
        ':type' => 'course'
      ))->fetchCol();
    } else {
      $year = gmdate('Y', $month_year + 86400);
      $result = db_query('SELECT AVG(g.number_passed / (SELECT
                            COUNT(DISTINCT ug.uid) FROM {opigno_statistics_user_group} ug WHERE ug.group_nid = g.group_nid
                          ))
                          FROM {opigno_statistics_group} g
                          WHERE EXTRACT(YEAR FROM FROM_UNIXTIME(g.month_year)) <= :year
                          AND g.group_type = :type
                          AND g.category_taxonomy_term_id = :category_id', array(
        ':year' => $year,
        ':category_id' => $category_id,
        ':type' => 'course'
      ))->fetchCol();
    }
    $percentage = $result[0];
    cache_set($cache_key, $percentage, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $percentage;
}

/**
 * Query total number of page view (cached for 1 day)
 *
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return array
 */
function opigno_statistics_app_query_total_number_of_page_view($month_year, $filter_month) {
  $cache_key = __FUNCTION__ . ':' . $month_year . ':' . $filter_month;
  $cached_object = cache_get($cache_key);
  if($cached_object) {

    $total_number_of_page_view = $cached_object->data;

  } else {
    $total_number_of_page_view = array();

    // The accesslog table is flushed every month. So if the user want the data for the current month, use the accesslog
    //    Else, use the statistics_group table
    if ($filter_month && date('Y-m') == date('Y-m', $month_year)) {
      $result = db_query("
          SELECT DATE_FORMAT(FROM_UNIXTIME(timestamp), '%Y-%m-%d') as day, COUNT(*) as value
          FROM {accesslog}
          WHERE DATE_FORMAT(FROM_UNIXTIME(timestamp), '%m-%Y') = DATE_FORMAT(FROM_UNIXTIME(:timestamp), '%m-%Y')
          GROUP BY day
        ", array(
          ':timestamp' => $month_year
      ));
    }
    else {
      $filter_month_year = ($filter_month ? '%m-%Y' : '%Y');

      $result = db_query("
        SELECT DATE_FORMAT(FROM_UNIXTIME(month_year), '%Y-%m-%d') as day, SUM(page_views) as value
        FROM {opigno_statistics_group}
        WHERE DATE_FORMAT(FROM_UNIXTIME(month_year), '". $filter_month_year ."') = DATE_FORMAT(FROM_UNIXTIME(:month_year), '". $filter_month_year ."')
        GROUP BY day
      ", array(
        ':month_year' => $month_year
      ));
    }

    while($record = $result->fetchAssoc()) $total_number_of_page_view[] = $record;
    cache_set($cache_key, $total_number_of_page_view, 'cache', time() + 7200); // 7200s = 2h cache
  }

  return $total_number_of_page_view;
}

/**
 * Retrieve active user last week percentage (cached for 1 day)
 *
 * @return float
 */
function opigno_statistics_app_query_active_users_last_week_percentage() {
  $cache_key = __FUNCTION__;
  $cached_object = cache_get($cache_key);
  if($cached_object)
  {
    $percentage = $cached_object->data;
  }
  else
  {
    $number_of_users_last_week = db_query('SELECT COUNT(distinct uid) FROM {accesslog} WHERE timestamp BETWEEN :start_of_last_week AND :end_of_last_week ', array(
      ':start_of_last_week' => opigno_statistics_app_start_of_last_week(),
      ':end_of_last_week' => opigno_statistics_app_end_of_last_week()
    ))->fetchField();
    $total_number_of_users = db_query('SELECT COUNT(distinct uid) FROM {accesslog}')->fetchField();
    $percentage = ($total_number_of_users != 0 && $number_of_users_last_week != 0)? $number_of_users_last_week/$total_number_of_users : 0;
    cache_set($cache_key, $percentage, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $percentage;
}

/**
 * Retrieve 10 most active users (sorted, cached for 1 day)
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
function opigno_statistics_app_query_most_active_users(){
  $cache_key = __FUNCTION__;
  $cached_object = cache_get($cache_key);
  if($cached_object)
  {
    $most_active_users = $cached_object->data;
  }
  else
  {
    $most_active_users = array();
    $result = db_query('
      SELECT accesslog.uid, MAX(accesslog.timestamp) as "last_visit", login_history.user_name
      FROM {accesslog}
      INNER JOIN {opigno_statistics_login_history} as login_history ON accesslog.uid = login_history.uid
      WHERE accesslog.uid <> 0
      GROUP BY accesslog.uid
      ORDER BY COUNT(*) DESC
      LIMIT 10');
    while($record = $result->fetchAssoc()) $most_active_users[] = $record;
    cache_set($cache_key, $most_active_users, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $most_active_users;
}
