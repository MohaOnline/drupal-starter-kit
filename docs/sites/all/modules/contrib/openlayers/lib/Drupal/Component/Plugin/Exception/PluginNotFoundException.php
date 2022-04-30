<?php

namespace OpenlayersDrupal\Component\Plugin\Exception;

/**
 * Plugin exception class to be thrown when a plugin ID could not be found.
 */
class PluginNotFoundException extends PluginException {

  /**
   * Construct an PluginNotFoundException exception.
   *
   * @param string $plugin_id
   *   The plugin ID that was not found.
   * @param string $message
   *   FIX - insert comment here.
   * @param int $code
   *   FIX - insert comment here.
   * @param \Exception $previous
   *   FIX - insert comment here.
   */
  public function __construct($plugin_id, $message = '', $code = 0, \Exception $previous = NULL) {
    if (empty($message)) {
      $message = sprintf("Plugin ID '%s' was not found.", $plugin_id);
    }
    parent::__construct($message, $code, $previous);
  }

}
