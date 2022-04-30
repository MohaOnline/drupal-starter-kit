<?php

namespace OpenlayersDrupal\Core\KeyValueStore;

use OpenlayersDrupal\Component\Serialization\SerializationInterface;
use OpenlayersDrupal\Core\Database\Query\Merge;
use OpenlayersDrupal\Core\Database\Connection;

/**
 * Defines a default key/value store implementation.
 *
 * This is Drupal's default key/value store implementation. It uses the database
 * to store key/value data.
 */
class DatabaseStorage extends StorageBase {

  /**
   * The serialization class to use.
   *
   * @var \OpenlayersDrupal\Component\Serialization\SerializationInterface
   */
  protected $serializer;

  /**
   * The database connection.
   *
   * @var \OpenlayersDrupal\Core\Database\Connection
   */
  protected $connection;

  /**
   * The name of the SQL table to use.
   *
   * @var string
   */
  protected $table;

  /**
   * Overrides OpenlayersDrupal\Core\KeyValueStore\StorageBase::__construct().
   *
   * @param string $collection
   *   The name of the collection holding key and value pairs.
   * @param \OpenlayersDrupal\Component\Serialization\SerializationInterface $serializer
   *   The serialization class to use.
   * @param \OpenlayersDrupal\Core\Database\Connection $connection
   *   The database connection to use.
   * @param string $table
   *   The name of the SQL table to use, defaults to key_value.
   */
  public function __construct($collection, SerializationInterface $serializer, Connection $connection, $table = 'key_value') {
    parent::__construct($collection);
    $this->serializer = $serializer;
    $this->connection = $connection;
    $this->table = $table;
  }

  /**
   * {@inheritdoc}
   */
  public function has($key) {
    return (bool) $this->connection->query('SELECT 1 FROM {' . $this->connection->escapeTable($this->table) . '} WHERE collection = :collection AND name = :key', array(
      ':collection' => $this->collection,
      ':key' => $key,
    ))->fetchField();
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::getMultiple().
   */
  public function getMultiple(array $keys) {
    $values = array();
    try {
      $result = $this->connection->query(
        'SELECT name, value FROM {' . $this->connection->escapeTable($this->table) . '} WHERE name IN (:keys) AND collection = :collection',
        array(':keys' => $keys, ':collection' => $this->collection)
      )->fetchAllAssoc('name');
      foreach ($keys as $key) {
        if (isset($result[$key])) {
          $values[$key] = $this->serializer->decode($result[$key]->value);
        }
      }
    }
    catch (\Exception $e) {
      // FIX: Perhaps if the database is never going to be available,
      // key/value requests should return FALSE in order to allow exception
      // handling to occur but for now, keep it an array, always.
    }
    return $values;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::getAll().
   */
  public function getAll() {
    $result = $this->connection->query('SELECT name, value FROM {' . $this->connection->escapeTable($this->table) . '} WHERE collection = :collection', array(':collection' => $this->collection));
    $values = array();

    foreach ($result as $item) {
      if ($item) {
        $values[$item->name] = $this->serializer->decode($item->value);
      }
    }
    return $values;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::set().
   */
  public function set($key, $value) {
    $this->connection->merge($this->table)
      ->key(array(
        'name' => $key,
        'collection' => $this->collection,
      ))
      ->fields(array('value' => $this->serializer->encode($value)))
      ->execute();
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::setIfNotExists().
   */
  public function setIfNotExists($key, $value) {
    $result = $this->connection->merge($this->table)
      ->insertFields(array(
        'collection' => $this->collection,
        'name' => $key,
        'value' => $this->serializer->encode($value),
      ))
      ->condition('collection', $this->collection)
      ->condition('name', $key)
      ->execute();
    return $result == Merge::STATUS_INSERT;
  }

  /**
   * FIX - insert comment here.
   */
  public function rename($key, $new_key) {
    $this->connection->update($this->table)
      ->fields(array('name' => $new_key))
      ->condition('collection', $this->collection)
      ->condition('name', $key)
      ->execute();
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::deleteMultiple().
   */
  public function deleteMultiple(array $keys) {
    // Delete in chunks when a large array is passed.
    while ($keys) {
      $this->connection->delete($this->table)
        ->condition('name', array_splice($keys, 0, 1000))
        ->condition('collection', $this->collection)
        ->execute();
    }
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Core\KeyValueStore\KeyValueStoreInterface::deleteAll().
   */
  public function deleteAll() {
    $this->connection->delete($this->table)
      ->condition('collection', $this->collection)
      ->execute();
  }

}
