<?php
/**
 * @file
 * dvg_piwik.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_piwik_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
