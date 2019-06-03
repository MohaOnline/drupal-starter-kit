<?php
/**
 * @file
 * Relations settings handler for CRM Core Data Import.
 */

$plugin = array(
  'label' => t('Linked Imports'),
  'handler' => array(
    'class' => 'LinkedImportSettings',
  ),
);

class LinkedImportSettings extends CRMCoreDataImportSettings {
  /**
   * Returns TRUE if conditions match for settings.
   */
  public function displayConditions($importer) {
    // Display only if there is more than one importer.
    $importer_ids = CRMCoreDataImport::getIds();
    if (!is_array($importer_ids) || count($importer_ids) < 2) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Configuration form for settings plugin.
   */
  public function configForm(&$form, &$form_state, $importer, $label) {
    $settings = $importer->getSettings();
    if (!empty($settings['linked_imports']) && empty($form_state['values']['linked_imports'])) {
      $form_state['values']['linked_imports'] = $settings['linked_imports'];
    }

    $state_settings = array(
      'visible' => array(
        ':input[name="linked_imports[enable]"]' => array('checked' => TRUE),
      ),
    );

    $form['linked_imports'] = array(
      '#type' => 'container',
      '#weight' => -30,
    );
    $form['linked_imports']['title'] = array(
      '#type' => 'item',
      '#title' => $label,
    );
    $form['linked_imports']['enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use already imported data?'),
      '#default_value' => !empty($form_state['values']['linked_imports']['enable']) ? $form_state['values']['linked_imports']['enable'] : FALSE,
    );
    $form['linked_imports']['import'] = array(
      '#type' => 'select',
      '#title' => t('Use data from'),
      '#options' => $this->getAvailableImports($importer),
      '#states' => $state_settings,
    );
    $form['linked_imports']['fields'] = array(
      '#type' => 'container',
      '#states' => $state_settings,
      '#prefix' => '<div id="linked-imports-form-fields-wrapper">',
      '#suffix' => '</div>',
      '#attributes' => array(
        'class' => array('settings-fieldset-wrapper'),
      ),
    );
    $form['linked_imports']['add_import'] = array(
      '#type' => 'button',
      '#value' => t('Add'),
      '#states' => $state_settings,
      '#name' => 'add-import-button',
      '#ajax' => array(
        'callback' => '_crm_core_data_import_linked_import_callback',
        'method' => 'replace',
        'wrapper' => 'linked-imports-form-fields-wrapper',
      ),
    );
    $this->formAttachLinkedImports($form, $form_state, $settings, $importer);
  }

  /**
   * @inheritdoc
   */
  public function configFormValidate(&$form, &$form_state, $settings) {

  }

  /**
   * @inheritdoc
   */
  public function configFormSubmit(&$form, &$form_state, $importer) {
    unset($form_state['values']['linked_imports']['import']);
    unset($form_state['values']['linked_imports']['add_import']);
    if (!empty($form_state['values']['linked_imports']['fields'])) {
      $form_state['values']['linked_imports']['fields'] = $this->cleanFormValues($form_state['values']['linked_imports']['fields']);
    }
    return array('linked_imports' => $form_state['values']['linked_imports']);
  }

  /**
   * Attach relation fields to form.
   */
  public function formAttachLinkedImports(&$form, &$form_state, $settings, CRMCoreDataImport $importer) {
    $triggering_element = !empty($form_state['triggering_element']) ? $form_state['triggering_element'] : FALSE;

    if ($triggering_element['#name'] == 'add-import-button' && !empty($form_state['values']['linked_imports']['import'])) {
      $import_id = $form_state['values']['linked_imports']['import'];
      $form_state['values']['linked_imports']['fields'][$import_id] = array('import_id' => $import_id);
    }

    if (!empty($triggering_element['#parents'][3]) && $triggering_element['#parents'][3] == 'remove_field' && $triggering_element['#parents'][0] == 'linked_imports') {
      $field_key = $triggering_element['#parents'][2];
      unset($form_state['values']['linked_imports']['fields'][$field_key]);
    }

    if (!empty($form_state['values']['linked_imports']['fields'])) {
      foreach ($form_state['values']['linked_imports']['fields'] as $delta => $field) {
        $this->formAttachField($form, $form_state, $importer, $settings, $field);
      }
    }
  }

  /**
   * Attach single field.
   */
  public function formAttachField(&$form, &$form_state, CRMCoreDataImport $importer, $settings, $field) {
    $delta = $field['import_id'];
    $form['linked_imports']['fields'][$delta] = array(
      '#type' => 'fieldset',
      '#attributes' => array(
        'class' => array('settings-fieldset'),
      ),
    );
    $linked_importer = crm_core_data_import_load_importer($field['import_id']);
    $mapping_settings = $linked_importer->getMappingSettings();
    $options = array();
    foreach ($mapping_settings as $key => $mapping_instance) {
      list($entity_type, $entity_bundle,) = explode(':', $key);
      $options[$key] = $entity_bundle;
    }

    $form['linked_imports']['fields'][$delta]['import_id'] = array(
      '#type' => 'value',
      '#value' => $field['import_id'],
    );

    $form['linked_imports']['fields'][$delta]['source'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => !empty($form_state['values']['linked_imports']['fields'][$delta]['source']) ? $form_state['values']['linked_imports']['fields'][$delta]['source'] : FALSE,
    );
    $form['linked_imports']['fields'][$delta]['label'] = array(
      '#type' => 'item',
      '#markup' => 'with key',
    );
    $form['linked_imports']['fields'][$delta]['destination'] = array(
      '#type' => 'select',
      '#options' => $importer->getSourceFields(),
      '#default_value' => !empty($form_state['values']['linked_imports']['fields'][$delta]['destination']) ? $form_state['values']['linked_imports']['fields'][$delta]['destination'] : FALSE,
    );
    $form['linked_imports']['fields'][$delta]['remove_field'] = array(
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
   * Performs when import was successful.
   */
  public function postImport($importer, $item) {
  }

  /**
   * @param CRMCoreDataImport $importer
   */
  public function getAvailableImports(CRMCoreDataImport $importer) {
    $extracted_condition = db_and()->condition('id', $importer->id, '<>');
    $ids = CRMCoreDataImport::getIds(array($extracted_condition));
    $options = array();
    foreach ($ids as $id) {
      $object = crm_core_data_import_load_importer($id);
      $options[$id] = $object->title;
    }
    return $options;
  }
}

/**
 * Callback for relations add button.
 */
function _crm_core_data_import_linked_import_callback(&$form, &$form_state) {
  return $form['linked_imports']['fields'];
}
