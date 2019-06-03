<?php
/**
 * @file
 * Data source handler for CSV files
 */

$plugin = array(
  'label' => t('CSV'),
  'handler' => array(
    'class' => 'CsvDataSourceHandler',
  ),
);

class CsvDataSourceHandler extends CRMCoreDataSourceHandler {

  /**
   * Configuration form for CsvDataSourceHandler plugin.
   *
   * @param array $form
   *   Form elements.
   *
   * @param array $form_state
   *   Form state.
   *
   * @param array $settings
   *   Source plugin settings.
   */
  public function configForm(&$form, &$form_state, $settings) {
    $default_value = !empty($settings['file']->fid) ? $settings['file']->fid : FALSE;
    $delimiter_default_value = !empty($settings['delimiter']) ? $settings['delimiter'] : FALSE;

    $validators = array(
      'file_validate_extensions' => array('csv'),
      'file_validate_size' => array(100 * 1024 * 1024),
    );

    $form['file'] = array(
      '#type' => 'managed_file',
      '#required' => TRUE,
      '#title' => t('File to import'),
      '#upload_location' => 'public://',
      '#default_value' => $default_value,
      '#upload_validators' => $validators,
    );
    $form['delimiter'] = array(
      '#type' => 'select',
      '#required' => TRUE,
      '#title' => t('Delimiter'),
      '#options' => array(
        ',' => ',',
        ';' => ';',
        'TAB' => 'TAB',
        '|' => '|',
        '+' => '+',
      ),
      '#default_value' => $delimiter_default_value,
    );
  }

  /**
   * Validation handler for configForm().
   */
  public function configFormValidate(&$form, &$form_state, $settings) {
    $form_state['values']['file'] = file_load($form_state['values']['file']);
  }

  /**
   * Submission handler for configForm().
   */
  public function configFormSubmit(&$form, &$form_state, $settings, $importer_id = NULL) {
    // Add file usage.
    if (!empty($importer_id)) {
      file_usage_add($form_state['values']['file'], 'file', 'crm_core_data_import', $importer_id);
    }
    return array('file' => $form_state['values']['file'], 'delimiter' => $form_state['values']['delimiter']);
  }

  /**
   * Returns available fields for mapping.
   */
  public function getFields($settings) {
    $fields = array();
    if (!empty($settings['file'])) {
      $file_path = $this->getFilePath($settings);
      $delimiter = $this->getDelimiter($settings);
      ini_set('auto_detect_line_endings', TRUE);
      $file = fopen($file_path, 'r');
      if ($file) {
        // Get the first line of CSV file.
        $fields_values = fgetcsv($file, NULL, $delimiter);
        foreach ($fields_values as $field) {
          $fields[$field] = $field;
        }
        fclose($file);
      }
    }

    return $fields;
  }

  /**
   * Returns file path.
   */
  public function getFilePath($settings) {
    if (!empty($settings['file'])) {
      return drupal_realpath($settings['file']->uri);
    }
    return FALSE;
  }

  /**
   * Returns delimiter.
   */
  public function getDelimiter($settings) {
    if (!empty($settings['delimiter'])) {
      // Replace tab with html equivalent.
      if ($settings['delimiter'] == 'TAB') {
        $settings['delimiter'] = "\t";
      }
      return $settings['delimiter'];
    }
    return FALSE;
  }

  /**
   * Returns MigrationSource instance.
   */
  public function migrateSource($settings, $entity_type, $entity_bundle, $mapping) {
    $columns = array();
    $fields = $this->getFields($settings);
    foreach ($fields as $field) {
      $columns[] = array($field, $field);
    }
    $path = $this->getFilePath($settings);
    if ($path) {
      $delimiter = $this->getDelimiter($settings);
      return new MigrateSourceCSV($path, $columns, array('header_rows' => 1, 'delimiter' => $delimiter));
    }
    return FALSE;
  }

  /**
   * Returns related list entities from migration results.
   *
   * This method based on the rows of csv file and ids. As result we can associate entities by line.
   *
   * @todo: Allow more than one additional source key.
   */
  public function getListOfRelatedEntities($settings, $item, CRMCoreDataImport $importer) {
    $list = array();
    if (!empty($settings)) {
      foreach ($settings['fields'] as $plugin_data) {
        list($source_type, $source_bundle, $source_delta) = explode(':', $plugin_data['source']);
        list($destination_type, $destination_bundle, $destination_delta) = explode(':', $plugin_data['destination']);

        // If current entity match to source entity in the plugin data.
        if ($item['entity_type'] == $source_type && $item['bundle'] == $source_bundle && $item['delta'] == $source_delta) {
          $machine_name = _crm_core_data_import_migration_machine_name($importer->id, $destination_type, $destination_bundle, $destination_delta);
          $migration = Migration::getInstance($machine_name);

          // Get related entity.
          if ($migration) {
            $map = $migration->getMap();
            $target_entity_id = db_select($map->getMapTable(), 'map')
              ->fields('map', array('destid1'))
              ->condition('map.sourceid1', $item['relation_id1'])
              ->execute()
              ->fetchField();

            $source_entity = new stdClass();
            $destination_entity = new stdClass();
            $source_entity->id = $item['entity_id'];
            $destination_entity->id = $target_entity_id;

            // Add entity type to easily check entity type in settings.
            $source_entity->entity_type = $source_type;
            $destination_entity->entity_type = $destination_type;

            // Add data to list.
            $list[] = array(
              'source_entity' => $source_entity,
              'destination_entity' => $destination_entity,
              'plugin_data' => $plugin_data,
            );
          }
        }
      }
    }

    return $list;
  }

  /**
   * Returns related list entities from migration results for relationships.
   */
  public function getEntitiesForRelationships($settings, $item, $importer) {
    return $this->getListOfRelatedEntities($settings, $item, $importer);
  }

  /**
   * Returns related list entities from migration results for references.
   */
  public function getEntitiesForReferences($settings, $item, $importer) {
    return $this->getListOfRelatedEntities($settings, $item, $importer);
  }

  /**
   * Returns related list entities from migration results for users.
   */
  public function getEntitiesForUsers($settings, $item, $importer) {
    return $this->getListOfRelatedEntities($settings, $item, $importer);
  }
}
