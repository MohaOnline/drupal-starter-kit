<?php

/**
 * @file
 * Checking callback of sure items.
 */

/**
 * Check availability of Views Litepager.
 */
function bs_performance_module_litepager() {
  return module_exists('views_litepager');
}

/**
 * Check availability of Fast 404.
 */
function bs_performance_module_fast_404() {
  return module_exists('fast_404');
}

/**
 * Check availability of Speedy.
 */
function bs_performance_module_speedy() {
  return module_exists('speedy');
}

/**
 * Module Mobile Tools should be disabled.
 */
function bs_performance_module_mobile_tools() {
  return !module_exists('mobile_tools');
}

/**
 * Module Performance Logging and Monitoring should be disabled.
 */
function bs_performance_module_plam() {
  return !module_exists('performance');
}


/**
 * Module Database logging should be disabled.
 */
function bs_performance_module_dblog() {
  return !module_exists('dblog');
}

/**
 * Module Theme Developer should be disabled.
 */
function bs_performance_module_tdeveloper() {
  return !module_exists('devel_themer');
}

/**
 * Module Theme Developer should be disabled.
 */
function bs_performance_module_update() {
  return !module_exists('update');
}

/**
 * Module Backup and Migrate should be disabled.
 */
function bs_performance_module_backup() {
  return !module_exists('backup_migrate');
}

/**
 * Module Coder should be disabled.
 */
function bs_performance_module_coder() {
  return !module_exists('coder');
}

/**
 * Module SimpleTests should be disabled.
 */
function bs_performance_module_simpletest() {
  return !module_exists('simpletest');
}

/**
 * Check availability of Image Resize Filter.
 */
function bs_performance_module_image_resize_filter() {
  return module_exists('image_resize_filter');
}

/**
 * Check availability of ImageAPI Optimize (or Image Optimize).
 */
function bs_performance_module_imageapi_optimize() {
  return module_exists('imageapi_optimize');
}
