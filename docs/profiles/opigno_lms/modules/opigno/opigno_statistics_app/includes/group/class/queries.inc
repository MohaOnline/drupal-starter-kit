<?php

/**
 * Retrieve course progress percentage for a class (cached for 1 day)
 *
 * @param int $class_nid
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return int
 */
function opigno_statistics_app_query_class_course_progress_percentage($class_nid, $month_year, $filter_month){
  $cache_key = __FUNCTION__ . ':' . $class_nid . ':' . $month_year . ':' . $filter_month;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $percentage = $cached_object->data;
  } else {
    if($filter_month){
      $result = db_query('SELECT AVG(number_passed / (SELECT
                            COUNT(DISTINCT ug.uid) FROM {opigno_statistics_user_group} ug WHERE ug.group_nid = og.group_nid
                          )) FROM {opigno_statistics_group} og
                          INNER JOIN {opigno_statistics_user_course} uc
                          ON og.group_nid = uc.course_nid
                          WHERE uc.uid IN (
                            SELECT uid FROM {opigno_statistics_user_group} ug
                            WHERE ug.group_nid = :group_nid
                          )
                          AND og.group_type = :type
                          AND month_year <= :month_year', array(
        ':type' => 'course',
        ':group_nid' => $class_nid,
        ':month_year' => $month_year
      ))->fetchCol();
    } else {
      $year = gmdate('Y', $month_year + 86400);
      $result = db_query('SELECT AVG(number_passed / (SELECT
                            COUNT(DISTINCT ug.uid) FROM {opigno_statistics_user_group} ug WHERE ug.group_nid = og.group_nid
                          )) FROM {opigno_statistics_group} og
                          INNER JOIN {opigno_statistics_user_course} uc
                          ON og.group_nid = uc.course_nid
                          WHERE uc.uid IN (
                            SELECT uid FROM {opigno_statistics_user_group} ug
                            WHERE ug.group_nid = :group_nid
                          )
                          AND og.group_type = :type
                          AND EXTRACT(YEAR FROM FROM_UNIXTIME(month_year)) <= :year', array(
        ':type' => 'course',
        ':group_nid' => $class_nid,
        ':year' => $year
      ))->fetchCol();
    }
    $percentage = $result[0];
    cache_set($cache_key, $percentage, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $percentage;
}

/**
 * Retrieve course quiz completion percentage for a class (cached for 1 day)
 *
 * @param int $class_nid
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return int
 */
function opigno_statistics_app_query_class_quiz_completion_percentage($class_nid, $month_year, $filter_month){
  $cache_key = __FUNCTION__ . ':' . $class_nid . ':' . $month_year . ':' . $filter_month;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $percentage = $cached_object->data;
  } else {
    if($filter_month){
      $result = db_query('SELECT AVG(cd.status) FROM {opigno_statistics_user_course_details} cd
                          INNER JOIN opigno_statistics_user_course uc
                          ON cd.opigno_statistics_user_course_fk = uc.opigno_statistics_user_course_pk
                          WHERE uc.uid IN (
                            SELECT uid FROM {opigno_statistics_user_group} ug
                            WHERE ug.group_nid = :group_nid
                          )
                          AND cd.type = :type
                          AND DATE_FORMAT(FROM_UNIXTIME(cd.timestamp),\'%m-%Y\') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),\'%m-%Y\')', array(
        ':group_nid' => $class_nid,
        ':type' => 'quiz',
        ':timestamp' => $month_year
      ))->fetchCol();
    } else {
      $result = db_query('SELECT AVG(cd.status) FROM {opigno_statistics_user_course_details} cd
                          INNER JOIN opigno_statistics_user_course uc
                          ON cd.opigno_statistics_user_course_fk = uc.opigno_statistics_user_course_pk
                          WHERE uc.uid IN (
                            SELECT uid FROM {opigno_statistics_user_group} ug
                            WHERE ug.group_nid = :group_nid
                          )
                          AND cd.type = :type
                          AND DATE_FORMAT(FROM_UNIXTIME(cd.timestamp),\'%Y\') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),\'%Y\')', array(
        ':group_nid' => $class_nid,
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
 * Query total number of page view for a class (cached for 1 day)
 *
 * @param int $course_nid
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return array
 */
function opigno_statistics_app_query_class_total_number_of_page_view($class_nid, $month_year, $filter_month) {
  $cache_key = __FUNCTION__ . ':' . $class_nid . ':' . $month_year . ':' . $filter_month;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $total_number_of_page_view = $cached_object->data;
  } else {
    $total_number_of_page_view = array();


    // If the filter is for the current month, we use the accesslog table (because the accesslog is flushed every month)
    //   Else, we use the opigno_statistics_group table
    if ($filter_month && date('Y-m') == date('Y-m', $month_year)) {
      // First, get all the nodes that are in the courses that are in the current class
      $all_child_nids = opigno_statistics_app_query_class_child_nids($class_nid);
      if (empty($all_child_nids)) {
        return array();
      }


      // Construct the node ID filter
      $node_id_filter = '';
      foreach($all_child_nids as $nid_to_filter)
        $node_id_filter .= "path LIKE 'node/". $nid_to_filter ."%' OR ";
      $node_id_filter = substr($node_id_filter, 0, -4);


      // Then, get the number of pages view by the users of the class
      $nbr_pages_query = db_query("
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
          ':group_nid' => $class_nid,
          ':timestamp' => $month_year
      ));
    }
    // If the user selected no filter or filter another month than the current one, we use the statistics_group table
    else {
      $filter_date_format = ($filter_month ? '%m-%Y' : '%Y');

      $nbr_pages_query = db_query("
          SELECT DATE_FORMAT(FROM_UNIXTIME(month_year),'%Y-%m-%d') as day, page_views as value
          FROM {opigno_statistics_group}
          WHERE DATE_FORMAT(FROM_UNIXTIME(month_year), '". $filter_date_format ."') = DATE_FORMAT(FROM_UNIXTIME(:month_year), '". $filter_date_format ."')
          AND group_nid = :group_nid
        ", array(
          ':group_nid' => $class_nid,
          ':month_year' => $month_year
      ));
    }


    while($record = $nbr_pages_query->fetchAssoc()) $total_number_of_page_view[] = $record;
    cache_set($cache_key, $total_number_of_page_view, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $total_number_of_page_view;
}

/**
 * Query total number of interaction for a class (cached for 1 day)
 *
 * @param int $class_nid
 *
 * @return array
 */
function opigno_statistics_app_query_class_number_of_interactions($class_nid) {
  $cache_key = __FUNCTION__ . ':' . $class_nid;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $number_of_interactions = $cached_object->data;
  } else {
    $number_of_interactions = array();

    // Create the class childs filter
    $class_childs = opigno_statistics_app_query_class_child_nids($class_nid);
    $class_childs[] = $class_nid;

    $filter_class_childs = '(';
    foreach($class_childs as $class_child_nid) $filter_class_childs .= $class_child_nid .',';
    $filter_class_childs = substr($filter_class_childs, 0, -1);
    $filter_class_childs .= ')';


    $result = db_query("
      SELECT SUM(page_views) as number_of_interactions, j1.username, ROUND(j1.score, 0) as score
      FROM {opigno_statistics_user_group} ug
      INNER JOIN (
        SELECT AVG(score) as score, username, uid
        FROM {opigno_statistics_user_course} uc
        WHERE course_nid IN (
          SELECT opigno_class_courses_target_id
          FROM {field_data_opigno_class_courses} occ
          WHERE entity_id = :group_nid
        )
        AND uid IN (
          SELECT etid
          FROM {og_membership}
          WHERE group_type = 'node'
          AND gid = :group_nid
          AND entity_type = 'user'
        )
        GROUP BY uid
      ) j1 ON j1.uid = ug.uid
      WHERE ug.group_nid IN ". $filter_class_childs ."
      GROUP BY j1.uid
      ORDER BY score
      ", array(
        ':group_nid' => $class_nid
    ));

    while($record = $result->fetchAssoc()) $number_of_interactions[] = $record;
    cache_set($cache_key, $number_of_interactions, 'cache', time() + 7200); // 7200s = 2h cache
  }

  return $number_of_interactions;
}

/**
 * Query students results statistics for a class (cached for 1 day)
 *
 * @param int $class_nid
 *
 * @return array
 */
function opigno_statistics_app_query_class_students_results($class_nid) {
  $cache_key = __FUNCTION__ . ':' . $class_nid;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $students_results = $cached_object->data;
  } else {
    $students_results = array();


    $class_childs = opigno_statistics_app_query_class_child_nids($class_nid);
    $class_childs[] = $class_nid;

    $filter_class_childs = '(';
    foreach($class_childs as $class_child_nid) $filter_class_childs .= $class_child_nid .',';
    $filter_class_childs = substr($filter_class_childs, 0, -1);
    $filter_class_childs .= ')';


    $result = db_query("
      SELECT ROUND(AVG(uc.score), 0) as avg_score, j1.page_views as number_of_interactions, j1.uid, uc.username as student_name, COUNT(*) as tot_courses, SUM(status) as passed_courses
      FROM {opigno_statistics_user_course} uc
      INNER JOIN (
        SELECT SUM(page_views) as page_views, uid
          FROM {opigno_statistics_user_group} ug
          WHERE group_nid IN ". $filter_class_childs ."
          GROUP BY uid
      ) j1 ON j1.uid = uc.uid
      WHERE uc.uid IN (
          SELECT etid
          FROM {og_membership}
          WHERE group_type = 'node'
          AND gid = :group_nid
          AND entity_type = 'user'
      )
      AND course_nid IN ". $filter_class_childs ."
      GROUP BY j1.uid
    ", array(
      ':group_nid' => $class_nid
    ));


    $total_number_of_interactions = 0;
    $total_score = 0;
    while($record = $result->fetchAssoc()) {
      $record['status'] = ($record['tot_courses'] == $record['passed_courses'] ? true : false);

      $students_results[] = $record;
      $total_number_of_interactions += $record['number_of_interactions'];
      $total_score += $record['avg_score'];
    }

    $avg_number_of_interactions = (count($students_results)>0)? $total_number_of_interactions / count($students_results) : 0;
    $avg_score = (count($students_results)>0)? $total_score / count($students_results) : 0;

    foreach($students_results as &$student_result) {
      $student_result['avg_number_of_interactions'] = round($avg_number_of_interactions, 0);
      $student_result['general_avg_score'] = round($avg_score, 0);
    }

    cache_set($cache_key, $students_results, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $students_results;
}


/**
 * Get all the node IDs that are in the class (course IDs, lessons IDs of each courses of the class, etc)
 */
function opigno_statistics_app_query_class_child_nids($class_nid)
{
  $all_nids_query = db_query("
      SELECT etid, gid
      FROM {field_data_opigno_class_courses} occ
      INNER JOIN og_membership ogm ON occ.opigno_class_courses_target_id = ogm.gid
      WHERE occ.entity_id = :class_id AND ogm.entity_type = 'node'
    ", array(
      ':class_id' => $class_nid
  ));
  $all_nids = array();
  while($row = $all_nids_query->fetchAssoc()) { $all_nids[] = $row['etid']; $all_nids[] = $row['gid']; }
  return array_unique($all_nids);
}