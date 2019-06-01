<?php
/**
 * @file
 * wetkit_deployment_environment.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wetkit_deployment_environment_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "environment_indicator" && $api == "default_environment_indicator_environments") {
    return array("version" => "1");
  }
}
