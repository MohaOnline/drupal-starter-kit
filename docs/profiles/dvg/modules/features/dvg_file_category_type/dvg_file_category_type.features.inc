<?php
/**
 * @file
 * dvg_file_category_type.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_file_category_type_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "file_entity" && $api == "file_default_displays") {
    return array("version" => "1");
  }
  if ($module == "file_entity" && $api == "file_type") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function dvg_file_category_type_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}
