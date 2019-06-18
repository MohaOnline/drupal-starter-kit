<?php
/**
 * @file
 * survey_builder_ui.features.inc
 */

/**
 * Implements hook_views_api().
 */
function survey_builder_ui_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
