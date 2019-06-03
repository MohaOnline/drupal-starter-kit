<?php
/**
 * @file
 * Base relations settings handler for CRM Core Data Import.
 */

abstract class RelationsImportSettingsBase extends CRMCoreDataImportSettings {

  /**
   * Configuration form for settings plugin.
   */
  public function configForm(&$form, &$form_state, $importer, $label) {
    $settings = $importer->getSettings();
    if (!empty($settings['relations']) && empty($form_state['values']['relations'])) {
      $form_state['values']['relations'] = $settings['relations'];
    }

    $state_settings = array(
      'visible' => array(
        ':input[name="relations[enable]"]' => array('checked' => TRUE),
      ),
    );

    $form['relations'] = array(
      '#type' => 'container',
      '#weight' => -10,
    );
    $form['relations']['title'] = array(
      '#type' => 'item',
      '#title' => $label,
    );
    $form['relations']['enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create relationships for data being imported?'),
      '#default_value' => !empty($form_state['values']['relations']['enable']) ? $form_state['values']['relations']['enable'] : FALSE,
    );
    $form['relations']['relation_type'] = array(
      '#type' => 'select',
      '#title' => t('Add relationship'),
      '#options' => $this->getAvailableRelations($settings),
      '#states' => $state_settings,
    );
    $form['relations']['add_relation'] = array(
      '#type' => 'button',
      '#id' => 'add-relation-button',
      '#value' => t('Add'),
      '#states' => $state_settings,
      '#name' => 'add-relation-button',
      '#ajax' => array(
        'callback' => '_crm_core_data_import_relations_callback',
        'method' => 'replace',
        'wrapper' => 'relations-form-fields-wrapper',
      ),
    );
    $form['relations']['fields'] = array(
      '#type' => 'container',
      '#states' => $state_settings,
      '#prefix' => '<div id="relations-form-fields-wrapper">',
      '#suffix' => '</div>',
      '#attributes' => array(
        'class' => array('settings-fieldset-wrapper'),
      ),
    );
    $this->formAttachRelations($form, $form_state, $settings);
  }

  /**
   * Validation handler for configForm().
   */
  public function configFormValidate(&$form, &$form_state, $importer) {}

  /**
   * Submission handler for configForm().
   */
  public function configFormSubmit(&$form, &$form_state, $importer) {
    unset($form_state['values']['relations']['relation_type']);
    unset($form_state['values']['relations']['add_relation']);
    if (!empty($form_state['values']['relations']['fields'])) {
      $form_state['values']['relations']['fields'] = $this->cleanFormValues($form_state['values']['relations']['fields']);
    }
    return array('relations' => $form_state['values']['relations']);
  }

  /**
   * Attach relation fields to form.
   */
  public function formAttachRelations(&$form, &$form_state, $settings) {
    $triggering_element = !empty($form_state['triggering_element']) ? $form_state['triggering_element'] : FALSE;

    if ($triggering_element['#id'] == 'add-relation-button' && !empty($form_state['values']['relations']['relation_type'])) {
      $form_state['values']['relations']['fields'][]['relation_type'] = $form_state['values']['relations']['relation_type'];
    }

    if (!empty($triggering_element['#parents'][3]) && $triggering_element['#parents'][3] == 'remove_field' && $triggering_element['#parents'][0] == 'relations') {
      $field_key = $triggering_element['#parents'][2];
      unset($form_state['values']['relations']['fields'][$field_key]);
    }

    if (!empty($form_state['values']['relations']['fields'])) {
      foreach ($form_state['values']['relations']['fields'] as $delta => $field) {
        $relation_type_object = relation_type_load($field['relation_type']);
        $source = crm_core_relationship_load_contact_types($relation_type_object, 0);
        $destination = crm_core_relationship_load_contact_types($relation_type_object, 1);
        $this->formAttachFieldRelation($form, $form_state, $settings, $source, $destination, $relation_type_object, $delta);
      }
    }
  }

  /**
   * Attach one relation field to form.
   */
  public function formAttachFieldRelation(&$form, &$form_state, $settings, $source, $destination, $relation_type_object, $delta) {
    $source_values = array();
    $destination_values = array();
    foreach ($source as $relation) {
      $list = $this->getMatchMappingEntity('crm_core_contact', $relation->type, $settings);
      foreach ($list as $key => $item) {
        $source_values[$key] = crm_core_data_import_get_destination_label($item['entity_controller_type'], $item['entity_bundle'], $item['delta']);
      }
    }
    foreach ($destination as $relation) {
      $list = $this->getMatchMappingEntity('crm_core_contact', $relation->type, $settings);
      foreach ($list as $key => $item) {
        $destination_values[$key] = crm_core_data_import_get_destination_label($item['entity_controller_type'], $item['entity_bundle'], $item['delta']);
      }
    }
    $form['relations']['fields'][$delta] = array(
      '#type' => 'fieldset',
      '#attributes' => array(
        'class' => array('settings-fieldset'),
      ),
    );
    $form['relations']['fields'][$delta]['source'] = array(
      '#type' => 'select',
      '#options' => $source_values,
      '#default_value' => !empty($form_state['values']['relations']['fields'][$delta]['source']) ? $form_state['values']['relations']['fields'][$delta]['source'] : FALSE,
    );
    $form['relations']['fields'][$delta]['label'] = array(
      '#type' => 'item',
      '#markup' => $relation_type_object->label,
    );
    $form['relations']['fields'][$delta]['destination'] = array(
      '#type' => 'select',
      '#options' => $destination_values,
      '#default_value' => !empty($form_state['values']['relations']['fields'][$delta]['destination']) ? $form_state['values']['relations']['fields'][$delta]['destination'] : FALSE,
    );
    $form['relations']['fields'][$delta]['relation_type'] = array(
      '#type' => 'value',
      '#value' => $relation_type_object->relation_type,
    );
    $form['relations']['fields'][$delta]['remove_field'] = array(
      '#type' => 'button',
      '#value' => t('Remove'),
      '#name' => drupal_html_id('relations-settings-remove-button-' . $delta),
      '#ajax' => array(
        'callback' => '_crm_core_data_import_relations_callback',
        'method' => 'replace',
        'wrapper' => 'relations-form-fields-wrapper',
      ),
    );
  }

  /**
   * Build a list of available relations relevant to current settings.
   */
  public function getAvailableRelations($settings) {
    // @TODO inversion.
    $relations = array();
    $entities = $this->entityList($settings);
    $entities_names = array();

    // A list with entity info.
    foreach ($entities as $entity_info) {
      $entities_names[] = $entity_info['entity_type'] . ':' . $entity_info['bundle'];
    }

    $relation_types = relation_get_types();

    foreach ($relation_types as $relation_type) {
      $available_source = FALSE;
      $available_destination = FALSE;

      // If empty, available for all bundles.
      if (empty($relation_type->source_bundles)) {
        $available_source = TRUE;
      }
      foreach ($relation_type->source_bundles as $source) {
        if (in_array($source, $entities_names) || $source == 'crm_core_contact:*') {
          $available_source = TRUE;
        }
      }

      // If empty, available for all bundles.
      if (empty($relation_type->target_bundles)) {
        $available_destination = TRUE;
      }
      foreach ($relation_type->target_bundles as $destination) {
        if (in_array($destination, $entities_names) || $destination == 'crm_core_contact:*') {
          $available_destination = TRUE;
        }
      }

      // If both relations exist in the settings.
      if ($available_source && $available_destination) {
        $relations[$relation_type->relation_type] = $relation_type->label;
      }
    }

    if (empty($relations)) {
      $relations[] = t('No available relations');
    }

    return $relations;
  }

  /**
   * Create relation between contacts.
   *
   * @param object $source_entity
   *   Source entity.
   * @param object $destination_entity
   *   Target entity.
   * @param string $relation_type
   *   Type of relation.
   */
  public function createRelation($source_entity, $destination_entity, $relation_type) {
    $source_entity = entity_load_single($source_entity->entity_type, $source_entity->id);
    $destination_entity = entity_load_single($destination_entity->entity_type, $destination_entity->id);
    if ($source_entity && $destination_entity) {
      // We cannot rely on relation_get_related_entity() because as stated in
      // documentation "Do not expect to get exactly what you want, especially
      // if you have multiple relations of the same type on the search entity."
      // We need to check more precisely here.
      if (!$this->relationExists($source_entity, $destination_entity, $relation_type)) {
        $endpoints = array(
          0 => array(
            'entity_type' => 'crm_core_contact',
            'entity_id' => $source_entity->contact_id,
            'entity_bundle' => $source_entity->type,
            'r_index' => 0,
            'entity_key' => $source_entity->type . ': ' . $source_entity->contact_id,
          ),
          1 => array(
            'entity_type' => 'crm_core_contact',
            'entity_id' => $destination_entity->contact_id,
            'entity_bundle' => $destination_entity->type,
            'r_index' => 1,
            'entity_key' => $destination_entity->type . ': ' . $destination_entity->contact_id,
          ),
        );
        $relation = relation_create($relation_type, $endpoints);
        relation_save($relation);
      }
    }
  }

  /**
   * Check if a relation exists between two contacts.
   *
   * @param CRMCoreContactEntity $source_entity
   *   Fully loaded source contact.
   * @param CRMCoreContactEntity $destination_entity
   *   Fully loaded source contact.
   * @param string $relation_type
   *   Relation type.
   *
   * @return int|NULL
   *    Relation ID if it exists, NULL otherwise.
   */
  public function relationExists(CRMCoreContactEntity $source_entity,CRMCoreContactEntity $destination_entity, $relation_type) {
    $entity_type = 'crm_core_contact';

    $query = db_select('field_data_endpoints', 'endpoint_src');
    $query->innerJoin('field_data_endpoints', 'endpoint_dst', "endpoint_src.entity_type = endpoint_dst.entity_type AND endpoint_src.bundle = endpoint_dst.bundle AND endpoint_src.deleted = endpoint_dst.deleted AND endpoint_src.entity_id = endpoint_dst.entity_id AND endpoint_src.endpoints_entity_type = endpoint_dst.endpoints_entity_type");
    $query
      ->condition('endpoint_src.entity_type', 'relation')
      ->condition('endpoint_src.bundle', $relation_type)
      ->condition('endpoint_src.deleted', 0)
      ->condition('endpoint_src.endpoints_entity_type', $entity_type)
      ->condition('endpoint_src.endpoints_entity_id', $source_entity->contact_id)
      ->condition('endpoint_dst.endpoints_entity_id', $destination_entity->contact_id)
      ->fields('endpoint_src', array('entity_id'));

    return $query->execute()->fetchField();
  }
}

/**
 * Callback for relations add button.
 */
function _crm_core_data_import_relations_callback(&$form, &$form_state) {
  return $form['relations']['fields'];
}
