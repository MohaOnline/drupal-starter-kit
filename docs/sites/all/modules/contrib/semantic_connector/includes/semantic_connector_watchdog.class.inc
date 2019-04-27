<?php
/**
 * @file
 * The Exception Classes.
 */

/**
 * The Exception Class for the Semantic Connector.
 */
class SemanticConnectorWatchdog {

  /**
   * Puts the message into the watchdog and as drupal message if is set.
   *
   * @param string $type
   * @param string $message
   * @param array $variables
   * @param int $severity
   * @param bool $showMessage
   */
  public static function message($type, $message, $variables = array(), $severity = WATCHDOG_ERROR, $showMessage = FALSE) {
    watchdog('PoolParty Semantic Connector - ' . $type, $message, $variables, $severity);
    if ($showMessage) {
      switch ($severity) {
        case WATCHDOG_EMERGENCY:
        case WATCHDOG_ALERT:
        case WATCHDOG_CRITICAL:
        case WATCHDOG_ERROR:
          $type = 'error';
          break;

        case WATCHDOG_WARNING:
          $type = 'warning';
          break;

        case WATCHDOG_NOTICE:
        case WATCHDOG_INFO:
        case WATCHDOG_DEBUG:
        default:
          $type = 'status';
      }
      drupal_set_message(t($message, $variables), $type);
    }
  }
}
