<?php
/**
 * @file
 * References settings handler for CRM Core Data Import.
 */

$plugin = array(
  'label' => t('References'),
  'handler' => array(
    'class' => 'ReferencesImportSettings',
  ),
);

class ReferencesImportSettings extends ReferencesImportSettingsBase {
  /**
   * Returns TRUE if conditions match for settings.
   */
  public function displayConditions($importer) {
    $active = FALSE;
    $settings = $importer->getSettings();
    $entity_types = $this->entityList($settings);
    if (!is_a($importer->source_plugin, 'CivicrmDataSourceHandler')) {
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
  public function postImport($importer, $item) {
    $settings = $importer->getSettings();
    if (!empty($settings['references']['enable']) &&!empty($settings['references']['fields']) && !is_a($importer->source_plugin, 'CivicrmDataSourceHandler')) {
      $list = $importer->source_plugin->getEntitiesForReferences($settings['references'], $item, $importer);
      foreach ($list as $item) {
        $this->createReference($item['source_entity'], $item['destination_entity'], $item['plugin_data']);
      }
    }
  }
}
