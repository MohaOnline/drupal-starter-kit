<?php

namespace Drupal\openlayers;

/**
 * FIX - Insert short comment here.
 */
class Config {

  /**
   * Get default configuration.
   *
   * @param string $key
   *   Key to get. If not provided, returns the full array.
   *
   * @return array|null
   *   Returns the array or if a key is provided, it's value.
   */
  protected static function defaults($key = NULL) {
    $defaults = array(
      'openlayers.js_css.group' => 'openlayers',
      'openlayers.js_css.weight' => 20,
      'openlayers.js_css.media' => 'screen',
      'openlayers.edit_view_map' => 'openlayers_map_view_edit_form',
      'openlayers.debug' => 0,
    );
    if ($key == NULL) {
      return $defaults;
    }

    return isset($defaults[$key]) ? $defaults[$key] : NULL;
  }

  /**
   * Fetches a configuration value.
   *
   * @param string|array $parents
   *   The path to the configuration value. Strings use dots as path separator.
   * @param string|array $default_value
   *   The default value to use if the config value isn't set.
   *
   * @return mixed
   *   The configuration value.
   */
  public static function get($parents, $default_value = NULL) {
    $options = \OpenlayersDrupal::service('variable')->get('openlayers_config', array());

    if (is_string($parents)) {
      $parents = explode('.', $parents);
    }

    if (is_array($parents)) {
      $notfound = FALSE;
      foreach ($parents as $parent) {
        if (array_key_exists($parent, $options)) {
          $options = $options[$parent];
        }
        else {
          $notfound = TRUE;
          break;
        }
      }
      if (!$notfound) {
        return $options;
      }
    }

    $value = Config::defaults(implode('.', $parents));
    if (isset($value)) {
      return $value;
    }

    if (is_null($default_value)) {
      return FALSE;
    }

    return $default_value;
  }

  /**
   * Sets a configuration value.
   *
   * @param string|array $parents
   *   The path to the configuration value. Strings use dots as path separator.
   * @param mixed $value
   *   The  value to set.
   *
   * @return array
   *   The configuration array.
   */
  public static function set($parents, $value) {
    $config = \OpenlayersDrupal::service('variable')->get('openlayers_config', array());

    if (is_string($parents)) {
      $parents = explode('.', $parents);
    }

    $ref = &$config;
    foreach ($parents as $parent) {
      if (isset($ref) && !is_array($ref)) {
        $ref = array();
      }
      $ref = &$ref[$parent];
    }
    $ref = $value;

    \OpenlayersDrupal::service('variable')->set('openlayers_config', $config);
    return $config;
  }

  /**
   * Removes a configuration value.
   *
   * @param string|array $parents
   *   The path to the configuration value. Strings use dots as path separator.
   *
   * @return array
   *   The configuration array.
   */
  public static function clear($parents) {
    $config = \Drupal::service('variable')->get('openlayers_config', array());
    $ref = &$config;

    if (is_string($parents)) {
      $parents = explode('.', $parents);
    }

    $last = end($parents);
    reset($parents);
    foreach ($parents as $parent) {
      if (isset($ref) && !is_array($ref)) {
        $ref = array();
      }
      if ($last == $parent) {
        unset($ref[$parent]);
      }
      else {
        if (isset($ref[$parent])) {
          $ref = &$ref[$parent];
        }
        else {
          break;
        }
      }
    }
    \OpenlayersDrupal::service('variable')->set('openlayers_config', $config);
    return $config;
  }

}
