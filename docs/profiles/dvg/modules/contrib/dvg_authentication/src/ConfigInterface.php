<?php

namespace Drupal\dvg_authentication;

/**
 * Defines a common config interface for plugins handling settings.
 */
interface ConfigInterface {

  /**
   * Get a setting from the plugin configuration.
   *
   * @param string $setting
   *   Name of the setting to fetch.
   *
   * @return mixed|null
   *   The value or NULL if the setting doesn't exist.
   */
  public function getConfig($setting);

}
