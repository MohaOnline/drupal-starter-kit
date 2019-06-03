<?php

/**
 * Retrieve course progress percentage for a course (cached for 1 day)
 *
 * @param int $course_nid
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return int
 */
function opigno_statistics_app_query_course_course_progress_percentage($course_nid, $month_year, $filter_month){
  $cache_key = __FUNCTION__ . ':' . $course_nid . ':' . $month_year . ':' . $filter_month;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $percentage = $cached_object->data;
  } else {
    if($filter_month){
      $result = db_query('SELECT AVG(number_passed / (SELECT
                            COUNT(DISTINCT ug.uid) FROM {opigno_statistics_user_group} ug WHERE ug.group_nid = g.group_nid
                          ))
                          FROM {opigno_statistics_group} g
                          WHERE group_nid = :group_nid
                          AND month_year <= :month_year', array(
        ':group_nid' => $course_nid,
        ':month_year' => $month_year
      ))->fetchCol();
    } else {
      $year = gmdate('Y', $month_year + 86400);
      $result = db_query('SELECT AVG(number_passed / (SELECT
                            COUNT(DISTINCT ug.uid) FROM {opigno_statistics_user_group} ug WHERE ug.group_nid = g.group_nid
                          ))
                          FROM {opigno_statistics_group} g
                          WHERE group_nid = :group_nid
                          AND EXTRACT(YEAR FROM FROM_UNIXTIME(month_year)) <= :year', array(
        ':group_nid' => $course_nid,
        ':year' => $year
      ))->fetchCol();
    }
    $percentage = $result[0];
    cache_set($cache_key, $percentage, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $percentage;
}

/**
 * Retrieve course quiz completion percentage (cached for 1 day)
 *
 * @param int $course_nid
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return int
 */
function opigno_statistics_app_query_course_quiz_completion_percentage($course_nid, $month_year, $filter_month){
  $cache_key = __FUNCTION__ . ':' . $course_nid . ':' . $month_year . ':' . $filter_month;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $percentage = $cached_object->data;
  } else {
    if($filter_month){
      $result = db_query('SELECT AVG(cd.status) FROM {opigno_statistics_user_course_details} cd
                          INNER JOIN opigno_statistics_user_course uc
                          ON cd.opigno_statistics_user_course_fk = uc.opigno_statistics_user_course_pk
                          WHERE uc.course_nid = :course_nid
                          AND cd.type = :type
                          AND DATE_FORMAT(FROM_UNIXTIME(cd.timestamp),\'%m-%Y\') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),\'%m-%Y\')', array(
        ':course_nid' => $course_nid,
        ':type' => 'quiz',
        ':timestamp' => $month_year
      ))->fetchCol();
    } else {
      $result = db_query('SELECT AVG(cd.status) FROM {opigno_statistics_user_course_details} cd
                          INNER JOIN opigno_statistics_user_course uc
                          ON cd.opigno_statistics_user_course_fk = uc.opigno_statistics_user_course_pk
                          WHERE uc.course_nid = :course_nid
                          AND cd.type = :type
                          AND DATE_FORMAT(FROM_UNIXTIME(cd.timestamp),\'%Y\') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),\'%Y\')', array(
        ':course_nid' => $course_nid,
        ':type' => 'quiz',
        ':timestamp' => $month_year
      ))->fetchCol();
    }

    $percentage = $result[0];
    cache_set($cache_key, $percentage, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $percentage;
}

/**
 * Query total number of page view for a course (cached for 1 day)
 *
 * @param int $course_nid
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return array
 */
function opigno_statistics_app_query_course_total_number_of_page_view($course_nid, $month_year, $filter_month) {
  $cache_key = __FUNCTION__ . ':' . $course_nid . ':' . $month_year . ':' . $filter_month;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $total_number_of_page_view = $cached_object->data;
  } else {
    $total_number_of_page_view = array();

    // If the filter is for the current month, we use the accesslog table (because the accesslog is flushed every month)
    //   Else, we use the opigno_statistics_group table
    if ($filter_month && date('Y-m') == date('Y-m', $month_year)) {
      // First, get all the nodes that are in the course
      $all_child_query = db_query("
        SELECT etid
        FROM {og_membership}
        WHERE group_type = 'node'
        AND entity_type = 'node'
        AND gid = :course_id
      ", array(
            ':course_id' => $course_nid
      ));
      $all_child_nids = array_unique($all_child_query->fetchCol());
      $all_child_nids[] = $course_nid;



      // Construct the node ID filter
      $node_id_filter = '';
      foreach($all_child_nids as $nid_to_filter)
        $node_id_filter .= "path LIKE 'node/". $nid_to_filter ."%' OR ";
      $node_id_filter = substr($node_id_filter, 0, -4);


      // Then, get the number of pages view by the users of the class
      $result = db_query("
          SELECT DATE_FORMAT(FROM_UNIXTIME(timestamp),'%Y-%m-%d') as day, COUNT(*) as value
          FROM {accesslog} a
          WHERE (". $node_id_filter .") AND a.uid IN (
            SELECT DISTINCT uid
            FROM {opigno_statistics_user_group} ug
            WHERE group_nid = :group_nid
          )
          AND DATE_FORMAT(FROM_UNIXTIME(a.timestamp),'%m-%Y') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),'%m-%Y')
          GROUP BY day
        ", array(
          ':group_nid' => $course_nid,
          ':timestamp' => $month_year
      ));
    }
    // If the user selected no filter or filter another month than the current one, we use the statistics_group table
    else {
      $filter_date_format = ($filter_month ? '%m-%Y' : '%Y');

      $result = db_query("
          SELECT DATE_FORMAT(FROM_UNIXTIME(month_year),'%Y-%m-%d') as day, page_views as value
          FROM {opigno_statistics_group}
          WHERE DATE_FORMAT(FROM_UNIXTIME(month_year), '". $filter_date_format ."') = DATE_FORMAT(FROM_UNIXTIME(:month_year), '". $filter_date_format ."')
          AND group_nid = :group_nid
        ", array(
          ':group_nid' => $course_nid,
          ':month_year' => $month_year
      ));
    }

    while($record = $result->fetchAssoc()) $total_number_of_page_view[] = $record;
    cache_set($cache_key, $total_number_of_page_view, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $total_number_of_page_view;
}

/**
 * Query course lesson statistics for a course (cached for 1 day)
 *
 * @param int $course_nid
 *
 * @return array
 */
function opigno_statistics_app_query_course_course_lessons($course_nid) {
  $cache_key = __FUNCTION__ . ':' . $course_nid;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $course_lessons = $cached_object->data;
  } else {
    $course_lessons = array();
    $result = db_query("
      SELECT ucd.entity_name as lesson_name, ROUND(AVG(ucd.score), 0) as score, SUM(ug.page_views) as number_of_interactions
      FROM {opigno_statistics_user_course_details} ucd
      LEFT JOIN opigno_statistics_user_group ug ON ug.nid = ucd.entity_id
      WHERE entity_id IN (
        SELECT etid
        FROM {og_membership}
        WHERE group_type = 'node'
        AND entity_type = 'node'
        AND gid = :course_nid
      )
      GROUP BY entity_id
    ", array(
      ':course_nid' => $course_nid
    ));

    while($record = $result->fetchAssoc()) {
      if (empty($record['number_of_interactions']))
        $record['number_of_interactions'] = 0;

      $course_lessons[] = $record;
    }

    cache_set($cache_key, $course_lessons, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $course_lessons;
}

/**
 * Query total number of interaction for a course (cached for 1 day)
 *
 * @param int $course_nid
 *
 * @return array
 */
function opigno_statistics_app_query_course_number_of_interactions($course_nid){
  $cache_key = __FUNCTION__ . ':' . $course_nid;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $number_of_interactions = $cached_object->data;
  } else {
    $number_of_interactions = array();

    $result = db_query("
      SELECT uc.username, uc.score, j1.page_views as number_of_interactions
      FROM {opigno_statistics_user_course} uc
      INNER JOIN (
        SELECT SUM(page_views) as page_views, uid
        FROM {opigno_statistics_user_group} ug
        WHERE group_nid = :course_nid
        GROUP BY uid
      ) j1 ON uc.uid = j1.uid
      WHERE course_nid = :course_nid
      ORDER BY j1.page_views
    ", array(
      ':course_nid' => $course_nid
    ));

    while($record = $result->fetchAssoc()) $number_of_interactions[] = $record;
    cache_set($cache_key, $number_of_interactions, 'cache', time() + 7200); // 7200s = 2h cache
  }

  return $number_of_interactions;
}

/**
 * Query students results statistics for a course (cached for 1 day)
 *
 * @param int $course_nid
 *
 * @return array
 */
function opigno_statistics_app_query_course_students_results($course_nid) {
  $cache_key = __FUNCTION__ . ':' . $course_nid;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $students_results = $cached_object->data;
  } else {
    $students_results = array();
    $result = db_query("
      SELECT uc.username as student_name, j1.page_views as number_of_interactions, uc.score, uc.status, uc.uid
      FROM {opigno_statistics_user_course} uc
      INNER JOIN (
        SELECT SUM(page_views) as page_views, uid
        FROM {opigno_statistics_user_group} ug
        WHERE group_nid = :course_nid
        GROUP BY uid
      ) j1 ON uc.uid = j1.uid
      WHERE course_nid = :course_nid
    ", array(
      ':course_nid' => $course_nid
    ));

    $total_number_of_interactions = 0;
    $total_score = 0;
    while($record = $result->fetchAssoc()) {
      $students_results[] = $record;
      $total_number_of_interactions += $record['number_of_interactions'];
      $total_score += $record['score'];
    }

    $avg_number_of_interactions = count($students_results)>0 ? $total_number_of_interactions / count($students_results) : 0;
    $avg_score = count($students_results)>0 ? $total_score / count($students_results) : 0;

    foreach($students_results as &$student_result) {
      $student_result['avg_number_of_interactions'] = round($avg_number_of_interactions, 0);
      $student_result['avg_score'] = round($avg_score, 0);
    }

    cache_set($cache_key, $students_results, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $students_results;
}