<?php

/**
 * @file
 */

define('API_VERSION', 'A');

/**
 * Generate a new api key for this user and put it into the database
 * @return the new API key or FALSE on database insert error
 */
function shurly_generate_new_api_key($uid) {
  // be sure that the new key is unique
  do {
    $key = md5(uniqid(rand(), TRUE)) . '_' . API_VERSION;
  } while (shurly_get_uid($key));
  db_query('DELETE FROM {shurly_keys} WHERE uid = :uid', array('uid' => $uid));
  $result = db_query("INSERT INTO {shurly_keys} (uid, apikey) VALUES (:uid, :key)", array('uid' => $uid, 'key' => $key));
  return $result ? $key : FALSE;
}

function shurly_get_uid($key) {
  return db_query("SELECT uid FROM {shurly_keys} WHERE apikey = :key", array('key' => $key))->fetchField();
}

function shurly_get_api_key($uid) {
  return db_query("SELECT apikey FROM {shurly_keys} WHERE uid = :uid", array('uid' => $uid))->fetchField();
}

