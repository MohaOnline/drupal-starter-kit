<?php

/**
 * @file
 * Other part of performance.
 */

/**
 * Check last cron ran.
 */
function bs_performance_other_last_cron() {
  return variable_get('cron_last') > (REQUEST_TIME - 86400);
}

/**
 * Check availability of CSS optimization.
 */
function bs_performance_other_css() {
  return variable_get('preprocess_css');
}

/**
 * Check page compression.
 */
function bs_performance_other_page_compression() {
  return variable_get('page_compression');
}

/**
 * Check availability of JavaScript optimization.
 */
function bs_performance_other_js() {
  return variable_get('preprocess_js');
}

/**
 * Check availability of PHP APC.
 */
function bs_performance_other_php_apc() {
  return ini_get('apc.enabled');
}

/**
 * Check PHP execution time.
 */
function bs_performance_other_max_execution() {
  return ini_get('max_execution_time') < 300;
}

/**
 * Check availability of Advanced CSS/JS Aggregation.
 */
function bs_performance_other_advagg() {
  return module_exists('advagg') && module_exists('advagg_css_compress') && module_exists('advagg_js_compress');
}
