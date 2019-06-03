<?php
/**
 * @file
 * References settings handler for CiviCRM.
 */

$plugin = array(
  'label' => t('References CiviCRM'),
  'handler' => array(
    'class' => 'ReferencesCiviCrmImportSettings',
  ),
);

class ReferencesCiviCrmImportSettings extends ReferencesImportSettingsBase {
  /**
   * Returns TRUE if conditions match for settings.
   */
  public function displayConditions($importer) {
    $active = FALSE;
    $settings = $importer->getSettings();
    $entity_types = $this->entityList($settings);
    if (is_a($importer->source_plugin, 'CivicrmDataSourceHandler')) {
      foreach ($entity_types as $entity_type) {
        $fields = field_info_instances($entity_type['entity_type'], $entity_type['bundle']);
        foreach ($fields as $field_name => $field) {
          $field_info = field_info_field($field_name);
          if ($field_info['type'] == 'entityreference') {
            $active = TRUE;
          }
        }
      }
    }
    return $active;
  }

  /**
   * Performs when import was successful.
   */
  public function postImport($importer, $imported_item) {
    $settings = $importer->getSettings();
    if (!empty($settings['references']['enable']) &&!empty($settings['references']['fields']) && is_a($importer->source_plugin, 'CivicrmDataSourceHandler')) {
      $list = $importer->source_plugin->getEntitiesForReferences($settings['references'], $imported_item, $importer);
      if (!empty($list)) {
        $source_entity = entity_load_single($imported_item['entity_type'], $imported_item['entity_id']);
        $changed = FALSE;
        foreach ($list as $item) {
          $this->attachReference($source_entity, $imported_item['entity_type'], $item['destination_entity'], $item['plugin_data'], $changed);
        }
        if ($changed) {
          entity_save($imported_item['entity_type'], $source_entity);
        }
      }
    }
  }

  /**
   * Create reference between entities.
   */
  public function attachReference(&$source_entity, $source_type, $destination_entity_info, $reference_settings, &$changed) {
    try {
      list(,,, $target_field) = explode(':', $reference_settings['reference_type']);
      $destination_entity = entity_load_single($destination_entity_info->entity_type, $destination_entity_info->id);
      $source_wrapper = entity_metadata_wrapper($source_type, $source_entity);

      if ($source_entity && $destination_entity) {
        $multivalue = FALSE;
        $field = field_info_field($target_field);

        if (!empty($field['cardinality']) && $field['cardinality'] != 1) {
          $multivalue = TRUE;
        }

        // Add value to entity.
        if ($source_entity && $destination_entity) {
          if ($multivalue) {
            $source_wrapper->{$target_field}[] = $destination_entity_info->id;
            $changed = TRUE;
          }
          else {
            $source_wrapper->{$target_field}->set($destination_entity_info->id);
            $changed = TRUE;
          }
        }
      }
    }
    catch (Exception $e) {
      watchdog('crm_core_data_import', $e->getMessage(), array(), WATCHDOG_ERROR);
    }
  }
}
