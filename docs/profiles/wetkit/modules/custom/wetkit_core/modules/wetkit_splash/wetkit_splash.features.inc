<?php
/**
 * @file
 * wetkit_splash.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wetkit_splash_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
