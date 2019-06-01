<?php
/**
 * @file
 * wetkit_bean.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wetkit_bean_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "bean_admin_ui" && $api == "bean") {
    return array("version" => "5");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function wetkit_bean_image_default_styles() {
  $styles = array();

  // Exported image style: highlighted.
  $styles['highlighted'] = array(
    'effects' => array(
      3 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 380,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 2,
      ),
      4 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 380,
          'height' => 250,
        ),
        'weight' => 2,
      ),
    ),
    'label' => 'highlighted',
  );

  return $styles;
}
