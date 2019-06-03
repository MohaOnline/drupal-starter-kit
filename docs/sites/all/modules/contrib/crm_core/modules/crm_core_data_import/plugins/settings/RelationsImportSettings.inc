<?php
/**
 * @file
 * Relations settings handler for CRM Core Data Import.
 */

$plugin = array(
  'label' => t('Relationships'),
  'handler' => array(
    'class' => 'RelationsImportSettings',
  ),
);

class RelationsImportSettings extends RelationsImportSettingsBase {
  /**
   * Returns TRUE if conditions match for settings.
   */
  public function displayConditions($importer) {
    $active = FALSE;
    $settings = $importer->getSettings();
    // If enabled crm_core_relationship module and source plugin not CivicrmDataSourceHandler.
    if (module_exists('crm_core_relationship') && !is_a($importer->source_plugin, 'CivicrmDataSourceHandler')) {
      $entities = $this->entityList($settings);
      foreach ($entities as $entity_info) {
        if ($entity_info['entity_type'] == 'crm_core_contact') {
          $active = TRUE;
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
    
    if (!empty($settings['relations']['enable']) &&!empty($settings['relations']['fields']) && !is_a($importer->source_plugin, 'CivicrmDataSourceHandler')) {
      $list = $importer->source_plugin->getEntitiesForRelationships($settings['relations'], $item, $importer);
      
      // Process list of the related entities.
      foreach ($list as $item2) {
        if(empty($item2['source_entity']->id) || empty($item2['destination_entity']->id)) {
          // throw an error if one or more of the entities are missing
          $message = 'Error importing relationships: The entity ID for the source or destination cannot be found.<br>';
          $message .= 'Process Name: "@title"<br>Relationship Importer: "@importer"<br>Source ID: "@source"<br>Destination ID: "@destination"';
          watchdog(
            t('CRM Core Data Import'), 
            t($message), 
            $variables = array(
            	'@title' => $importer->title, 
            	'@importer' => $importer->source, 
            	'@source' => $item2['source_entity']->id, 
            	'@destination' => $item2['destination_entity']->id
            ), 
            $severity = WATCHDOG_ERROR
          );
        } else {
          $this->createRelation($item2['source_entity'], $item2['destination_entity'], $item2['plugin_data']['relation_type']);
        }
      }
    }
  }
}
