<?php
define('OPIGNO_STATISTICS_APP_MAX_LOGIN_HISTORY_RECORDS', 31556940); //1 year by default

function _opigno_statistics_app_login_history_clear_old_records() {
  $current_timestamp = time();
  $clear_timestamp_older_than = $current_timestamp - OPIGNO_STATISTICS_APP_MAX_LOGIN_HISTORY_RECORDS;
  $num_deleted = db_delete('login_history')
    ->condition('login', $clear_timestamp_older_than, '<')
    ->execute();
  return $num_deleted;
}

function _opigno_statistics_app_login_history_insert($user) {
  db_merge('opigno_statistics_login_history')
    ->key(array('uid' => $user->uid))
    ->fields(array(
      'user_name' => $user->name,
      'user_email' => $user->mail,
      'last_login_count' => time(),
    ))
    ->expression('login_count', 'login_count + :inc', array(':inc' => 1))
    ->execute();
}
