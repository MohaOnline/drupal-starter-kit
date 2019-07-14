<?php

namespace Drupal\pgbar\Source;

/**
 * Interface for pgbar source plugins.
 */
interface PluginInterface {
  /**
   * Return a properly translated label for this plugin.
   */
  public static function label();

  /**
   * Static constructor: Get plugin instance given entity and field instance.
   */
  public static function forField($entity, $field, $instance);

  /**
   * Get the current count given a field item.
   *
   * @param array
   *   A field item.
   *
   * @return int
   *   The current count.
   */
  public function getValue($item);

  /**
   * Additional fields for the field witget for if needed.
   *
   * @param array
   *   A field item containing the current values.
   *
   * @return array OR NULL
   *   A partial form-array containing the form-fields for this plugin.
   *   Otherwise NULL if no additional form fields should be displayed.
   */
  public function widgetForm($item);
}
