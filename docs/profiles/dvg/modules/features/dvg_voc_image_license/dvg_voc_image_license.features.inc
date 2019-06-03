<?php
/**
 * @file
 * dvg_voc_image_license.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_voc_image_license_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
