<?php
/**
 * @file
 * shared_content_example_destination.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function shared_content_example_destination_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
