<?php

/**
 * @file
 * Contains the \Drupal\akamai\Batcher class.
 *
 * Groups queued purge requests into batches by hostname.
 */

namespace Drupal\akamai;

use Drupal\akamai\Batch;
use InvalidArgumentException;

class Batcher {

  /**
   * CCU API client object.
   *
   * @var CcuClientInterface
   */
  protected $client;

  /**
   * Items to be batched, grouped by hostname.
   *
   * @var array
   */
  protected $items = [];

  /**
   * The maximum number of items to put in each batch.
   *
   * @var int
   */
  protected $maxBatchSize = NULL;

  /**
   * Constructor.
   *
   * @param CcuClientInterface $client
   *   A CCU client object. Used to determine the maximum size of the batch
   *   based on the 50,000 byte limit of the purge request body.
   * @param int $max_batch_size
   *   The maximum number of items to place in each batch. 0 means no limit and
   *   the batch size will only be limited by the byte size limit.
   */
  public function __construct(CcuClientInterface $client, $max_batch_size = 0) {
    $this->client = $client;
    if (!empty($max_batch_size)) {
      $this->maxBatchSize = $max_batch_size;
    }
  }

  /**
   * Adds an item to be batched.
   */
  public function insertItem($item) {
    if (empty($item->data['hostname'])) {
      throw new InvalidArgumentException("Expected item to contain a hostname.");
    }
    $hostname = $item->data['hostname'];
    $this->items[$hostname][$item->item_id] = $item;
  }

  /**
   * Removes an item after it has been batched.
   */
  public function removeItem($item) {
    $hostname = $item->data['hostname'];
    unset($this->items[$hostname][$item->item_id]);
  }

  /**
   * Gets all items that are yet to be batched.
   */
  public function getItems() {
    $all_items = [];
    foreach ($this->items as $hostname => $items) {
      $all_items = array_merge($all_items, $items);
    }
    return $all_items;
  }

  /**
   * Checks if all items have been batched.
   */
  public function isEmpty() {
    return empty($this->items);
  }

  /**
   * Generates a batch of items to submit to the CCU API.
   *
   * The batch size is determined by whichever limit is reached first:
   * - The maximum byte size of the API request body.
   * - The configured max batch size if one has been set.
   */
  public function getBatch() {
    reset($this->items);
    $hostname = key($this->items);
    $batch = new Batch($this->maxBatchSize);

    foreach ($this->items[$hostname] as $item) {
      if (!$batch->isFull() && $this->itemWillFitInRequest($batch, $item)) {
        $batch->addItem($item);
        $this->removeItem($item);
      }
      else {
        break;
      }
    }
    if (empty($this->items[$hostname])) {
      unset($this->items[$hostname]);
    }
    return $batch;
  }

  /**
   * Determines if an item will exceed the byte size limit of API request body.
   */
  protected function itemWillFitInRequest(Batch $batch, $item) {
    $hostname = $batch->getHostname();
    $paths = array_merge($batch->getPaths(), $item->data['paths']);
    return $this->client->bodyIsBelowLimit($hostname, $paths);
  }

}
