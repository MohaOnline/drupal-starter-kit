<?php

/**
 * @file
 * User experience part of SEO.
 */

/**
 * Check availability of Site map.
 */
function bs_seo_user_experience_site_map() {
  return module_exists('site_map');
}

/**
 * Check availability of Scheduler.
 */
function bs_seo_user_experience_scheduler() {
  return module_exists('scheduler');
}

/**
 * Check availability of HTML Purifier.
 */
function bs_seo_user_experience_html_purifier() {
  return module_exists('htmlpurifier');
}

/**
 * Check availability of Redirect.
 */
function bs_seo_user_experience_redirect() {
  return module_exists('redirect');
}

/**
 * Check availability of Global Redirect.
 */
function bs_seo_user_experience_global_redirect() {
  return module_exists('globalredirect');
}