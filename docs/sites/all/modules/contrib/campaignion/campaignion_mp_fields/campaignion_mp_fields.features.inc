<?php
/**
 * @file
 * campaignion_mp_fields.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function campaignion_mp_fields_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
