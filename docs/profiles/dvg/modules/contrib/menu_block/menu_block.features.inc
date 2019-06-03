<?php

define('MENU_BLOCK_FEATURES_PREFIX', 'menu_block__');

function _menu_block_features_real_delta($delta) {
  $strlen = strlen(MENU_BLOCK_FEATURES_PREFIX);
  return strpos($delta, MENU_BLOCK_FEATURES_PREFIX) === 0 ? substr($delta, $strlen) : $delta;
}

/**
 * Implements hook_features_export_options().
 */
function menu_block_features_export_options() {
  module_load_include('inc', 'menu_block', 'menu_block.admin');
  $deltas = variable_get('menu_block_ids', array());

  // Less simple.
  $blocks = array();
  foreach ($deltas as $delta) {
    $config = menu_block_get_config($delta);
    $blocks[MENU_BLOCK_FEATURES_PREFIX . $delta] = _menu_block_format_title($config);
  }

  ksort($blocks);
  return $blocks;
}

/**
 * Implements hook_features_export().
 */
function menu_block_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['features'] = 'features';
  $export['dependencies']['menu_block'] = 'menu_block';
  $export['features']['menu_block'] = $data;

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function menu_block_features_export_render($module, $data) {
  ksort($data);

  $code = array();
  foreach ($data as $delta) {
    $real_delta = _menu_block_features_real_delta($delta);
    $config = menu_block_get_config($real_delta);
    $config['exported_to_code'] = TRUE;
    $code[$delta] = $config;
  }

  $code = "  return ". features_var_export($code, '  ') .";";

  return array('menu_block_features_default' => $code);
}

/**
 * Implements hook_features_revert().
 */
function menu_block_features_revert($module) {
  menu_block_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function menu_block_features_rebuild($module) {
  $blocks = module_invoke($module, 'menu_block_features_default');

  $block_ids = variable_get('menu_block_ids', array());
  $block_ids = array_combine($block_ids, $block_ids);
  foreach ($blocks as $delta => $block) {
    $real_delta = _menu_block_features_real_delta($delta);

    unset($block['exported_to_code']);
    $block['parent'] = $block['menu_name'] . ':' . $block['parent_mlid'];
    menu_block_block_save($real_delta, $block);

    $block_ids[$real_delta] = $real_delta;
  }
  variable_set('menu_block_ids', array_values($block_ids));
}
