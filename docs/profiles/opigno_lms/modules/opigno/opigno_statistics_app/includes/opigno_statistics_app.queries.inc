<?php

/**
 * Retrieve course AVG score (cached for 1 day)
 *
 * @param int $course_nid
 *
 * @return float
 */
function opigno_statistics_app_query_course_avg_score($course_nid){
  $cache_key = __FUNCTION__ . ':' . $course_nid;
  $cached_object = cache_get($cache_key);
  if($cached_object)
  {
    $avg_score = $cached_object->data;
  }
  else
  {
    $avg_score = db_query('SELECT AVG(score) FROM {opigno_statistics_user_course} WHERE course_nid = :course_nid', array(
      ':course_nid' => $course_nid
    ))->fetchField();
    cache_set($cache_key, $avg_score, 'cache', time() + 7200); // 7200s = 2h cache
  }

  return $avg_score;
}

/**
 * Update user course status
 * All latest required quizzes must passed to pass the course
 *
 * @param int $user_course_id
 *
 * @return int (id of new record)
 */
function opigno_statistics_app_query_user_course_update_score($user_course_id) {
  $avg_score = db_query('SELECT AVG(score) FROM {opigno_statistics_user_course_details} WHERE opigno_statistics_user_course_fk = :opigno_statistics_user_course_fk', array(
    ':opigno_statistics_user_course_fk' => $user_course_id
  ))->fetchField();

  if(!$avg_score) $avg_score = 0; //No course details

  return db_update('opigno_statistics_user_course')
    ->fields(array(
      'score' => $avg_score,
      'timestamp' => time()
    ))
    ->condition('opigno_statistics_user_course_pk', $user_course_id)
    ->execute();
}

/**
 * Store user course detail record in the database only if best score
 *
 * @param int $uid
 * @param int $course_id
 * @param string $type
 * @param int $entity_id
 * @param string $entity_name
 * @param int $score
 * @param int $status
 * @param int $required
 */
function opigno_statistics_app_query_user_course_details_insert_or_update_best_score($user_course_id, $type, $entity_id, $entity_name, $score, $status, $required) {
  $record = db_query('SELECT opigno_statistics_user_course_details_pk, score FROM {opigno_statistics_user_course_details} WHERE entity_id = :entity_id AND opigno_statistics_user_course_fk = :opigno_statistics_user_course_fk', array(
    ':entity_id' => $entity_id,
    ':opigno_statistics_user_course_fk' => $user_course_id
  ))
    ->fetchAssoc();

  $old_score = $record['score'];
  if(isset($old_score)) {
    if($score >= $old_score) {
      $user_course_details_id = $record['opigno_statistics_user_course_details_pk'];
      db_update('opigno_statistics_user_course_details')
        ->fields(array(
          'score' => $score,
          'status' => $status,
          'required' => $required,
          'timestamp' => time()
        ))
        ->condition('opigno_statistics_user_course_details_pk', $user_course_details_id)
        ->execute();
    }
  }
  else
  {
    db_insert('opigno_statistics_user_course_details')
      ->fields(array(
        'opigno_statistics_user_course_fk' => $user_course_id,
        'type' => $type,
        'entity_id' => $entity_id,
        'entity_name' => $entity_name,
        'score' => $score,
        'status' => $status,
        'required' => $required,
        'timestamp' => time()
      ))
      ->execute();
  }
}

/**
 * Find user course id associated with the specified user id and course id
 *
 * @param int $uid
 * @param int $course_nid
 *
 * @return mixed (int if found otherwise FALSE)
 */
function opigno_statistics_app_query_user_course_find_id_by_uid_and_course_nid($uid, $course_nid) {
  return db_query('SELECT opigno_statistics_user_course_pk FROM {opigno_statistics_user_course} WHERE uid = :uid and course_nid = :course_nid', array(
    ':uid' => $uid,
    ':course_nid' => $course_nid
  ))->fetchField();
}

/**
 * Store user course record in the database
 *
 * @param int $uid
 * @param string $username
 * @param int $course_id
 * @param int $status
 * @param string $course_name
 * @param int $score
 *
 * @return int (id of new record)
 */
function opigno_statistics_app_query_user_course_insert($uid, $username, $course_nid, $status, $course_name, $score) {
  return db_insert('opigno_statistics_user_course')
    ->fields(array(
      'uid' => $uid,
      'username' => $username,
      'course_nid' => ($course_nid == null ? 0 : $course_nid),
      'status' => $status,
      'course_name' => ($course_name == false ? '' : $course_name),
      'score' => $score,
      'timestamp' => time()
    ))
    ->execute();
}

/**
 * Get start of last week timestamp
 *
 * @return int
 */
function opigno_statistics_app_start_of_last_week() {
  return strtotime('Midnight Last Monday - 1 week');
}

/**
 * Get end of last week timestamp
 *
 * @return int
 */
function opigno_statistics_app_end_of_last_week() {
  return strtotime('Last Sunday, 23:59:59');
}


/**
 * Get the course status for a user (status = if the user has passed or not)
 */
function opigno_statistics_app_query_status_from_course_and_user($course_nid, $user_uid)
{
  return db_query("
    SELECT status
    FROM {opigno_statistics_user_course}
    WHERE uid = :uid
    AND course_nid = :nid
    ORDER BY timestamp
    DESC
    LIMIT 1
  ", array(
      ':uid' => $user_uid,
      ':nid' => $course_nid
  ))->fetchField();
}