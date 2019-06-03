<?php

require_once(drupal_get_path('module', 'opigno_statistics_app') .'/includes/group/course/queries.inc');

/**
 * Present user general info
 *
 * Output example:
 * array(
 *    'picture' => 'http://...'
 *    'name' => 'admin',
 *    'email' => 'admin@email.com',
 *    'date_joined' => 12312312414,
 *    'last_access' => 12313123323
 *  );
 *
 * @param int $uid
 *
 * @return array
 */
function opigno_statistics_app_query_user_general_informations($uid){
  $alive_user = user_load($uid);
  global $base_path;
  $default_picture_url = $base_path . drupal_get_path('module', 'opigno_statistics_app') . '/img/default_user_picture.jpg';
  if($alive_user) {
    $data = array(
      'picture' => (isset($alive_user->picture) && !empty($alive_user->picture)? image_style_url('user_thumbnail', $alive_user->picture->uri) : $default_picture_url),
      'name' => $alive_user->name,
      'email' => $alive_user->mail,
      'date_joined' => $alive_user->created,
      'last_access' => $alive_user->access
    );
  } else {
    $name=db_query('SELECT username FROM {opigno_statistics_user_course} WHERE uid = :uid LIMIT 1', array(
      ':uid' => $uid
    ))->fetchCol();
    $email=db_query('SELECT user_email FROM {opigno_statistics_login_history} WHERE uid = :uid ORDER BY last_login_count DESC LIMIT 1', array(
      ':uid' => $uid
    ))->fetchCol();
    $date_joined=db_query('SELECT last_login_count FROM {opigno_statistics_login_history} WHERE uid = :uid ORDER BY last_login_count DESC LIMIT 1', array(
      ':uid' => $uid
    ))->fetchCol();
    $last_access=db_query('SELECT timestamp FROM {accesslog} WHERE uid = :uid ORDER BY timestamp DESC LIMIT 1', array(
      ':uid' => $uid
    ))->fetchCol();
    $data = array(
      'picture' => $default_picture_url,
      'name' => $name[0],
      'email' => $email[0],
      'date_joined' => $date_joined[0],
      'last_access' => $last_access[0]
    );

    // TODO: Do a better sql request (you can do all these in one request)
  }
  return $data;
}

/**
 * Query badges earned for a user on filesystem (public://badges)
 *
 * Output example:
 * array(
 *    array('image' => 'http://opigno.com/sites/default/files/badges/badge_103_126/badge.png', 'title' => 'Badge best ...'),
 *    array('image' => 'http://opigno.com/sites/default/files/badges/badge_103_127/badge.png', 'title' => 'Badge best ... 2')
 *    array('image' => 'http://opigno.com/sites/default/files/badges/badge_103_156/badge.png', 'title' => 'Badge best ... 3')
 *    array('image' => 'http://opigno.com/sites/default/files/badges/badge_103_165/badge.png', 'title' => 'Badge best ... 4')
 *  );
 *
 * @param int $uid
 *
 * @return array
 */
function opigno_statistics_app_query_user_badges_earned($uid) {
  $badges_earned = array();
  $badges_folder = drupal_realpath('public://badges');
  $badge_assert_json_files = glob("{$badges_folder}/**/assert_{$uid}.json");

  foreach($badge_assert_json_files as $badge_assert_json_file){
    $badge_class_file = dirname($badge_assert_json_file) . '/class.json';
    $class_file_data = json_decode(file_get_contents($badge_class_file));
    $badges_earned[] = array('title' => $class_file_data->name, 'image' => $class_file_data->image);
  }
  return $badges_earned;
}


/**
 * Query total number of page view for a user (cached for 1 day)
 *
 * @param int $uid
 * @param int $month_year
 * @param boolean $filter_month
 *
 * @return array
 */
function opigno_statistics_app_query_user_total_number_of_page_view($uid, $month_year, $filter_month) {
  $cache_key = __FUNCTION__ . ':' . $uid . ':' . $month_year . ':' . $filter_month;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $total_number_of_page_view = $cached_object->data;
  }
  else {

    $total_number_of_page_view = array();
    if($filter_month && date('Y-m') == date('Y-m', $month_year)) {
      $result = db_query("
        SELECT DATE_FORMAT(FROM_UNIXTIME(timestamp),'%Y-%m-%d') as day, COUNT(*) as value
        FROM {accesslog}
        WHERE uid = :uid
        AND DATE_FORMAT(FROM_UNIXTIME(timestamp),'%m-%Y') = DATE_FORMAT(FROM_UNIXTIME(:timestamp),'%m-%Y')
        GROUP BY day
      ", array(
        ':uid' => $uid,
        ':timestamp' => $month_year
      ));
    }
    else {
      $filter_timestamp_format = ($filter_month ? '%m-%Y' : '%Y');

      $result = db_query("
        SELECT DATE_FORMAT(FROM_UNIXTIME(month_year),'%Y-%m-%d') as day, page_views as value
        FROM {opigno_statistics_user}
        WHERE uid = :uid
        AND DATE_FORMAT(FROM_UNIXTIME(month_year), '". $filter_timestamp_format ."') = DATE_FORMAT(FROM_UNIXTIME(:month_year), '". $filter_timestamp_format ."')
      ", array(
        ':uid' => $uid,
        ':month_year' => $month_year
      ));
    }

    while($record = $result->fetchAssoc()) $total_number_of_page_view[] = $record;
    cache_set($cache_key, $total_number_of_page_view, 'cache', time() + 7200); // 7200s = 2h cache

  }

  return $total_number_of_page_view;
}

/**
 * Query courses results statistics for a user (cached for 1 day)
 *
 * @param int $uid
 *
 * @return array
 */
function opigno_statistics_app_query_user_courses_results($uid) {
  $cache_key = __FUNCTION__ . ':' . $uid;
  $cached_object = cache_get($cache_key);
  if($cached_object) {
    $courses_results = $cached_object->data;
  } else {
    $courses_results = array();

    $result = db_query("
      SELECT course_name, score, course_nid, status
      FROM {opigno_statistics_user_course} uc
      WHERE uc.uid = :uid
    ", array(
      ':uid' => $uid
    ));


    while($record = $result->fetchAssoc()) {
      $averages = opigno_statistics_app_query_course_students_results($record['course_nid']);
      foreach($averages as $average)
        if ($average['uid'] == $uid) {$averages = $average; break;}

      $record['avg_number_of_interactions'] = $averages['avg_number_of_interactions'];
      $record['avg_score'] = $averages['avg_score'];
      $record['number_of_interactions'] = $averages['number_of_interactions'];

      $courses_results[] = $record;
    }

    cache_set($cache_key, $courses_results, 'cache', time() + 7200); // 7200s = 2h cache
  }
  return $courses_results;
}
