<?php
/**
 * @file
 * dvg_voc_webform_button_title.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_voc_webform_button_title_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
