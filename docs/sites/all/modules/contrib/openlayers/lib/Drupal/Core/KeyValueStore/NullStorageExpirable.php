<?php

namespace OpenlayersDrupal\Core\KeyValueStore;

/**
 * Defines a null key/value store implementation.
 */
class NullStorageExpirable implements KeyValueStoreExpirableInterface {

  /**
   * The actual storage of key-value pairs.
   *
   * @var array
   */
  protected $data = array();

  /**
   * The name of the collection holding key and value pairs.
   *
   * @var string
   */
  protected $collection;

  /**
   * Creates a new expirable null key/value store.
   */
  public function __construct($collection) {
    $this->collection = $collection;
  }

  /**
   * {@inheritdoc}
   */
  public function has($key) {
    return FALSE;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::get().
   */
  public function get($key, $default = NULL) {
    return NULL;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::getMultiple().
   */
  public function getMultiple(array $keys) {
    return array();
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::getAll().
   */
  public function getAll() {
    return array();
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::set().
   */
  public function set($key, $value) {}

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::setIfNotExists().
   */
  public function setIfNotExists($key, $value) {}

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::setMultiple().
   */
  public function setMultiple(array $data) {}

  /**
   * {@inheritdoc}
   */
  public function rename($key, $new_key) {
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::delete().
   */
  public function delete($key) {}

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::deleteMultiple().
   */
  public function deleteMultiple(array $keys) {}

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::deleteAll().
   */
  public function deleteAll() {}

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
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreExpirableInterface::setMultipleWithExpire().
   */
  public function setMultipleWithExpire(array $data, $expire) {}

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreExpirableInterface::setWithExpire().
   */
  public function setWithExpire($key, $value, $expire) {}

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreExpirableInterface::setWithExpireIfNotExists().
   */
  public function setWithExpireIfNotExists($key, $value, $expire) {}

}
