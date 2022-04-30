<?php

namespace Drupal\openlayers\KeyValueStore;

/**
 * FIX - insert short comment here.
 */
class KeyValueDatabaseExpirableFactory extends KeyValueDatabaseExpirableFactory {

  /**
   * {@inheritdoc}
   */
  public function get($collection) {
    if (!isset($this->storages[$collection])) {
      $this->storages[$collection] = new DatabaseStorageExpirable($collection, $this->serializer, $this->connection);
    }
    return $this->storages[$collection];
  }

}
