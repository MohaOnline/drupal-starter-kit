<?php
/**
 * @file
 * Data source handler for CiviCRM.
 */

$plugin = array(
  'label' => t('CiviCRM'),
  'handler' => array(
    'class' => 'CivicrmDataSourceHandler',
  ),
);

class CivicrmDataSourceHandler extends CRMCoreDataSourceHandler {
  public $sourceMapping = TRUE;
  /**
   * Configuration form for CivicrmDataSourceHandler plugin.
   */
  public function configForm(&$form, &$form_state, $settings) {
    ctools_include('plugins');
    $plugins = ctools_get_plugins('crm_core_data_import', 'crm_core_data_import_source_civicrm_entity_type');
    $form['#tree'] = TRUE;
    foreach ($plugins as $plugin) {
      if (!isset($form['civicrm_types'])) {
        $form['civicrm_types'] = array(
          '#type' => 'container',
        );
      }
      $fieldset_name = $plugin['name'];
      $plugin_instance = crm_core_data_import_load_plugin_instance('crm_core_data_import_source_civicrm_entity_type', $plugin['name']);
      $form['civicrm_types'][$fieldset_name] = array(
        '#type' => 'fieldset',
        '#title' => $plugin['label'],
      );
      $form['civicrm_types'][$fieldset_name]['plugin_name'] = array(
        '#type' => 'value',
        '#value' => $plugin['name'],
      );
      $form['civicrm_types'][$fieldset_name]['plugin_type'] = array(
        '#type' => 'value',
        '#value' => $plugin['plugin type'],
      );
      $form['civicrm_types'][$fieldset_name]['entity_types'] = array(
        '#type' => 'checkboxes',
        '#options' => $plugin_instance->getCivicrmTypes(),
        '#default_value' => !empty($settings['civicrm_types'][$fieldset_name]['entity_types']) ? $settings['civicrm_types'][$fieldset_name]['entity_types'] : array(),
      );
    }
  }

  /**
   * Validation handler for configForm().
   */
  public function configFormValidate(&$form, &$form_state, $settings) {}

  /**
   * Submission handler for configForm().
   */
  public function configFormSubmit(&$form, &$form_state, $settings) {
    return array(
      'civicrm_types' => $form_state['values']['civicrm_types'],
      'source_mapping' => !empty($settings['source_mapping']) ? $settings['source_mapping'] : array(),
    );
  }

  /**
   * Configuration form for source mapping.
   */
  public function sourceMappingForm(&$form, &$form_state, $settings) {
    $form['#tree'] = TRUE;
    $form['source_mapping'] = array(
      '#type' => 'container',
    );
    if (!empty($settings['civicrm_types'])) {
      ctools_include('plugins');
      $plugins = ctools_get_plugins('crm_core_data_import', 'crm_core_data_import_source_civicrm_entity_type');
      foreach ($settings['civicrm_types'] as $civicrm_type) {
        // If need to display on source mapping form.
        // See "source_mapping" property of plugin.
        if ($plugins[$civicrm_type['plugin_name']]['source_mapping']) {
          $plugin = crm_core_data_import_load_plugin_instance('crm_core_data_import_source_civicrm_entity_type', $civicrm_type['plugin_name']);
          $form['source_mapping'][$civicrm_type['plugin_name']] = array(
            '#type' => 'fieldset',
            '#title' => $plugins[$civicrm_type['plugin_name']]['entity_name'],
          );
          foreach ($civicrm_type['entity_types'] as $entity_type_key => $entity_type) {
            if (!empty($entity_type)) {
              $form['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key] = array(
                '#type' => 'container',
              );
              $form['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['source_entity'] = array(
                '#type' => 'value',
                '#value' => $entity_type_key,
              );
              $form['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['source_entity_markup'] = array(
                '#markup' => $entity_type_key,
              );
              $form['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['destination_entity'] = array(
                '#type' => 'select',
                '#options' => array(
                  'new' => t('New'),
                  'existing' => t('Existing'),
                ),
                '#default_value' => !empty($settings['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['destination_entity']) ? $settings['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['destination_entity'] : '',
              );
              $form['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['destination_entity_new'] = array(
                '#type' => 'textfield',
                '#states' => array(
                  'visible' => array(
                    ':input[name="source_mapping[' . $civicrm_type['plugin_name'] . '][' . $entity_type_key . '][destination_entity]"]' => array('value' => 'new'),
                  ),
                ),
                '#default_value' => !empty($settings['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['destination_entity_new']) ? $settings['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['destination_entity_new'] : '',
              );
              $form['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['destination_entity_existing'] = array(
                '#type' => 'select',
                '#states' => array(
                  'visible' => array(
                    ':input[name="source_mapping[' . $civicrm_type['plugin_name'] . '][' . $entity_type_key . '][destination_entity]"]' => array('value' => 'existing'),
                  ),
                ),
                '#options' => $plugin->getDrupalTypes(),
                '#default_value' => !empty($settings['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['destination_entity_existing']) ? $settings['source_mapping'][$civicrm_type['plugin_name']][$entity_type_key]['destination_entity_existing'] : '',
              );
            }
          }
        }
      }
    }
  }

  /**
   * Validation handler for sourceMappingForm().
   */
  public function sourceMappingFormValidate(&$form, &$form_state, $settings) {}

  /**
   * Submission handler for sourceMappingForm().
   */
  public function sourceMappingFormSubmit(&$form, &$form_state, $settings) {
    return array(
      'source_mapping' => $form_state['values']['source_mapping'],
      'civicrm_types' => !empty($settings['civicrm_types']) ? $settings['civicrm_types'] : array(),
    );
  }

  /**
   * Returns available fields for mapping.
   */
  public function getFields($settings) {
    $fields = array();
    if (!empty($settings['civicrm_types'])) {
      foreach ($settings['civicrm_types'] as $civicrm_type) {
        if (!empty($civicrm_type['entity_types'])) {
          foreach ($civicrm_type['entity_types'] as $entity_type => $active) {
            if (!empty($active)) {
              $plugin = crm_core_data_import_load_plugin_instance('crm_core_data_import_source_civicrm_entity_type', $civicrm_type['plugin_name']);
              $fields += $plugin->getEntityFields($entity_type);
            }
          }
        }
      }
    }
    return $fields;
  }

  /**
   * Returns MigrationSource instance.
   */
  public function migrateSource($settings, $drupal_entity_type, $drupal_entity_bundle, $mapping) {
    if (!empty($settings['civicrm_types'])) {
      ctools_include('plugins');
      $civicrm_entity_type = $this->getCiviCrmEntityTypeFromMapping($mapping, $drupal_entity_type, $drupal_entity_bundle);
      $civicrm_entity_bundle = $this->getCiviCrmEntityBundleFromMapping($mapping, $drupal_entity_type, $drupal_entity_bundle);

      return new EntityListMigrateSource($civicrm_entity_type, $civicrm_entity_bundle);
    }
    return FALSE;
  }

  /**
   * Returns list of entities which has relationships.
   */
  public function getEntitiesForRelationships($settings, $item, $importer) {
    $list = array();
    if (!empty($settings['relations_values'])) {
      foreach ($settings['relations_values'] as $relationship_type_id => $relation) {
        $civicrm_destination_ids = array();
        if (!empty($relation['relation_type'])) {
          $data = array(
            'source' => $this->getDrupalEntityInfoFromMapping($importer->getMappingSettings(), 'Contact', $relation['source_entity']),
            'destination' => $this->getDrupalEntityInfoFromMapping($importer->getMappingSettings(), 'Contact', $relation['destination_entity']),
          );

          // Get source data.
          $this->getSourceResourceData($data, $item, $importer);

          // Fill civicrm_destination_ids with source ids and list of items.
          if (!empty($data['source']) && !empty($data['destination'])) {
            $this->getRelationshipCiviCrmDestinationIds($civicrm_destination_ids, $data['remote_source_id'], $relationship_type_id);
            $civicrm_destination_ids = array_unique($civicrm_destination_ids);
            if (!empty($data['table']) && !empty($civicrm_destination_ids)) {
              $this->buildItemList($list, $data, $civicrm_destination_ids, $relation);
            }
          }
        }
      }
    }

    return $list;
  }

  /**
   * Returns related list entities from migration results for references.
   */
  public function getEntitiesForReferences($settings, $item, $importer) {
    $list = array();
    if (!empty($settings)) {
      foreach ($settings['fields'] as $plugin_data) {
        if (!$this->entityIsSourceReference($item['entity_type'], $item['bundle'], $item['delta'], $plugin_data['source'])) {
          continue;
        }

        $data = array(
          'destination' => array(),
          'source' => array(),
        );

        // Get types, bundles and deltas of drupal entity.
        list($data['source']['type'], $data['source']['bundle'], $data['source']['delta']) = explode(':', $plugin_data['source']);
        list($data['destination']['type'], $data['destination']['bundle'], $data['destination']['delta']) = explode(':', $plugin_data['destination']);

        // Get source data.
        $this->getSourceResourceData($data, $item, $importer);

        // Fill civicrm_destination_ids with source ids and list of items.
        $civicrm_destination_ids = array();
        $this->getReferenceCiviCrmDestinationIds($civicrm_destination_ids, $data['remote_source_id'], $importer, $plugin_data);
        $civicrm_destination_ids = array_unique($civicrm_destination_ids);
        if (!empty($data['table']) && !empty($civicrm_destination_ids)) {
          $this->buildItemList($list, $data, $civicrm_destination_ids, $plugin_data);
        }
      }
    }

    return $list;
  }

  /**
   * Returns related list entities from migration results for users.
   */
  public function getEntitiesForUsers($settings, $item, $importer) {
    return array();
  }

  /**
   * Add destination id for relationships.
   */
  public function getRelationshipCiviCrmDestinationIds(&$civicrm_destination_ids, $source_id, $relationship_type_id) {
    $civicrm_relations = crm_core_data_import_civicrm_api('relationship', 'get',
      array(
        'contact_id_a' => $source_id,
        'relationship_type_id' => $relationship_type_id,
      )
    );
    foreach ($civicrm_relations as $civicrm_relation) {
      if (!empty($civicrm_relation['contact_id_b'])) {
        $civicrm_destination_ids[] = $civicrm_relation['contact_id_b'];
      }
    }
  }

  /**
   * Add destination id for references.
   */
  public function getReferenceCiviCrmDestinationIds(&$civicrm_destination_ids, $source_id, $importer, $reference) {
    $mapping = $importer->getMappingSettings();

    list($entity_type, $entity_bundle, $delta, $drupal_field_name) = explode(':', $reference['reference_type']);
    $civicrm_fields = $this->getAppropriateCiviCrmField($entity_type, $entity_bundle, $delta, $drupal_field_name, $mapping);

    if (!empty($civicrm_fields)) {
      $options = array(
        'activity_id' => $source_id,
        'sequential' => 1,
      );

      foreach ($civicrm_fields as $civicrm_field) {
        $options['return.' . $civicrm_field] = 1;
      }

      $civicrm_activity = crm_core_data_import_civicrm_api('Activity', 'get', $options);
      $civicrm_activity = reset($civicrm_activity);

      foreach ($civicrm_fields as $civicrm_field) {
        if (!empty($civicrm_activity[$civicrm_field])) {
          if (is_array($civicrm_activity[$civicrm_field])) {
            foreach ($civicrm_activity[$civicrm_field] as $target) {
              $civicrm_destination_ids[] = $target;
            }
          }
          else {
            $civicrm_destination_ids[] = $civicrm_activity[$civicrm_field];
          }
        }
      }
    }
  }

  /**
   * Returns source resource data for settings plugin.
   */
  public function getSourceResourceData(&$data, $item, $importer) {
    // Get migration instance.
    $machine_name = _crm_core_data_import_migration_machine_name($importer->id, $data['destination']['type'], $data['destination']['bundle'], $data['destination']['delta']);
    $data['table'] = _crm_core_data_import_map_table_get($machine_name);

    // Get ids of drupal entity and remote source entity.
    $data['remote_source_id'] = $item['sourceid1'];
    $data['drupal_source_id'] = $item['entity_id'];

    return $data;
  }

  /**
   * Build list of items for settings plugin.
   */
  public function buildItemList(&$list, $data, $civicrm_destination_ids, $plugin_data) {
    foreach ($civicrm_destination_ids as $civicrm_destination_id) {
      // Get id of drupal entity which should be target in source plugin.
      $target_entity_id = $this->getDrupalDestinationId($data['table'], $civicrm_destination_id);

      if (!empty($data['drupal_source_id']) && !empty($target_entity_id)) {
        $source_entity = new stdClass();
        $destination_entity = new stdClass();
        $source_entity->id = $data['drupal_source_id'];
        $destination_entity->id = $target_entity_id;

        // Add entity type to easily check entity type in settings.
        $source_entity->entity_type = $data['source']['type'];
        $destination_entity->entity_type = $data['destination']['type'];

        // Add data to list.
        $list[] = array(
          'source_entity' => $source_entity,
          'destination_entity' => $destination_entity,
          'plugin_data' => $plugin_data,
        );
      }
    }
  }

  /**
   * Return appropriate fields which exist in mapping for certain Drupal fields.
   */
  public function getAppropriateCiviCrmField($entity_type, $entity_bundle, $delta, $field_name, $mapping) {
    $fields = array();

    // Build entity key string.
    $entity_controller_type = crm_core_data_import_get_destination_entity_controller($entity_type);
    $entity_key = implode(':',
      array(
        $entity_controller_type,
        $entity_bundle,
        $delta,
      )
    );

    // Get appropriate field name.
    if (!empty($mapping[$entity_key])) {
      foreach ($mapping[$entity_key] as $mapping_item) {
        if (!empty($mapping_item['destination_field']) && $mapping_item['destination_field'] == $field_name) {
          list(,, $civicrm_field_name) = explode(':', $mapping_item['source_field']);
          $fields[] = $civicrm_field_name;
        }
      }
    }
    return $fields;
  }

  /**
   * Returns Drupal entity id for destination.
   */
  public function getDrupalDestinationId($table, $destination_id) {
    return db_select($table, 'map')
      ->fields('map', array('destid1'))
      ->condition('map.sourceid1', $destination_id)
      ->execute()
      ->fetchField();
  }

  /**
   * Returns CiviCRM entity type based on mapping.
   */
  public function getCiviCrmEntityTypeFromMapping($mapping, $drupal_entity_type, $drupal_entity_bundle) {
    $entity_data = $this->getCiviCrmEntityInfoFromMapping($mapping, $drupal_entity_type, $drupal_entity_bundle);
    return $entity_data['type'];
  }

  /**
   * Returns CiviCRM entity bundle based on mapping.
   */
  public function getCiviCrmEntityBundleFromMapping($mapping, $drupal_entity_type, $drupal_entity_bundle) {
    $entity_data = $this->getCiviCrmEntityInfoFromMapping($mapping, $drupal_entity_type, $drupal_entity_bundle);
    return $entity_data['bundle'];
  }

  /**
   * Returns CiviCRM delta bundle based on mapping.
   */
  public function getCiviCrmEntityDeltaFromMapping($mapping, $drupal_entity_type, $drupal_entity_bundle) {
    $entity_data = $this->getCiviCrmEntityInfoFromMapping($mapping, $drupal_entity_type, $drupal_entity_bundle);
    return $entity_data['delta'];
  }

  /**
   * Returns CiviCRM entity info based on mapping.
   */
  public function getCiviCrmEntityInfoFromMapping($mapping, $drupal_entity_type, $drupal_entity_bundle) {
    $controller = crm_core_data_import_get_destination_entity_controller($drupal_entity_type);
    foreach ($mapping as $mapping_key => $mapping_item) {
      list($entity_type, $entity_bundle,) = explode(':', $mapping_key);
      if ($entity_type == $controller && $entity_bundle == $drupal_entity_bundle) {
        list($civicrm_entity_type, $civicrm_entity_bundle, $delta) = explode(':', $mapping_item['primary']['source_field_primary']);
        return array(
          'type' => $civicrm_entity_type,
          'bundle' => $civicrm_entity_bundle,
          'delta' => $delta,
        );
      }
    }
  }

  /**
   * Returns Drupal entity type based on mapping.
   */
  public function getDrupalEntityTypeFromMapping($mapping, $civicrm_entity_type, $civicrm_entity_bundle) {
    $entity_data = $this->getDrupalEntityInfoFromMapping($mapping, $civicrm_entity_type, $civicrm_entity_bundle);
    return $entity_data['type'];
  }

  /**
   * Returns Drupal entity bundle based on mapping.
   */
  public function getDrupalEntityBundleFromMapping($mapping, $civicrm_entity_type, $civicrm_entity_bundle) {
    $entity_data = $this->getDrupalEntityInfoFromMapping($mapping, $civicrm_entity_type, $civicrm_entity_bundle);
    return $entity_data['bundle'];
  }

  /**
   * Returns Drupal entity delta based on mapping.
   */
  public function getDrupalEntityDeltaFromMapping($mapping, $civicrm_entity_type, $civicrm_entity_bundle) {
    $entity_data = $this->getDrupalEntityInfoFromMapping($mapping, $civicrm_entity_type, $civicrm_entity_bundle);
    return $entity_data['delta'];
  }

  /**
   * Returns Drupal entity info based on mapping.
   */
  public function getDrupalEntityInfoFromMapping($mapping, $civicrm_desired_entity_type, $civicrm_desired_entity_bundle) {
    foreach ($mapping as $mapping_key => $mapping_item) {
      if (!empty($mapping_item['primary']['source_field_primary'])) {
        list($civicrm_entity_type, $civicrm_entity_bundle, $delta) = explode(':', $mapping_item['primary']['source_field_primary']);
        if ($civicrm_entity_type == $civicrm_desired_entity_type && $civicrm_entity_bundle == $civicrm_desired_entity_bundle) {
          list($drupal_controller_entity_type, $drupal_entity_bundle, $delta) = explode(':', $mapping_key);
          return array(
            'type' => crm_core_data_import_get_destination_entity_type($drupal_controller_entity_type),
            'bundle' => $drupal_entity_bundle,
            'delta' => $delta,
          );
        }
      }
    }
  }

  /**
   * Check is this entity a source of field for references.
   */
  public function entityIsSourceReference($entity_type, $bundle, $delta, $source_key) {
    list($source_type, $source_bundle, $source_delta) = explode(':', $source_key);
    return ($entity_type == $source_type && $bundle == $source_bundle && $delta == $source_delta);
  }

  /**
   * Alter for source fields on the mapping form.
   */
  public function mappingSourceFieldsAlter(&$fields, $entity_type, $entity_bundle, $importer) {
    $source_fields = array('default_value' => t('(provide value)'));
    if (!empty($importer->source_settings['source_mapping'])) {
      $source_mapping = $importer->source_settings['source_mapping'];

      if ($entity_type == 'crm_core_contact' && !empty($source_mapping['CivicrmContactEntityType'])) {
        $mapping_values = $source_mapping['CivicrmContactEntityType'];
        $civicrm_entity_type = 'Contact';
      }

      if ($entity_type == 'crm_core_activity' && !empty($source_mapping['CivicrmActivityEntityType'])) {
        $mapping_values = $source_mapping['CivicrmActivityEntityType'];
        $civicrm_entity_type = 'Activity';
      }

      // Loop over source mapping to restrict available options.
      if (!empty($mapping_values) && !empty($civicrm_entity_type)) {
        foreach ($mapping_values as $value) {
          if ($value['destination_entity_existing'] == $entity_bundle && !empty($fields[$civicrm_entity_type . ':' . $value['source_entity']])) {
            $source_fields += array($civicrm_entity_type . ':' . $value['source_entity'] => $fields[$civicrm_entity_type . ':' . $value['source_entity']]);
          }
        }
      }
    }

    // If there is not only default_value.
    if (count($source_fields) > 1) {
      $fields = $source_fields;
    }
  }
}

/**
 * Class EntityListMigrateSource which contains list of entities.
 *
 * This class is wrapper around CiviCRM entities.
 */
class EntityListMigrateSource extends MigrateSource {
  protected $currentId;
  protected $entityList;
  protected $entityType;
  protected $bundle;

  /**
   * {@inheritdoc}
   */
  public function __construct($civicrm_entity_type, $civicrm_entity_bundle, $options = array()) {
    parent::__construct($options);
    $this->entityList = array();
    $this->entityType = $civicrm_entity_type;
    $this->bundle = $civicrm_entity_bundle;
  }

  /**
   * {@inheritdoc}
   */
  public function __toString() {
    return t('Available %num entities', array('%num' => $this->computeCount()));
  }

  /**
   * {@inheritdoc}
   */
  public function fields() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function computeCount() {
    // @TODO should be improved.
    if ($this->entityType == 'Contact') {
      return crm_core_data_import_single_query("SELECT count(*) FROM civicrm_contact WHERE contact_type = '$this->bundle' AND is_deleted = 0");
    }

    if ($this->entityType == 'Activity') {
      $activity_type = crm_core_data_get_civicrm_activity_type($this->bundle);
      if (!empty($activity_type['value'])) {
        $activity_type_id = $activity_type['value'];
        return crm_core_data_import_single_query("SELECT count(*) FROM civicrm_activity WHERE activity_type_id = '$activity_type_id' AND is_deleted = 0");
      }
    }

    return 0;
  }

  /**
   * {@inheritdoc}
   */
  public function performRewind() {
    $this->currentId = 0;

    // Set maximum items for get via CiviCRM API.
    $options = array('rowCount' => PHP_INT_MAX);
    $this->attachBundleCriteria($options);

    // @TODO there we can use custom query to get all ids.
    // Extract ids.
    $entity_list = crm_core_data_import_civicrm_api($this->entityType, 'get', $options);
    $this->entityList = array_keys($entity_list);
    drupal_alter('crm_core_data_import_civicrm_source_entity_list', $this->entityList, $this->entityType, $this->bundle);
  }

  /**
   * {@inheritdoc}
   */
  public function getNextRow() {
    if (!empty($this->entityList[$this->currentId])) {
      $row = $this->fetchItem();
      $this->currentId++;
      return $row;
    }
    else {
      return NULL;
    }
  }

  /**
   * Create object based on source data.
   */
  public function processObjectProperties($source_item) {
    $item = new stdClass();
    foreach ($source_item as $property_key => $value) {
      // Add field collection properties to source in expected format.
      // The format is property key with array of property values.
      if (isset($value['version'])) {
        list(, $field_name,) = explode('.', $property_key);
        foreach ($value['values'] as $delta => $properties) {
          foreach ($properties as $property_key => $property_value) {
            $item->{$this->entityType . ':' . $this->bundle . ':' . $field_name . ':' . $property_key}[$delta] = $property_value;
          }
        }
      }
      else {
        $item->{$this->entityType . ':' . $this->bundle . ':' . $property_key} = $value;
      }
    }
    return $item;
  }

  /**
   * Attach filter by bundle to options.
   */
  public function attachBundleCriteria(&$options) {
    if ($this->entityType == 'Contact') {
      $options['contact_type'] = $this->bundle;
    }
    if ($this->entityType == 'Activity') {
      $activity_type = crm_core_data_get_civicrm_activity_type($this->bundle);
      $options['activity_type_id'] = $activity_type['value'];
    }
  }

  /**
   * Attach filter by bundle to options.
   */
  public function attachIdCriteria(&$options) {
    if ($this->entityType == 'Contact') {
      $options['contact_id'] = $this->entityList[$this->currentId];
    }
    if ($this->entityType == 'Activity') {
      $options['activity_id'] = $this->entityList[$this->currentId];
    }
  }

  /**
   * Fetch item from CiviCRM.
   */
  public function fetchItem() {
    $options = array();
    $this->attachBundleCriteria($options);
    $this->attachIdCriteria($options);

    if ($this->entityType == 'Contact') {
      foreach (_crm_core_data_import_contact_field_collection_fields() as $collection_item) {
        $options['api.' . $collection_item . '.get'] = 1;
      }
    }

    $custom_fields_options = $this->attachCustomFields($options, $this->entityType);

    $core_fields = crm_core_data_import_civicrm_api($this->entityType, 'get', $options);
    $custom_fields = crm_core_data_import_civicrm_api($this->entityType, 'get', $custom_fields_options);

    // Need to check for empty data just in case to avoid exceptions.
    $current_id = $this->entityList[$this->currentId];
    $core_fields = !empty($core_fields[$current_id]) ? $core_fields[$current_id] : array();
    $custom_fields = !empty($custom_fields[$current_id]) ? $custom_fields[$current_id] : array();
    $item = array_merge($core_fields, $custom_fields);

    return $this->processObjectProperties($item);
  }

  /**
   * Attach custom fields.
   */
  public function attachCustomFields($options, $civicrm_entity_type) {
    $fields = _crm_core_data_import_civicrm_get_custom_fields($civicrm_entity_type);
    $options['return'] = array_keys($fields);
    return $options;
  }
}
