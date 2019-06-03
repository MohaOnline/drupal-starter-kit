<?php
/**
 * @file
 * campaignion_webform_widget.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function campaignion_webform_widget_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}
