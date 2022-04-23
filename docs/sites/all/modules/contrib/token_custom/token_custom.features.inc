<?php

/**
 * @file
 * Features integration for the Custom tokens module.
 */

/**
 * Implements hook_features_export_options().
 */
function token_custom_features_export_options() {
  $options = array();
  $data = token_custom_features_config_get_tokens();
  foreach ($data as $key) {
    $options[$key->machine_name] = $key->machine_name;
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function token_custom_features_export($data, &$export, $module_name) {
  // We have module dependencies in order for this module to function properly
  // so we'll add them here.
  $export['dependencies']['token_custom'] = 'token_custom';

  // The following is the simplest implementation of a straight object export
  // with no further export processors called.
  foreach ($data as $component) {
    $export['features']['token_custom'][$component] = $component;
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function token_custom_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $i = 0;
  foreach ($data as $component) {
    // Here is just a variable_get, in other cases, it could be a query!
    $token_obj = token_custom_features_config_get_machinename($component);
    if ($token_obj) {
      foreach ($token_obj as $key => $value) {
        $code[$i][$key] = $value;
      }
    }
    $i++;
  }
  $code = "  return " . features_var_export($code, '  ') . ";";
  return array('token_custom_features_default_settings' => $code);
}

/**
 * Implements hook_features_revert().
 */
function token_custom_features_revert($module = NULL) {
  token_custom_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function token_custom_features_rebuild($module) {
  $items = module_invoke($module, 'token_custom_features_default_settings');
  foreach ($items as $value) {
    try {
      $result = token_custom_features_config_get_machinename($value);
      $new = $result ? FALSE : TRUE;
      drupal_write_record('token_custom', $value, $new ? array() : 'machine_name');
    }
    catch (Exception $e) {
      watchdog("Error in updating", $e->getMessage(), array(), WATCHDOG_ERROR);
    }
  }
}

/**
 * Get machine name of all custom tokens.
 *
 * @return array
 *   An associative array, or FALSE if there is no next row.
 */
function token_custom_features_config_get_tokens() {
  $codes = db_select('token_custom', 'tc')
    ->fields('tc', array())
    ->execute();
  return $codes->fetchAll();
}

/**
 * Get machine name specific data.
 *
 * @param string $name
 *   Machine name from token_custom table.
 *
 * @return array
 *   An associative array.
 */
function token_custom_features_config_get_machinename($name) {
  $codes = db_select('token_custom', 'tc')
    ->fields('tc', array())
    ->condition('machine_name', $name)
    ->execute();
  return $codes->fetchAssoc();
}
