<?php

namespace OpenlayersDrupal\Core\KeyValueStore;

/**
 * Provides a base class for key/value storage implementations.
 */
abstract class StorageBase implements KeyValueStoreInterface {

  /**
   * The name of the collection holding key and value pairs.
   *
   * @var string
   */
  protected $collection;

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::__construct().
   */
  public function __construct($collection) {
    $this->collection = $collection;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::getCollectionName().
   */
  public function getCollectionName() {
    return $this->collection;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::get().
   */
  public function get($key, $default = NULL) {
    $values = $this->getMultiple(array($key));
    return isset($values[$key]) ? $values[$key] : $default;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::setMultiple().
   */
  public function setMultiple(array $data) {
    foreach ($data as $key => $value) {
      $this->set($key, $value);
    }
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::delete().
   */
  public function delete($key) {
    $this->deleteMultiple(array($key));
  }

}
