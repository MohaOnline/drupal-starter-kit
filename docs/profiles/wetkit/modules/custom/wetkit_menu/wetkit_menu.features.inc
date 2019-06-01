<?php
/**
 * @file
 * wetkit_menu.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wetkit_menu_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "panels_mini" && $api == "panels_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
