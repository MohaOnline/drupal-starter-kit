<?php
/**
 * @file
 * wetkit_deployment_source.pm_existing_pages.inc
 */

/**
 * Implements hook_pm_existing_pages_info().
 */
function wetkit_deployment_source_pm_existing_pages_info() {
  $export = array();

  $pm_existing_page = new stdClass();
  $pm_existing_page->api_version = 1;
  $pm_existing_page->name = 'deploy';
  $pm_existing_page->label = 'Deploy';
  $pm_existing_page->context = '';
  $pm_existing_page->paths = 'admin/structure/deploy';
  $export['deploy'] = $pm_existing_page;

  return $export;
}
