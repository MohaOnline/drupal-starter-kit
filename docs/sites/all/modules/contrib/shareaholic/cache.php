<?php
/**
 * File for the ShareaholicCache class.
 *
 * @package shareaholic
 */

/**
 * A simple wrapper around the Drupal caching library
 * because it is incapable of respecting the expire time
 *
 * @package shareaholic
 */
class ShareaholicCache {

  /**
   * Get the cached item based on the key
   *
   * @param string $key the key to get the cache value for
   * @return mixed the data cached or false if not found
   */
  public static function get($key) {
    $cache = cache_get($key);
    // check if the cache is stale
    if (!$cache) {
      return FALSE;
    }

    if (REQUEST_TIME > $cache->expire) {
      return FALSE;
    }

    return $cache->data;
  }


  /**
   * Updates the cache value given a key and expire time
   *
   * @param string $key the key for the cached data
   * @param mixed $data the data to cache
   * @param integer $expire the number of seconds to cache the data for
   */
  public static function set($key, $data, $expire) {
    $expire_time = REQUEST_TIME + $expire;
    cache_set($key, $data, 'cache', $expire_time);
  }



}