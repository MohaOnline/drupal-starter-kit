<?php

namespace Drupal\openlayers\Flood;

use OpenlayersDrupal\Core\Database\Connection;
use OpenlayersDrupal\Core\Flood\FloodInterface;
use Drupal\openlayers\Legacy\Drupal7;

/**
 * Defines the database flood backend. This is the default Drupal backend.
 *
 * @codeCoverageIgnore
 */
class LegacyBackend implements FloodInterface {

  /**
   * The database connection used to store flood event information.
   *
   * @var \OpenlayersDrupal\Core\Database\Connection
   */
  protected $connection;

  /**
   * The Drupal 7 legacy service.
   *
   * @var \Drupal\openlayers\Legacy\Drupal7
   */
  protected $drupal7;

  /**
   * Construct the DatabaseBackend.
   *
   * @param \OpenlayersDrupal\Core\Database\Connection $connection
   *   The database connection which will be used to store the flood event
   *   information.
   * @param \Drupal\openlayers\Legacy\Drupal7 $drupal7
   *   The Drupal 7 legacy service.
   */
  public function __construct(Connection $connection, Drupal7 $drupal7) {
    $this->connection = $connection;
    $this->drupal7 = $drupal7;
  }

  /**
   * Implements OpenlayersDrupal\Core\Flood\FloodInterface::register().
   *
   * Registers an event for the current visitor to the flood control mechanism.
   *
   * @param string $name
   *   The name of an event.
   * @param int $window
   *   Optional number of seconds before this event expires. Defaults to 3600 (1
   *   hour). Typically uses the same value as the flood_is_allowed() $window
   *   parameter. Expired events are purged on cron run to prevent the flood
   *   table from growing indefinitely.
   * @param string $identifier
   *   Optional identifier (defaults to the current user's IP address).
   */
  public function register($name, $window = 3600, $identifier = NULL) {
    $this->drupal7->flood_register_event($name, $window, $identifier);
  }

  /**
   * Implements OpenlayersDrupal\Core\Flood\FloodInterface::clear().
   *
   * Makes the flood control mechanism forget an event for the current visitor.
   *
   * @param string $name
   *   The name of an event.
   * @param string $identifier
   *   Optional identifier (defaults to the current user's IP address).
   */
  public function clear($name, $identifier = NULL) {
    $this->drupal7->flood_clear_event($name, $identifier);
  }

  /**
   * Implements OpenlayersDrupal\Core\Flood\FloodInterface::isAllowed().
   *
   * Checks whether a user is allowed to proceed with the specified event.
   *
   * Events can have thresholds saying that each user can only do that event
   * a certain number of times in a time window. This function verifies that the
   * current user has not exceeded this threshold.
   *
   * @param string $name
   *   The unique name of the event.
   * @param int $threshold
   *   The maximum number of times each user can do this event per time window.
   * @param int $window
   *   Number of seconds in the time window for this event (default is 3600
   *   seconds, or 1 hour).
   * @param string $identifier
   *   Unique identifier of the current user. Defaults to their IP address.
   *
   * @return bool
   *   TRUE if the user is allowed to proceed. FALSE if they have exceeded the
   *   threshold and should not be allowed to proceed.
   */
  public function isAllowed($name, $threshold, $window = 3600, $identifier = NULL) {
    return $this->drupal7->flood_is_allowed($name, $threshold, $window, $identifier);
  }

  /**
   * Implements OpenlayersDrupal\Core\Flood\FloodInterface::garbageCollection().
   */
  public function garbageCollection() {
    return $this->connection->delete('flood')
      ->condition('expiration', REQUEST_TIME, '<')
      ->execute();
  }

}
