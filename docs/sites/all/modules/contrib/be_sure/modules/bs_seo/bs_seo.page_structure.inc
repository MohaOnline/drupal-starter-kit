<?php

/**
 * @file
 * Page structure part of SEO.
 */

/**
 * Check availability of Page Title.
 */
function bs_seo_page_structure_page_title() {
  return module_exists('page_title');
}

/**
 * Check availability of Clean URLs.
 */
function bs_seo_page_structure_clean_urls() {
  return variable_get('clean_url');
}

/**
 * Check availability of Pathauto.
 */
function bs_seo_page_structure_pathauto() {
  return module_exists('pathauto');
}

/**
 * Check availability of Metatag.
 */
function bs_seo_page_structure_metatag() {
  return module_exists('metatag');
}

/**
 * Check availability of Microdata.
 */
function bs_seo_page_structure_microdata() {
  return module_exists('microdata');
}

/**
 * Check availability of Keyword Research.
 */
function bs_seo_page_structure_kwresearch() {
  return module_exists('kwresearch');
}
