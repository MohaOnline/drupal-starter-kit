<?php
/**
 * @file
 * Handler for migration process.
 */

class MigrationDataImport extends Migration {
  /**
   * Mapping of current migration
   */
  public $fields_mapping;

  /**
   * Mapping for Field Collection fields.
   */
  public $collection_mapping;

  /**
   * List of Field Collection fields.
   */
  public $collection_items;

  /**
   * General initialization of a Migration object.
   */
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct($arguments);

    $importer = $this->getImporter();
    $entity_type = $this->getEntityType();
    $entity_bundle = $this->getEntityBundle();
    $delta = $this->getDelta();

    // Set migrate source settings.
    $this->source = $importer->migrateSource($entity_type, $entity_bundle);

    // Set migrate destination settings.
    $this->destination = $importer->migrateDestination($entity_type, $entity_bundle, $delta);

    // Key schema.
    $this->map = new CRMCoreMigrateSQLMap(
      $this->machineName,
      $importer->migrateSourceMap($entity_type, $entity_bundle, $delta),
      $importer->migrateDestinationMap($entity_type, $entity_bundle, $delta),
      'default',
      array(
        'importer' => $importer,
        'entity_type' => $entity_type,
        'entity_bundle' => $entity_bundle,
        'delta' => $delta,
      )
    );

    // Get linked data imports.
    // If found, we will populate MigrateFieldMapping::sourceMigration later
    // to automatically pull already imported values.
    $linked_imports = $importer->getLinkedImports();
    $source_migration = array();
    if ($linked_imports) {
      foreach ($linked_imports as $id => $field) {
        $linked_importer = crm_core_data_import_load_importer($id);
        $candidates = $linked_importer->getMigrationInstances();
        foreach ($candidates as $m) {
          /** @var Migration $m */
          $source_keys = array_keys($m->getMap()->getSourceKey());
          if ($source_keys[0] == $field['destination']) {
            $source_migration[$field['destination']] = $m->getMachineName();
          }
        }
      }
    }

    // Set mapping according to settings.
    $destination = $importer->getDestinationsForEntity($this->getEntityType(), $this->getEntityBundle(), $this->getDelta());
    $mapping = $importer->getMappingSettings();
    if ($destination->entityType == $entity_type && $destination->bundle == $entity_bundle && !empty($mapping[$destination->controller . ':' . $destination->bundle . ':' . $delta])) {
      $this->fields_mapping = $mapping[$destination->controller . ':' . $destination->bundle . ':' . $delta];

      switch ($destination->entityType) {
        case 'user':
          // Separate handling for user entity.
          foreach ($this->fields_mapping as $key => $field) {
            if ($key !== 'primary' && !$this->skipCollectionField($field) && !$this->skipReferencesField($field)) {
              if ($field['destination_field'] == 'name') {
                $single_mapping = $this->addFieldMapping($field['destination_field'], $field['source_field'])->dedupe('users', 'name');
              }
              elseif ($field['destination_field'] == 'mail') {
                $single_mapping = $this->addFieldMapping($field['destination_field'], $field['source_field'])->dedupe('users', 'mail');
              }
              else {
                $single_mapping = $this->addRegularFieldMapping($field);
              }
              // Add sourceMigration from linked import(s) if necessary
              if (!empty($source_migration[$field['source_field']])) {
                $single_mapping->sourceMigration($source_migration[$field['source_field']]);
              }
            }
          }
          // Add status if not added in the configuration form.
          if (!array_key_exists('status', $this->codedFieldMappings)) {
            $this->addFieldMapping('status')->defaultValue(TRUE);
          }
          break;

        case 'crm_core_activity':
          // Separate handling for crm_core_activity entity.
          foreach ($this->fields_mapping as $key => $field) {
            if ($key !== 'primary' && !$this->skipReferencesField($field)) {
              $single_mapping = $this->addRegularFieldMapping($field);
              // Add sourceMigration from linked import(s) if necessary
              if (!empty($source_migration[$field['source_field']])) {
                $single_mapping->sourceMigration($source_migration[$field['source_field']]);
              }
            }
          }

          // Add uid if not added in the configuration form.
          if (!array_key_exists('uid', $this->codedFieldMappings)) {
            $this->addFieldMapping('uid')->defaultValue(0);
          }
          break;

        default:
          foreach ($this->fields_mapping as $key => $field) {
            if ($key !== 'primary' && !$this->skipCollectionField($field) && !$this->skipReferencesField($field)) {
              $single_mapping = $this->addRegularFieldMapping($field);
              // Add sourceMigration from linked import(s) if necessary
              if (!empty($source_migration[$field['source_field']])) {
                $single_mapping->sourceMigration($source_migration[$field['source_field']]);
              }
            }

            // Collect names of the field collection fields.
            if ($this->skipCollectionField($field)) {
              $main_field = $this->getMainFieldName($field['destination_field']);
              $this->collection_items[] = $main_field;

            }
          }
          break;
      }
    }

    // Run preImport method in settings plugins.
    $importer_settings = $importer->getSettings();
    if (!empty($importer_settings['settings_controllers'])) {
      foreach ($importer_settings['settings_controllers'] as $controller) {
        $settings_controller = crm_core_data_import_load_plugin_instance('crm_core_data_import_settings', $controller);
        if ($settings_controller) {
          $settings_controller->preImport($importer, $this);
        }
      }
    }

    if (!empty($this->collection_items)) {
      $this->collection_items = array_unique($this->collection_items);
      $this->buildFieldCollectionMapping();
    }

    drupal_alter('crm_core_data_import_migration', $this);
  }

  /**
   * This method is called from the source plugin upon first pulling the raw data from the source.
   */
  public function prepareRow($row) {
    $plugins = ctools_get_plugins('crm_core_data_import', 'crm_core_data_import_conversion');

    foreach ($plugins as $plugin) {
      $conversion = crm_core_data_import_load_plugin_instance('crm_core_data_import_conversion', $plugin['name']);
      // Run conversion plugin if he compatible with source plugin.
      if ($this->isCompatible($conversion)) {
        $conversion->prepareRow($row, $this->getImporter());
      }
    }

    parent::prepareRow($row);
    drupal_alter('crm_core_data_import_source_row', $row, $this->generateMachineName());
    $this->prepareDefaultValues($row);

    return $row;
  }

  /**
   * Prepare entity before saving.
   */
  public function prepare($entity, $row) {
    // Run match engine for crm_core_contact.
    if ($this->getEntityType() == 'crm_core_contact') {
      // Set temporary contact_id.
      $entity->contact_id = 0;
      $matches = array();
      $values = array(
        'contact' => &$entity,
        'matches' => &$matches,
      );
      module_invoke_all('crm_core_contact_match', $values);

      // If contact already exist, replace contact_id to match.
      if (!empty($matches)) {
        $entity->contact_id = reset($matches);
        // Set current entity as original to update fields.
        $entity->original = $entity;
        entity_save('crm_core_contact', $entity);
      }
    }
    drupal_alter('crm_core_data_import_prepare_entity', $entity, $row, $this->generateMachineName());
  }

  /**
   * Complete callback for each item.
   *
   * @param object $entity
   *   Imported entity.
   *
   * @param object $row
   *   Row of source values.
   */
  public function complete($entity, $row) {
    // Process field collections.
    if (!empty($this->collection_items) && !empty($this->collection_mapping)) {
      $collection_items = $this->collection_items;
      foreach ($collection_items as $collection_item) {
        $this->attachFieldCollection($entity, $row, $collection_item, $this->collection_mapping[$collection_item]['mapping'], $this->collection_mapping[$collection_item]['main_field']);
      }
    }
    // @TODO need add hook
  }

  /**
   * Generate machine name. Data Import task and id with prefix.
   */
  public function generateMachineName($class_name = NULL) {
    return _crm_core_data_import_migration_machine_name($this->arguments['importer']->id, $this->arguments['entity_type'], $this->arguments['entity_bundle'], $this->arguments['delta']);
  }

  /**
   * Returns entity type.
   */
  public function getEntityType() {
    return $this->arguments['entity_type'];
  }

  /**
   * Returns entity bundle.
   */
  public function getEntityBundle() {
    return $this->arguments['entity_bundle'];
  }

  /**
   * Returns entity delta.
   */
  public function getDelta() {
    return $this->arguments['delta'];
  }

  /**
   * Returns importer.
   */
  public function getImporter() {
    return $this->arguments['importer'];
  }

  /**
   * Helper to add field to mapping.
   *
   * @return MigrateFieldMapping
   */
  public function addRegularFieldMapping($field) {
    if ($field['source_field'] == 'default_value') {
      // If need to set only default value.
      $mapping = $this->addFieldMapping($field['destination_field'])
        ->defaultValue($field['default_value']);
    }
    elseif (!empty($field['default_value'])) {
      // If need to set default value. Will be apply if source data is empty.
      $mapping = $this->addFieldMapping($field['destination_field'], $field['source_field'])
        ->defaultValue($field['default_value']);
    }
    else {
      $mapping = $this->addFieldMapping($field['destination_field'], $field['source_field']);
    }
    return $mapping;
  }

  /**
   * Prepare row for setting default values.
   */
  public function prepareDefaultValues(&$row) {
    // Remove values from row, if they are empty and have default value.
    $importer = $this->getImporter();
    $entity_type = $this->getEntityType();
    $entity_bundle = $this->getEntityBundle();
    $delta = $this->getDelta();
    $mapping = $importer->getMappingSettings();
    $destination = $importer->getDestinationsForEntity($this->getEntityType(), $this->getEntityBundle(), $this->getDelta());
    if ($destination->entityType == $entity_type && $destination->bundle == $entity_bundle && !empty($mapping[$destination->controller . ':' . $destination->bundle . ':' . $delta])) {
      $fields_mapping = $mapping[$destination->controller . ':' . $destination->bundle . ':' . $delta];
      foreach ($fields_mapping as $field) {
        if (!empty($field['default_value']) && !empty($field['source_field']) && empty($row->{$field['source_field']})) {
          unset($row->{$field['source_field']});
        }
      }
    }
  }

  /**
   * Check if we need skip references field.
   */
  public function skipReferencesField($field) {
    $settings = $importer = $this->getImporter()->getSettings();
    if (!empty($settings['references']['enable']) && !empty($settings['references']['fields'])) {
      foreach ($settings['references']['fields'] as $reference) {
        list($entity_type, $entity_bundle, $delta, $target_field_name) = explode(':', $reference['reference_type']);
        if ($this->getEntityType() == $entity_type
          && $this->getEntityBundle() == $entity_bundle
          && $this->getDelta() == $delta
          && $target_field_name == $field['destination_field']) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Check if we need skip field collection.
   */
  public function skipCollectionField($field) {
    if (!empty($field['destination_field'])) {
      $main_field = $this->getMainFieldName($field['destination_field']);
      if (!empty($main_field) && _crm_core_data_import_is_field_collection_field($main_field)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Returns main field name.
   */
  public function getMainFieldName($field) {
    list($main_field) = explode(':', $field);
    return $main_field;
  }

  /**
   * Return TRUE if conversion plugin is compatible with current source plugin.
   */
  public function isCompatible($conversion) {
    $importer = $this->getImporter();
    $types = $conversion->compatible();
    return empty($types) || in_array($importer->source, $types);
  }

  /**
   * Attach FC item to host entity.
   *
   * @param object $entity
   *   Imported entity.
   *
   * @param object $row
   *   Data to import.
   *
   * @param string $collection_item
   *   Name of the FC item.
   *
   * @param array $collection_mapping
   *   Mapping of the FC entity.
   *
   * @param string $main_source_field
   *   Name of the main field in source.
   */
  public function attachFieldCollection(&$entity, &$row, $collection_item, $collection_mapping, $main_source_field) {
    if (!empty($row->{$main_source_field . '_collection'})) {
      foreach ($row->{$main_source_field . '_collection'} as $collection_value) {
        try {
          // Create FC entity.
          $collection_entity = entity_create('field_collection_item', array('field_name' => $collection_item));
          $collection_entity_wrapper = entity_metadata_wrapper('field_collection_item', $collection_entity);
          $collection_entity->setHostEntity($this->getEntityType(), $entity);

          // Add values according to mapping.
          foreach ($collection_mapping as $mapping_item) {
            $value = '';
            // Process default value.
            if ($mapping_item['source']['type'] == 'default_value') {
              // Set default value.
              $value = $mapping_item['destination']['default_value'];
            }
            elseif (isset($collection_value[$mapping_item['source']['sub_field']])) {
              // Set field value from source.
              $value = $collection_value[$mapping_item['source']['sub_field']];
            }

            if (empty($value) && !empty($mapping_item['destination']['default_value'])) {
              // Set default value if source value is empty.
              $value = $mapping_item['destination']['default_value'];
            }

            if (!empty($value) && isset($mapping_item['destination']['sub_field'])) {
              $field_name = $mapping_item['destination']['sub_field'];

              if (!empty($mapping_item['destination']['part'])) {
                // If field is part, just attach to existing entity.
                $collection_entity_wrapper->{$field_name}->{$mapping_item['destination']['part']}->set($value);
              }
              else {
                // Process field with MigrateFieldHandler.
                $field_info = field_info_field($field_name);
                $field_instance = field_info_instance('field_collection_item', $field_name, $collection_item);
                $value = migrate_field_handler_invoke_all($collection_entity, $field_info, $field_instance, array($value));
                $collection_entity->{$field_name} = $value;
              }
            }
          }
          $collection_entity->save();
        }
        catch (Exception $e) {
          watchdog('crm_core_data_import', $e->getMessage(), array(), WATCHDOG_ERROR);
        }
      }
    }
  }

  /**
   * Build field collection mapping.
   */
  public function buildFieldCollectionMapping() {
    if (!empty($this->collection_items)) {
      $collection_items = $this->collection_items;
      foreach ($collection_items as $collection_item) {
        $collection_mapping = array();
        foreach ($this->fields_mapping as $mapping_item) {
          if (!empty($mapping_item['source_field']) && !empty($mapping_item['destination_field'])) {
            $source_raw = explode(':', $mapping_item['source_field']);
            $destination_raw = explode(':', $mapping_item['destination_field']);

            $source = array(
              'type' => !empty($source_raw[0]) ? $source_raw[0] : NULL,
              'bundle' => !empty($source_raw[1]) ? $source_raw[1] : NULL,
              'main_field' => !empty($source_raw[2]) ? $source_raw[2] : NULL,
              'sub_field' => !empty($source_raw[3]) ? $source_raw[3] : NULL,
            );
            $destination = array(
              'type' => $this->getEntityType(),
              'bundle' => $this->getEntityBundle(),
              'main_field' => !empty($destination_raw[0]) ? $destination_raw[0] : NULL,
              'sub_field' => !empty($destination_raw[1]) ? $destination_raw[1] : NULL,
              'part' => !empty($destination_raw[2]) ? $destination_raw[2] : NULL,
              'default_value' => !empty($mapping_item['default_value']) ? $mapping_item['default_value'] : NULL,
            );

            if (!empty($destination['main_field']) && $destination['main_field'] == $collection_item) {
              $collection_mapping[] = array(
                'source' => $source,
                'destination' => $destination,
              );

              if (!empty($source['main_field'])) {
                $main_source_field = $source['type'] . ':' . $source['bundle'] . ':' . $source['main_field'];
              }
            }
          }
        }

        if (!empty($collection_mapping) && !empty($main_source_field)) {
          $this->collection_mapping[$collection_item] = array(
            'mapping' => $collection_mapping,
            'main_field' => $main_source_field,
          );
        }
      }
    }
  }
}
