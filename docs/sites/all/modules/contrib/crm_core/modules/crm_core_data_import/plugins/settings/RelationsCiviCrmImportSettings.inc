<?php
/**
 * @file
 * Relations settings handler for CRM Core Data Import.
 */

$plugin = array(
  'label' => t('Relationships CiviCRM'),
  'handler' => array(
    'class' => 'RelationsCiviCrmImportSettings',
  ),
);

class RelationsCiviCrmImportSettings extends RelationsImportSettingsBase {
  /**
   * Returns TRUE if conditions match for settings.
   */
  public function displayConditions($importer) {
    $active = FALSE;
    $settings = $importer->getSettings();
    // If enabled crm_core_relationship module and source plugin not CivicrmDataSourceHandler.
    if (module_exists('crm_core_relationship') && is_a($importer->source_plugin, 'CivicrmDataSourceHandler')) {
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
   * Configuration form for settings plugin.
   */
  public function configForm(&$form, &$form_state, $importer, $label) {
    $settings = $importer->getSettings();
    $source_settings = $importer->getSourceSettings();

    $available_relations = array(
      'new' => t('New relation'),
    ) + $this->getAvailableRelations($settings);

    $form['relations'] = array(
      '#type' => 'container',
      '#weight' => -10,
    );
    $form['relations']['title'] = array(
      '#type' => 'item',
      '#title' => $label,
    );
    $form['relations']['enable'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
    $form['relations']['civicrm'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
    $form['relations']['relations_values'] = array(
      '#type' => 'container',
      '#weight' => -10,
    );
    if (!empty($source_settings['civicrm_types']['CivicrmRelationshipEntityType']['entity_types'])) {
      foreach ($source_settings['civicrm_types']['CivicrmRelationshipEntityType']['entity_types'] as $civicrm_relation => $active) {
        if ($active) {
          $civicrm_relation = crm_core_data_import_civicrm_api('relationship_type', 'get', array('id' => $civicrm_relation));
          $relation_type_default_value = '';
          $relation = reset($civicrm_relation);

          // Get relation type default value.
          if (!empty($settings['relations']['relations_values'][$relation['id']]['relation_type']) && $this->relationIsAlreadyCreated($settings['relations']['relations_values'][$relation['id']]['relation_type'], $available_relations)) {
            $relation_type_default_value = $settings['relations']['relations_values'][$relation['id']]['relation_type'];
          }
          elseif (!empty($settings['relations']['relations_values'][$relation['id']]['relation_type']) && $this->relationShouldBeCreated($settings['relations']['relations_values'][$relation['id']]['relation_type'], $available_relations, $settings['relations']['relations_values'][$relation['id']]['new_relation_name'])) {
            $relation_type_default_value = 'new';
          }

          $form['relations']['relations_values'][$relation['id']] = array(
            '#type' => 'fieldset',
            '#title' => $relation['description'],
          );
          $form['relations']['relations_values'][$relation['id']]['civicrm_relation_id'] = array(
            '#type' => 'value',
            '#value' => $relation['id'],
          );
          $form['relations']['relations_values'][$relation['id']]['source_entity'] = array(
            '#type' => 'value',
            '#value' => $relation['contact_type_a'],
          );
          $form['relations']['relations_values'][$relation['id']]['source_entity_markup'] = array(
            '#markup' => $relation['contact_type_a'],
          );
          $form['relations']['relations_values'][$relation['id']]['source_relation_markup'] = array(
            '#markup' => $relation['name_a_b'],
          );
          $form['relations']['relations_values'][$relation['id']]['relation_type'] = array(
            '#type' => 'select',
            '#options' => $available_relations,
            '#empty_option' => t('-- Do not import --'),
            '#default_value' => $relation_type_default_value,
          );
          $form['relations']['relations_values'][$relation['id']]['new_relation_name']  = array(
            '#type' => 'machine_name',
            '#states' => array(
              'visible' => array(
                ':input[name="relations[relations_values][' . $relation['id'] . '][relation_type]"]' => array('value' => 'new'),
              ),
            ),
            '#default_value' => !empty($settings['relations']['relations_values'][$relation['id']]['new_relation_name']) ? $settings['relations']['relations_values'][$relation['id']]['new_relation_name'] : '',
            '#machine_name' => array(
              'exists' => 'crm_core_data_import_relation_exist',
            ),
            '#required' => FALSE,
          );
          $form['relations']['relations_values'][$relation['id']]['destination_entity'] = array(
            '#type' => 'value',
            '#value' => $relation['contact_type_b'],
          );
          $form['relations']['relations_values'][$relation['id']]['destination_entity_markup'] = array(
            '#markup' => $relation['contact_type_b'],
          );
        }
      }
    }
  }

  /**
   * Validation handler for configForm().
   */
  public function configFormValidate(&$form, &$form_state, $importer) {
    $relation_settings =& $form_state['values']['relations'];
    if (!empty($relation_settings['relations_values'])) {
      foreach ($relation_settings['relations_values'] as &$relation) {
        if ($this->needCreateRelationInstance($relation)) {
          if (empty($relation['new_relation_name'])) {
            form_set_error('relations[relations_values][' . $relation['id'] . '][relation_type]', t('Relationship name cannot be empty.'));
          }
        }
      }
    }
  }

  /**
   * Submission handler for configForm().
   */
  public function configFormSubmit(&$form, &$form_state, $importer) {
    $relation_settings =& $form_state['values']['relations'];
    if (!empty($relation_settings['relations_values'])) {
      foreach ($relation_settings['relations_values'] as &$relation) {
        if ($this->needCreateRelationInstance($relation)) {
          $this->createRelationInstance($relation);
          // Set new relationship as selected.
          $relation['relation_type'] = $relation['new_relation_name'];
        }
      }
    }
    return array('relations' => $form_state['values']['relations']);
  }

  /**
   * Performs when import was successful.
   */
  public function postImport($importer, $item) {
    $settings = $importer->getSettings();
    if (!empty($settings['relations']['enable']) &&!empty($settings['relations']['relations_values']) && $item['entity_type'] == 'crm_core_contact' && is_a($importer->source_plugin, 'CivicrmDataSourceHandler')) {
      $list = $importer->source_plugin->getEntitiesForRelationships($settings['relations'], $item, $importer);
      // Process list of the related entities.
      foreach ($list as $item) {
        $this->createRelation($item['source_entity'], $item['destination_entity'], $item['plugin_data']['relation_type']);
      }
    }
  }

  /**
   * Helper to determine do we need create relation instance.
   */
  public function needCreateRelationInstance($relation) {
    return (!empty($relation['relation_type']) && $relation['relation_type'] == 'new');
  }

  /**
   * Create relation instance for CiviCRM relation.
   */
  public function createRelationInstance($relation) {
    $civicrm_relation = crm_core_data_import_civicrm_api('relationship_type', 'get', array('id' => $relation['civicrm_relation_id']));
    $civicrm_relation = reset($civicrm_relation);
    $record = array(
      'relation_type' => $relation['new_relation_name'],
      'min_arity' => 2,
      'max_arity' => 2,
      'label' => $civicrm_relation['label_a_b'],
      'reverse_label' => $civicrm_relation['label_b_a'],
      'directional' => 1,
      'transitive' => 1,
      'r_unique' => 1,
      // @TODO bundles.
      'source_bundles' => array('crm_core_contact:*' => 'crm_core_contact:*'),
      'target_bundles' => array('crm_core_contact:*' => 'crm_core_contact:*'),
    );
    relation_type_save($record);
  }

  /**
   * Returns TRUE if relation already created.
   */
  public function relationIsAlreadyCreated($relation_type, $available_relations) {
    return !empty($relation_type) && $relation_type != 'new' && array_key_exists($relation_type, $available_relations);
  }

  /**
   * Returns TRUE if relation should be created.
   */
  public function relationShouldBeCreated($relation_type, $available_relations, $new_relation_name) {
    return !empty($relation_type) && !array_key_exists($relation_type, $available_relations) && !empty($new_relation_name);
  }
}
