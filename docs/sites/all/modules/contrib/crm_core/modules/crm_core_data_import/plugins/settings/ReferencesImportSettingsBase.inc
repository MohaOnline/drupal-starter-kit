<?php
/**
 * @file
 * Base references settings handler for CRM Core Data Import.
 */

abstract class ReferencesImportSettingsBase extends CRMCoreDataImportSettings {

  /**
   * Configuration form for settings plugin.
   */
  public function configForm(&$form, &$form_state, $importer, $label) {
    $settings = $importer->getSettings();
    if (!empty($settings['references']) && empty($form_state['values']['references'])) {
      $form_state['values']['references'] = $settings['references'];
    }

    $state_settings = array(
      'visible' => array(
        ':input[name="references[enable]"]' => array('checked' => TRUE),
      ),
    );

    $form['references'] = array(
      '#type' => 'container',
      '#weight' => -9,
    );
    $form['references']['title'] = array(
      '#type' => 'item',
      '#title' => $label,
    );
    $form['references']['enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create references for data being imported?'),
      '#default_value' => !empty($form_state['values']['references']['enable']) ? $form_state['values']['references']['enable'] : FALSE,
    );
    $form['references']['reference_type'] = array(
      '#type' => 'select',
      '#title' => t('Add reference'),
      '#options' => $this->getAvailableReferences($settings),
      '#states' => $state_settings,
    );
    $form['references']['add_reference'] = array(
      '#type' => 'button',
      '#id' => 'add-reference-button',
      '#value' => t('Add'),
      '#name' => 'add-reference-button',
      '#states' => $state_settings,
      '#ajax' => array(
        'callback' => '_crm_core_data_import_references_callback',
        'method' => 'replace',
        'wrapper' => 'references-form-fields-wrapper',
      ),
    );
    $form['references']['fields'] = array(
      '#type' => 'container',
      '#prefix' => '<div id="references-form-fields-wrapper">',
      '#suffix' => '</div>',
      '#states' => $state_settings,
      '#attributes' => array(
        'class' => array('settings-fieldset-wrapper'),
      ),
    );
    $this->formAttachReferences($form, $form_state, $settings);
  }

  /**
   * Validation handler for configForm().
   */
  public function configFormValidate(&$form, &$form_state, $importer) {}

  /**
   * Submission handler for configForm().
   */
  public function configFormSubmit(&$form, &$form_state, $importer) {
    unset($form_state['values']['references']['reference_type']);
    unset($form_state['values']['references']['add_reference']);
    if (!empty($form_state['values']['references']['fields'])) {
      $form_state['values']['references']['fields'] = $this->cleanFormValues($form_state['values']['references']['fields']);
    }
    return array('references' => $form_state['values']['references']);
  }

  /**
   * Attach reference fields to form.
   */
  public function formAttachReferences(&$form, &$form_state, $settings) {
    $triggering_element = !empty($form_state['triggering_element']) ? $form_state['triggering_element'] : FALSE;

    if ($triggering_element['#id'] == 'add-reference-button' && !empty($form_state['values']['references']['reference_type'])) {
      $form_state['values']['references']['fields'][]['reference_type'] = $form_state['values']['references']['reference_type'];
    }

    if (!empty($triggering_element['#parents'][3]) && $triggering_element['#parents'][3] == 'remove_field' && $triggering_element['#parents'][0] == 'references') {
      $field_key = $triggering_element['#parents'][2];
      unset($form_state['values']['references']['fields'][$field_key]);
    }

    if (!empty($form_state['values']['references']['fields'])) {
      foreach ($form_state['values']['references']['fields'] as $delta => $field) {
        list($entity_type, $bundle, $entity_delta, $field_name) = explode(':', $field['reference_type']);
        $this->formAttachFieldReference($form, $form_state, $settings, $entity_type, $bundle, $field_name, $delta, $entity_delta);
      }
    }
  }

  /**
   * Attach one reference field to form.
   */
  public function formAttachFieldReference(&$form, &$form_state, $settings, $entity_type, $bundle, $field_name, $delta, $entity_delta) {
    $destination_values = array();
    $field_info = field_info_field($field_name);
    $field_instance = field_info_instance($entity_type, $field_name, $bundle);

    // If field node author.
    if ($entity_type == 'node' && $field_name == 'uid') {
      $list = $this->getMatchMappingEntity('user', 'user', $settings);
      foreach ($list as $item) {
        $key = $item['entity_type'] . ':user:' . $item['delta'];
        $destination_values[$key] = t('User') . ' ' .  $item['delta'];
      }
      $field_instance['label'] = t('Node author');
      $field_info['field_name'] = 'uid';
    }

    if (!empty($field_info['settings']['handler_settings']['target_bundles'])) {
      $target_entity_type = $field_info['settings']['target_type'];
    }
    elseif (empty($field_info['settings']['handler_settings']['target_bundles']) && !empty($field_info['settings']['target_type']) && $field_info['settings']['target_type'] == 'crm_core_contact') {
      // Use default value for crm_core_contact and individual bundle.
      $target_entity_type = 'crm_core_contact';
    }

    if (!empty($target_entity_type)) {
      $entity_info = entity_get_info($target_entity_type);
      if (!empty($field_info['settings']['handler_settings']['target_bundles'])) {
        foreach ($field_info['settings']['handler_settings']['target_bundles'] as $target_bundle) {
          $list = $this->getMatchMappingEntity($target_entity_type, $target_bundle, $settings);
          foreach ($list as $item) {
            $key = $item['entity_type'] . ':' . $item['entity_bundle'] . ':' . $item['delta'];
            $destination_values[$key] = $entity_info['label'] . ' ' . $target_bundle . ' ' . $item['delta'];
          }
        }
      }
      else {
        // Use default value for crm_core_contact and individual bundle.
        $target_bundle = 'individual';
        $list = $this->getMatchMappingEntity($target_entity_type, $target_bundle, $settings);
        foreach ($list as $item) {
          $key = $item['entity_type'] . ':' . $target_bundle . ':' . $item['delta'];
          $destination_values[$key] = $entity_info['label'] . ' ' . $target_bundle . ' ' . $item['delta'];
        }
      }
    }

    $form['references']['fields'][$delta] = array(
      '#type' => 'fieldset',
      '#attributes' => array(
        'class' => array('settings-fieldset'),
      ),
    );
    $form['references']['fields'][$delta]['label'] = array(
      '#type' => 'item',
      '#markup' => t('@field_name (@bundle) should be reference', array('@field_name' => $field_instance['label'], '@bundle' => $bundle)),
    );
    $form['references']['fields'][$delta]['source'] = array(
      '#type' => 'value',
      '#value' => $entity_type . ':' . $bundle . ':' . $entity_delta,
    );
    $form['references']['fields'][$delta]['destination'] = array(
      '#type' => 'select',
      '#options' => $destination_values,
      '#default_value' => !empty($form_state['values']['references']['fields'][$delta]['destination']) ? $form_state['values']['references']['fields'][$delta]['destination'] : FALSE,
    );
    $form['references']['fields'][$delta]['reference_type'] = array(
      '#type' => 'value',
      '#value' => $entity_type . ':' . $bundle . ':' . $entity_delta . ':' . $field_info['field_name'],
    );
    $form['references']['fields'][$delta]['remove_field'] = array(
      '#type' => 'button',
      '#value' => t('Remove'),
      '#name' => drupal_html_id('references-settings-remove-button-' . $entity_delta),
      '#ajax' => array(
        'callback' => '_crm_core_data_import_references_callback',
        'method' => 'replace',
        'wrapper' => 'references-form-fields-wrapper',
      ),
    );
  }

  /**
   * Build a list of available references relevant to current settings.
   */
  public function getAvailableReferences($settings) {
    $references = array();

    $entity_types = $this->entityList($settings);
    foreach ($entity_types as $key => $entity_type) {
      list($current_entity_type, $current_entity_bundle, $current_entity_delta) = explode(':', $key);
      $fields = field_info_instances($entity_type['entity_type'], $entity_type['bundle']);
      foreach ($fields as $field_name => $field) {
        $field_info = field_info_field($field_name);
        if ($field_info['type'] == 'entityreference' && !empty($field_info['settings']['handler_settings']['target_bundles'])) {
          $field_entitiy_type = $field_info['settings']['target_type'];
          foreach ($field_info['settings']['handler_settings']['target_bundles'] as $field_bundle) {
            if ($this->entityBundleIsAvailable($settings, $field_entitiy_type, $field_bundle)) {
              $key_reference = $field['entity_type'] . ':' . $field['bundle'] . ':' . $current_entity_delta . ':' . $field_name;
              $references[$key_reference] = $field['label'];
            }
          }
        }
        elseif ($field_info['type'] == 'entityreference' && empty($field_info['settings']['handler_settings']['target_bundles']) && $field_info['settings']['target_type'] == 'crm_core_contact') {
          if ($this->entityBundleIsAvailable($settings, 'crm_core_contact', 'individual')) {
            $key_reference = $field['entity_type'] . ':' . $field['bundle'] . ':' . $current_entity_delta . ':' . $field_name;
            $references[$key_reference] = $field['label'] . ' (' . $field['bundle'] . ')';
          }
        }
      }

      // Node author.
      if ($current_entity_type == 'NodeDestinationHandler' && $this->entityTypeIsAvailable($settings, 'user')) {
        $key_reference = $field['entity_type'] . ':' . $field['bundle'] . ':' . $current_entity_delta . ':uid';
        $references[$key_reference] = 'Node author';
      }
    }

    if (empty($references)) {
      $references[] = t('No available references');
    }

    return $references;
  }

  /**
   * Returns TRUE if entity bundle available in current settings.
   */
  public function entityBundleIsAvailable($settings, $field_entitiy_type, $bundle) {
    $available = FALSE;
    $entity_types = $this->entityList($settings);
    foreach ($entity_types as $entity_type) {
      if ($entity_type['entity_type'] == $field_entitiy_type && $entity_type['bundle'] == $bundle) {
        $available = TRUE;
      }
    }
    return $available;
  }

  /**
   * Returns TRUE if entity type available in current settings.
   */
  public function entityTypeIsAvailable($settings, $field_entitiy_type) {
    $available = FALSE;
    $entity_types = $this->entityList($settings);
    foreach ($entity_types as $entity_type) {
      if ($entity_type['entity_type'] == $field_entitiy_type) {
        $available = TRUE;
      }
    }
    return $available;
  }

  /**
   * Create reference between entities.
   */
  public function createReference($source_entity, $destination_entity, $reference_settings) {
    try {
      // @TODO might be improved.
      list($target_entity_type, $target_bundle, ,$target_field) = explode(':', $reference_settings['reference_type']);

      $source_type = $source_entity->entity_type;
      $destination_type = $destination_entity->entity_type;

      $source_entity = entity_load_single($source_type, $source_entity->id);
      $destination_entity = entity_load_single($destination_type, $destination_entity->id);

      if ($source_entity && $destination_entity) {
        $source_entity->default_revision = FALSE;
        $destination_entity->default_revision = FALSE;
        $source_wrapper = entity_metadata_wrapper($source_type, $source_entity);
        $destination_wrapper = entity_metadata_wrapper($destination_type, $destination_entity);

        $multivalue = FALSE;
        $field = field_info_field($target_field);

        if (!empty($field['cardinality']) && $field['cardinality'] != 1) {
          $multivalue = TRUE;
        }

        // At this point we don't know which entity is target and which is source.
        if ($destination_type == $target_entity_type && $destination_entity->type == $target_bundle) {
          $origin = $destination_wrapper;
          $value = $source_wrapper;
        }
        elseif ($source_type == $target_entity_type && $source_entity->type == $target_bundle) {
          $origin = $source_wrapper;
          $value = $destination_wrapper;
        }

        // If field node author.
        if ($origin && $value && $target_entity_type == 'node' && $target_field == 'uid') {
          $origin->author->set($value);
          $origin->save();
          return;
        }

        // Add value to entity.
        if ($origin && $value) {
          if ($multivalue) {
            // Set value only if it not exist.
            $exist = FALSE;
            foreach ($origin->{$target_field}->getIterator() as $target_field_wrapper) {
              if ($target_field_wrapper->getIdentifier() == $value->getIdentifier() && $target_field_wrapper->getBundle() == $value->getBundle()) {
                $exist = TRUE;
              }
            }
            if (!$exist) {
              $origin->{$target_field}[] = $value;
              $origin->save();
            }
          }
          else {
            $origin->{$target_field}->set($value);
            $origin->save();
          }
        }
      }
    }
    catch (Exception $e) {
      watchdog('crm_core_data_import', $e->getMessage(), array(), WATCHDOG_ERROR);
    }
  }
}

/**
 * Callback for references add button.
 */
function _crm_core_data_import_references_callback(&$form, &$form_state) {
  return $form['references']['fields'];
}
