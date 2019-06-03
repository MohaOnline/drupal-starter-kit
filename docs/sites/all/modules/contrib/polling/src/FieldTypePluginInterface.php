<?php

namespace Drupal\polling;

/**
 * Common interface for all field polling plugins.
 */
interface FieldTypePluginInterface {
  /**
   * Create a new instance of this plugin.
   */
  public static function instance($entity, $field, $instance);

  /**
   * Get the plugins data for the current node.
   *
   * @return array
   *   Data to be returned for this plugin.
   */
  public function getData();
}
