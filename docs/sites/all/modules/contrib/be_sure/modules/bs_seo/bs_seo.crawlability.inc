<?php

/**
 * @file
 * Crawlability part of SEO.
 */

/**
 * Check availability of XML Sitemap.
 */
function bs_seo_crawlability_xmlsitemap() {
  return module_exists('xmlsitemap');
}

/**
 * Check availability of RobotsTxt.
 */
function bs_seo_crawlability_robotstxt() {
  return module_exists('robotstxt');
}
