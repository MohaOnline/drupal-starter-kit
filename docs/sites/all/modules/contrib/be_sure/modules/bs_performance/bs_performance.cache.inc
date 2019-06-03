<?php

/**
 * @file
 * Cache part of performance.
 */

/**
 * Check availability of Page caching.
 */
function bs_performance_cache_caching() {
  return variable_get('cache');
}

/**
 * Check availability of Page caching.
 */
function bs_performance_cache_entitycache() {
  return module_exists('entitycache');
}

/**
 * Check availability of Block caching.
 */
function bs_performance_cache_block() {
  return variable_get('block_cache');
}

/**
 * Check availability of Varnish.
 */
function bs_performance_cache_varnish() {
  if (module_exists('varnish')) {
    $statuses = varnish_get_status();

    foreach ($statuses as $status) {
      if (!$status) {
        return FALSE;
      }
    }

    return TRUE;
  }

  return FALSE;
}

/**
 * Module File cache should be disabled.
 */
function bs_performance_cache_file_cache() {
  return !module_exists('filecache');
}

/**
 * Check availability of Expire module.
 */
function bs_performance_cache_expire() {
  return module_exists('expire');
}

/**
 * Check minimum cache lifetime.
 */
function bs_performance_cache_min_lifetime() {
  return variable_get('cache_lifetime', 0) > 0;
}

/**
 * Check cache max age.
 */
function bs_performance_cache_max_age() {
  return variable_get('page_cache_maximum_age', 0) >= 300;
}

/**
 * Check availability of Memcache or Redis.
 */
function bs_performance_cache_memcache_redis() {
  return module_exists('memcache') || module_exists('redis') || module_exists('memcache_storage');
}
