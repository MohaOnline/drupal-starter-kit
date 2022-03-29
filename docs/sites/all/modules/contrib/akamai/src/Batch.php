<?php
/**
 * @file
 * Contains the \Drupal\akamai\Batch class.
 */

namespace Drupal\akamai;

use InvalidArgumentException;

class Batch {

  /**
   * The hostname to use for this batch.
   *
   * @var string
   */
  protected $hostname = NULL;

  /**
   * Items included in this batch.
   *
   * @var array
   */
  protected $items = [];

  /**
   * Maximum number of items allowed in the batch.
   *
   * @var array
   */
  protected $maxSize = NULL;

  /**
   * Constructor.
   *
   * @param int $max_size
   *   The maximum number of items allowed in the batch.
   */
  public function __construct($max_size = NULL) {
    if (is_int($max_size)) {
      $this->maxSize = $max_size;
    }
  }

  /**
   * Gets the hostname for this batch.
   */
  public function getHostname() {
    return $this->hostname;
  }

  /**
   * Adds an item to the batch.
   */
  public function addItem($item) {
    if (empty($this->hostname)) {
      $this->hostname = $item->data['hostname'];
    }
    elseif ($item->data['hostname'] != $this->hostname) {
      throw new InvalidArgumentException('All items in a batch must have the same hostname.');
    }
    $this->items[] = $item;
  }

  /**
   * Checks if the batch size limit has been reached.
   */
  public function isFull() {
    if (empty($this->maxSize)) {
      return FALSE;
    }
    return (count($this->items) >= $this->maxSize);
  }

  /**
   * Returns all items in the batch.
   */
  public function getItems() {
    return $this->items;
  }

  /**
   * Returns the paths of all items in the batch.
   */
  public function getPaths() {
    $paths = [];
    foreach ($this->items as $item) {
      foreach ($item->data['paths'] as $path) {
        $paths[] = $path;
      }
    }
    return $paths;
  }

}
