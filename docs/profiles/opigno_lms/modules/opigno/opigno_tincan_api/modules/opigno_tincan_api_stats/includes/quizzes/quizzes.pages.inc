<?php

/**
 * @file
 * Quizzes pages
 */

require_once(__DIR__ . '/model.inc');

/**
 * Display quizzes statistics page
 */
function opigno_lrs_stats_quizzes_page() {
  $module_path = drupal_get_path('module', 'opigno_tincan_api_stats');
  drupal_add_js("{$module_path}/js/jquery.tablesorter.min.js");
  drupal_add_js("{$module_path}/js/script.js");
  drupal_add_css("{$module_path}/css/styles.css", 'theme');
  return theme('opigno_lrs_stats_quizzes', array('quizzes' => opigno_lrs_stats_quizzes_quizzes_statistics()));
}