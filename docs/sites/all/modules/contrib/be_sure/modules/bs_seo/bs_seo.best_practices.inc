<?php

/**
 * @file
 * Best practices part of SEO.
 */

/**
 * Check availability of Search 404.
 */
function bs_seo_best_practices_search404() {
  return module_exists('search404');
}

/**
 * Check availability of Google news sitemap.
 */
function bs_seo_best_practices_googlenews() {
  return module_exists('googlenews');
}

/**
 * Check availability of Google Analytics.
 */
function bs_seo_best_practices_gag() {
  return module_exists('googleanalytics') && drupal_strlen(variable_get('googleanalytics_account')) > 3;
}

/**
 * Check availability of cache Google analytics.
 */
function bs_seo_best_practices_gag_cache() {
  return bs_seo_best_practices_gag() && variable_get('googleanalytics_cache');
}

/**
 * Check availability of Site Verification.
 */
function bs_seo_best_practices_site_verify() {
  return module_exists('site_verify');
}
