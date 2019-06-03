<?php
/**
 * @file
 * Defines a CRMCore extention of MigrateSQLMap
 */

class CRMCoreMigrateSQLMap extends MigrateSQLMap {
  // We need importer to get info about related migrations.
  // This will allow us to add relation_id fields to migrate map table schema.
  /** @var  CRMCoreDataImport */
  protected $importer;
  protected $options;

  public function __construct($machine_name, array $source_key,
                              array $destination_key, $connection_key = 'default', $options = array()) {
    if (!empty($options['importer']) && $options['importer'] instanceof CRMCoreDataImport) {
      $this->importer = $options['importer'];
      unset($options['importer']);
    }
    $this->options = $options;
    parent::__construct($machine_name, $source_key, $destination_key, $connection_key, $options);
  }

    protected function ensureTables() {
    if (!$this->ensured) {
      if (!$this->connection->schema()->tableExists($this->mapTable)) {
        // Generate appropriate schema info for the map and message tables,
        // and map from the source field names to the map/msg field names
        $count = 1;
        $source_key_schema = array();
        $pks = array();
        foreach ($this->sourceKey as $field_schema) {
          $mapkey = 'sourceid' . $count++;
          // Make source key case sensitive
          $field_schema['binary'] = TRUE;
          $source_key_schema[$mapkey] = $field_schema;
          $pks[] = $mapkey;
        }

        $fields = $source_key_schema;

        // Add destination keys to map table
        // TODO: How do we discover the destination schema?
        $count = 1;
        foreach ($this->destinationKey as $field_schema) {
          // Allow dest key fields to be NULL (for IGNORED/FAILED cases)
          $field_schema['not null'] = FALSE;
          $mapkey = 'destid' . $count++;
          $fields[$mapkey] = $field_schema;
        }
        $fields['needs_update'] = array(
          'type' => 'int',
          'size' => 'tiny',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => MigrateMap::STATUS_IMPORTED,
          'description' => 'Indicates current status of the source row',
        );
        $fields['rollback_action'] = array(
          'type' => 'int',
          'size' => 'tiny',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => MigrateMap::ROLLBACK_DELETE,
          'description' => 'Flag indicating what to do for this item on rollback',
        );
        $fields['last_imported'] = array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => 0,
          'description' => 'UNIX timestamp of the last time this row was imported',
        );
        $fields['hash'] = array(
          'type' => 'varchar',
          'length' => '32',
          'not null' => FALSE,
          'description' => 'Hash of source row data, for detecting changes',
        );

        // Find out if we need to add relation ids to schema.
        // Add source id for relations that we import.
        // We need source id only for one endpoint of relationship.
        // Let it be source endpoint.
        $destinations = $this->importer->getRelationDestinationEndPoints(implode(':', array($this->options['entity_type'], $this->options['entity_bundle'], $this->options['delta'])));
        foreach ($destinations as $key => $destination) {
          $fields['relation_id' . ($key + 1)] = array(
            'type' => 'varchar',
            'length' => '32',
            'binary' => TRUE,
            'not null' => FALSE,
            'description' => 'ID of relation endpoint entity',
          );
        }

        $schema = array(
          'description' => t('Mappings from source key to destination key'),
          'fields' => $fields,
          'primary key' => $pks,
        );
        $this->connection->schema()->createTable($this->mapTable, $schema);

        // Now for the message table
        $fields = array();
        $fields['msgid'] = array(
          'type' => 'serial',
          'unsigned' => TRUE,
          'not null' => TRUE,
        );
        $fields += $source_key_schema;

        $fields['level'] = array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => 1,
        );
        $fields['message'] = array(
          'type' => 'text',
          'size' => 'medium',
          'not null' => TRUE,
        );
        $schema = array(
          'description' => t('Messages generated during a migration process'),
          'fields' => $fields,
          'primary key' => array('msgid'),
          'indexes' => array('sourcekey' => $pks),
        );



        $this->connection->schema()->createTable($this->messageTable, $schema);
      }
      else {
        // Add any missing columns to the map table
        if (!$this->connection->schema()->fieldExists($this->mapTable,
          'rollback_action')) {
          $this->connection->schema()->addField($this->mapTable,
            'rollback_action', array(
              'type' => 'int',
              'size' => 'tiny',
              'unsigned' => TRUE,
              'not null' => TRUE,
              'default' => 0,
              'description' => 'Flag indicating what to do for this item on rollback',
            ));
        }
        if (!$this->connection->schema()->fieldExists($this->mapTable, 'hash')) {
          $this->connection->schema()->addField($this->mapTable, 'hash', array(
            'type' => 'varchar',
            'length' => '32',
            'not null' => FALSE,
            'description' => 'Hash of source row data, for detecting changes',
          ));
        }
      }
      $this->ensured = TRUE;
    }
  }

  /**
   * Called upon import of one record, we record a mapping from the source key
   * to the destination key. Also may be called, setting the third parameter to
   * NEEDS_UPDATE, to signal an existing record should be remigrated.
   *
   * @param stdClass $source_row
   *  The raw source data. We use the key map derived from the source object
   *  to get the source key values.
   * @param array $dest_ids
   *  The destination key values.
   * @param int $needs_update
   *  Status of the source row in the map. Defaults to STATUS_IMPORTED.
   * @param int $rollback_action
   *  How to handle the destination object on rollback. Defaults to
   *  ROLLBACK_DELETE.
   * $param string $hash
   *  If hashing is enabled, the hash of the raw source row.
   */
  public function saveIDMapping(stdClass $source_row, array $dest_ids,
                                $needs_update = MigrateMap::STATUS_IMPORTED,
                                $rollback_action = MigrateMap::ROLLBACK_DELETE, $hash = NULL) {
    migrate_instrument_start('saveIDMapping');
    // Construct the source key
    $keys = array();
    foreach ($this->sourceKeyMap as $field_name => $key_name) {
      // A NULL key value will fail.
      if (is_null($source_row->$field_name)) {
        Migration::displayMessage(t(
          'Could not save to map table due to NULL value for key field !field',
          array('!field' => $field_name)));
        migrate_instrument_stop('saveIDMapping');
        return;
      }
      $keys[$key_name] = $source_row->$field_name;
    }

    $fields = array(
      'needs_update' => (int)$needs_update,
      'rollback_action' => (int)$rollback_action,
      'hash' => $hash,
    );

    // Add relation ids when present.
    foreach ($this->importer->getRelationDestinationEndPoints(implode(':', array($this->options['entity_type'], $this->options['entity_bundle'], $this->options['delta']))) as $key => $ep) {
      list($ep_entity_type, $ep_bundle, $ep_importer_id) = explode(':', $ep);
      $endpoint_machine_name = _crm_core_data_import_migration_machine_name($ep_importer_id, $ep_entity_type, $ep_bundle, $this->options['delta']);
      $endpoint_migration = Migration::getInstance($endpoint_machine_name);
      $ep_source_map = array_keys($endpoint_migration->getMap()->getSourceKeyMap());
      $fields['relation_id' . ($key + 1)] = $source_row->{$ep_source_map[0]};
    }

    $count = 1;
    if (!empty($dest_ids)) {
      foreach ($dest_ids as $dest_id) {
        $fields['destid' . $count++] = $dest_id;
      }
    }
    if ($this->trackLastImported) {
      $fields['last_imported'] = time();
    }
    $this->connection->merge($this->mapTable)
      ->key($keys)
      ->fields($fields)
      ->execute();
    migrate_instrument_stop('saveIDMapping');
  }
}