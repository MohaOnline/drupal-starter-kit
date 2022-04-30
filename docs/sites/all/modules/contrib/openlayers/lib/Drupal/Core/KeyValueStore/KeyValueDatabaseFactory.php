<?php

namespace OpenlayersDrupal\Core\KeyValueStore;

use OpenlayersDrupal\Component\Serialization\SerializationInterface;
use OpenlayersDrupal\Core\Database\Connection;

/**
 * Defines the key/value store factory for the database backend.
 */
class KeyValueDatabaseFactory implements KeyValueFactoryInterface {

  /**
   * The serialization class to use.
   *
   * @var \OpenlayersDrupal\Component\Serialization\SerializationInterface
   */
  protected $serializer;

  /**
   * The database connection to use.
   *
   * @var \OpenlayersDrupal\Core\Database\Connection
   */
  protected $connection;

  /**
   * Constructs this factory object.
   *
   * @param \OpenlayersDrupal\Component\Serialization\SerializationInterface $serializer
   *   The serialization class to use.
   * @param \OpenlayersDrupal\Core\Database\Connection $connection
   *   The Connection object containing the key-value tables.
   */
  public function __construct(SerializationInterface $serializer, Connection $connection) {
    $this->serializer = $serializer;
    $this->connection = $connection;
  }

  /**
   * {@inheritdoc}
   */
  public function get($collection) {
    return new DatabaseStorage($collection, $this->serializer, $this->connection);
  }

}
