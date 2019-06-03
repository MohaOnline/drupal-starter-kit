<?php
/**
 * @file
 * dvg_voc_owms_uniform_product_name.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_voc_owms_uniform_product_name_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
