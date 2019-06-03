<?php

namespace Drupal\polling;

/**
 * Common interface for all global polling plugins.
 */
interface GlobalPluginInterface {
  /**
   * Create a new instance of this plugin.
   */
  public static function instance();

  /**
   * Get the plugins data for the current node.
   *
   * @return array
   *   Data to be returned for this plugin.
   */
  public function getData();
}
