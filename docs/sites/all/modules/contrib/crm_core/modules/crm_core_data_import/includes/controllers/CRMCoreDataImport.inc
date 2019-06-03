<?php
/**
 * @file
 * Handler for data imports in CRM Core
 */

class CRMCoreDataImport {
  public $id;
  public $machine_name;
  public $title;
  public $description;
  public $source;
  public $created;
  public $lastimport;
  public $source_plugin;
  public $settings;
  public $source_settings;

  protected $endpoints;
  /**
   * Constructor.
   */
  public function __construct() {
    $this->id = NULL;
    $this->endpoints = array();
  }

  /**
   * Load configuration.
   */
  public function load() {
    $data = db_select('crm_core_data_import', 'd')
      ->fields('d')
      ->condition('d.id', $this->id)
      ->execute()
      ->fetchAssoc();
    foreach ($data as $key => $value) {
      $this->{$key} = $value;
    }

    // Unserialize settings.
    $this->settings = unserialize($this->settings);
    $this->source_settings = unserialize($this->source_settings);

    // Load source plugin instance.
    $this->source_plugin = crm_core_data_import_load_plugin_instance('crm_core_data_import_source', $this->source);
  }

  /**
   * Save configuration.
   */
  public function save() {
    $fields = array(
      'machine_name' => $this->machine_name,
      'title' => $this->title,
      'description' => $this->description,
      'source' => $this->source,
      'settings' => serialize($this->settings),
      'source_settings' => serialize($this->source_settings),
      'lastimport' => !empty($this->lastimport) ? $this->lastimport : 0,
      'created' => !empty($this->created) ? $this->created : time(),
    );

    // Create/Update data import.
    if (empty($this->id)) {
      $this->id = db_insert('crm_core_data_import')
        ->fields($fields)
        ->execute();
    }
    else {
      db_merge('crm_core_data_import')
        ->key(array('id' => $this->id))
        ->fields($fields)
        ->execute();
    }
  }

  /**
   * Set source settings.
   */
  public function setSourceSettings(array $values) {
    $this->source_settings = $values;
  }

  /**
   * Get source settings.
   */
  public function getSourceSettings() {
    return !empty($this->source_settings) ? $this->source_settings : array();
  }

  /**
   * Set mapping settings.
   */
  public function setMappingSettings(array $values) {
    $this->settings['mapping'] = $values;
  }

  /**
   * Get mapping settings.
   */
  public function getMappingSettings() {
    return !empty($this->settings['mapping']) ? $this->settings['mapping'] : array();
  }

  /**
   * Set linked imports.
   * @param array of linked import ids.
   */
  public function setLinkedImports(array $values) {
    $this->settings['linked_imports'] = $values;
  }

  /**
   * Get mapping settings.
   * @return array of linked import ids.
   */
  public function getLinkedImports() {
    if (empty($this->settings['linked_imports'])
     || empty($this->settings['linked_imports']['enable'])
     || empty ($this->settings['linked_imports']['fields'])) {
          return array();
    }
    else {
      return $this->settings['linked_imports']['fields'];
    }
  }

  /**
   * Get relation settings.
   */
  public function getRelationSettings() {
    return !empty($this->settings['relations']) ? $this->settings['relations'] : array();
  }

  /**
   * Set settings.
   */
  public function setSettings(array $values) {
    $this->settings = $values;
  }

  /**
   * Get settings.
   */
  public function getSettings() {
    return !empty($this->settings) ? $this->settings : FALSE;
  }

  /**
   * Returns source fields for data import.
   */
  public function getSourceFields() {
    return $this->source_plugin->getFields($this->source_settings);
  }

  /**
   * Returns MigrationSource instance.
   */
  public function migrateSource($entity_type = FALSE, $entity_bundle = FALSE) {
    return $this->source_plugin->migrateSource($this->source_settings, $entity_type, $entity_bundle, $this->getMappingSettings());
  }

  /**
   * Returns all available destinations for current import task.
   */
  public function getDestinations() {
    $mapping = $this->getMappingSettings();
    $destinations = array();
    if (!empty($mapping)) {
      foreach ($mapping as $key => $mapping_item) {
        list($entity_controller_type, $entity_bundle_mapping, $bundle_delta) = explode(':', $key);
        $entity_type_mapping = crm_core_data_import_get_destination_entity_type($entity_controller_type);
        $entity_key = $entity_type_mapping . ':' . $entity_bundle_mapping . ':' . $bundle_delta;
        $destinations[$entity_key] = crm_core_data_import_load_plugin_instance('crm_core_data_import_destination', $entity_controller_type);
        $destinations[$entity_key]->controller = $entity_controller_type;
        $destinations[$entity_key]->bundle = $entity_bundle_mapping;
      }
    }
    return $destinations;
  }

  /**
   * Returns all available migration instances.
   */
  public function getMigrationInstances() {
    $mapping = $this->getMappingSettings();
    $migrations = array();
    if (!empty($mapping)) {
      foreach ($mapping as $key => $mapping_item) {
        list($entity_controller_type, $entity_bundle, $bundle_delta) = explode(':', $key);
        $entity_type = crm_core_data_import_get_destination_entity_type($entity_controller_type);
        $machine_name = _crm_core_data_import_migration_machine_name($this->id, $entity_type, $entity_bundle, $bundle_delta);
        $migration = Migration::getInstance($machine_name);
        if ($migration) {
          $migrations[] = $migration;
        }
        // Remove errors "No migration found" if migration not available.
        drupal_get_messages('error');
      }
    }
    return $migrations;
  }

  /**
   * Returns destination based on entity_type and bundle.
   */
  public function getDestinationsForEntity($entity_type, $entity_bundle, $delta) {
    $destinations = $this->getDestinations();
    $key = $entity_type . ':' . $entity_bundle . ':' . $delta;
    if (!empty($destinations[$key])) {
      return $destinations[$key];
    }
    return FALSE;
  }

  /**
   * Returns MigrationDestination instance.
   */
  public function migrateDestination($entity_type, $entity_bundle, $delta) {
    if ($destination = $this->getDestinationsForEntity($entity_type, $entity_bundle, $delta)) {
      return $destination->migrateDestination($entity_bundle);
    }
    return FALSE;
  }

  /**
   * Returns migrate source map from source plugin.
   */
  public function migrateSourceMap($entity_type, $entity_bundle, $delta) {
    if ($destination = $this->getDestinationsForEntity($entity_type, $entity_bundle, $delta)) {
      $key = $destination->controller . ':' . $entity_bundle . ':' . $delta;
      if ($mapping = $this->getMappingSettings()) {
        if (!empty($mapping[$key]['primary']['source_field_primary'])) {
          $return = array(
            $mapping[$key]['primary']['source_field_primary'] => array(
              'type' => 'varchar',
              'length' => 32,
              'not null' => TRUE,
            ),
          );
          return $return;
        }
      }
    }
    return array();
  }

  /**
   * Returns DestinationMap for certain destination.
   */
  public function migrateDestinationMap($entity_type, $entity_bundle, $delta) {
    if ($destination = $this->getDestinationsForEntity($entity_type, $entity_bundle, $delta)) {
      return $destination->migrateDestinationMap();
    }
    return array();
  }

  /**
   * find relation end
   * @param $source_key
   *
   * @return array of $destination keys in format entity_type:bundle:delta otherwise
   */
  public function getRelationDestinationEndPoints($source_key) {
    if (isset($this->endpoints[$source_key])) {
      return $this->endpoints[$source_key];
    }
    $relations = $this->getRelationSettings();
    $destinations = array();
    if (!$relations || !$relations['enable']) {
      return $destinations;
    }

    if (!empty($relations['fields'])) {
      foreach ($relations['fields'] as $relation) {
        if ($relation['source'] == $source_key) {
          $destinations[] = $relation['destination'];
        }
      }
      $this->endpoints[$source_key] = $destinations;
    }

    return $destinations;
  }

  /**
   * Returns list of all importer IDs
   *
   * @param $conditions array of db conditions.
   *
   * i.e. $conditions = db_and()->condition('id', 1);
   *
   * @returns int[]
   */
  public static function getIds($conditions = array()) {
    $query = db_select('crm_core_data_import', 'd')
      ->fields('d', array('id'));
    foreach ($conditions as $condition) {
      $query->condition($condition);
    }
    return $query->execute()
      ->fetchCol();
  }
}
