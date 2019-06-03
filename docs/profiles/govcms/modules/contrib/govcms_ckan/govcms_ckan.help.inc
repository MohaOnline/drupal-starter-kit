<?php
/**
 * @file
 * Help functionality.
 */

/**
 * Help page callback for govCMS CKAN modules.
 *
 * @param string $module
 *   Module providing the help files.
 * @param string $page
 *   The key for the help file.
 */
function govcms_ckan_help_callback($module, $page) {
  $page = drupal_get_path('module', $module) . '/help/' . $page . '.html';
  $page_content = '';
  if (file_exists($page)) {
    $page_content = file_get_contents($page);
  }
  print theme('govcms_ckan_help_page', array('page_content' => $page_content));
  exit;
}
