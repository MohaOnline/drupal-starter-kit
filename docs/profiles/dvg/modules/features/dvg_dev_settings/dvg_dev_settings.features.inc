<?php
/**
 * @file
 * dvg_dev_settings.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_dev_settings_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
