<?php
/**
 * @file
 * User contacts handler for CRM Core Data Import.
 */

$plugin = array(
  'label' => t('Contact for user'),
  'handler' => array(
    'class' => 'UserContactImportSettings',
  ),
);

class UserContactImportSettings extends CRMCoreDataImportSettings {
  /**
   * Returns TRUE if conditions match for settings.
   */
  public function displayConditions($importer) {
    $active = FALSE;
    $settings = $importer->getSettings();
    // Activate this plugin only if user entity in import.
    $entities = $this->entityList($settings);
    foreach ($entities as $entity_info) {
      if ($entity_info['entity_type'] == 'user') {
        $active = TRUE;
      }
    }
    return $active;
  }

  /**
   * Configuration form for settings plugin.
   */
  public function configForm(&$form, &$form_state, $importer, $label) {
    $settings = $importer->getSettings();
    if (!empty($settings['user_contact']) && empty($form_state['values']['user_contact'])) {
      $form_state['values']['user_contact'] = $settings['user_contact'];
    }

    $state_settings = array(
      'visible' => array(
        ':input[name="user_contact[create_automatically]"]' => array('checked' => FALSE),
      ),
    );

    $form['user_contact'] = array(
      '#type' => 'container',
      '#weight' => -11,
    );
    $form['user_contact']['title'] = array(
      '#type' => 'item',
      '#title' => $label,
    );

    // Ability to disable automatic creation only if crm_core_user_sync enabled.
    if (module_exists('crm_core_user_sync')) {
      $form['user_contact']['create_automatically'] = array(
        '#type' => 'checkbox',
        '#title' => t('Create а contact for imported users?'),
        '#default_value' => !empty($form_state['values']['user_contact']['create_automatically']) ? $form_state['values']['user_contact']['create_automatically'] : FALSE,
      );
    }

    // If contact entity in import.
    $contact = FALSE;
    $entities = $this->entityList($settings);
    foreach ($entities as $entity_info) {
      if ($entity_info['entity_type'] == 'crm_core_contact' && $entity_info['bundle'] == 'individual') {
        $contact = TRUE;
      }
    }

    if ($contact) {
      $form['user_contact']['user_type'] = array(
        '#type' => 'select',
        '#title' => t('Add user'),
        '#options' => $this->getAvailableUsers($settings),
        '#states' => $state_settings,
      );
      $form['user_contact']['add_user_contact'] = array(
        '#type' => 'button',
        '#id' => 'add-user-contact',
        '#value' => t('Add'),
        '#name' => 'add-user-contact',
        '#states' => $state_settings,
        '#ajax' => array(
          'callback' => '_crm_core_data_import_user_contact_callback',
          'method' => 'replace',
          'wrapper' => 'user-contact-form-fields-wrapper',
        ),
      );
      $form['user_contact']['fields'] = array(
        '#type' => 'container',
        '#prefix' => '<div id="user-contact-form-fields-wrapper">',
        '#suffix' => '</div>',
        '#states' => $state_settings,
        '#attributes' => array(
          'class' => array('settings-fieldset-wrapper'),
        ),
      );
      $this->formAttachUserContact($form, $form_state, $settings);
    }
  }

  /**
   * Validation handler for configForm().
   */
  public function configFormValidate(&$form, &$form_state, $importer) {}

  /**
   * Submission handler for configForm().
   */
  public function configFormSubmit(&$form, &$form_state, $importer) {
    unset($form_state['values']['user_contact']['user_type']);
    unset($form_state['values']['user_contact']['add_user_contact']);
    if (!empty($form_state['values']['references']['fields'])) {
      $form_state['values']['user_contact']['fields'] = $this->cleanFormValues($form_state['values']['user_contact']['fields']);
    }
    return array('user_contact' => $form_state['values']['user_contact']);
  }

  /**
   * Attach reference fields to form.
   */
  public function formAttachUserContact(&$form, &$form_state, $settings) {
    $triggering_element = !empty($form_state['triggering_element']) ? $form_state['triggering_element'] : FALSE;

    if ($triggering_element['#id'] == 'add-user-contact' && !empty($form_state['values']['user_contact']['user_type'])) {
      $form_state['values']['user_contact']['fields'][]['user_type'] = $form_state['values']['user_contact']['user_type'];
    }

    if (!empty($triggering_element['#parents'][3]) && $triggering_element['#parents'][3] == 'remove_field' && $triggering_element['#parents'][0] == 'user_contact') {
      $field_key = $triggering_element['#parents'][2];
      unset($form_state['values']['user_contact']['fields'][$field_key]);
    }

    if (!empty($form_state['values']['user_contact']['fields'])) {
      foreach ($form_state['values']['user_contact']['fields'] as $delta => $field) {
        list($entity_type, $bundle, $entity_delta) = explode(':', $field['user_type']);
        $this->formAttachFieldUserContact($form, $form_state, $settings, $entity_type, $bundle, $delta, $entity_delta);
      }
    }
  }

  /**
   * Attach one reference field to form.
   */
  public function formAttachFieldUserContact(&$form, &$form_state, $settings, $entity_type, $bundle, $delta, $entity_delta) {
    $destination_values = array();
    $list = $this->getMatchMappingEntity('crm_core_contact', 'individual', $settings);
    foreach ($list as $key => $item) {
      $destination_values[$key] = crm_core_data_import_get_destination_label($item['entity_controller_type'], $item['entity_bundle'], $item['delta']);
    }

    $form['user_contact']['fields'][$delta] = array(
      '#type' => 'fieldset',
      '#attributes' => array(
        'class' => array('settings-fieldset'),
      ),
    );
    $form['user_contact']['fields'][$delta]['label'] = array(
      '#type' => 'item',
      '#markup' => t('User @delta should be related to contact', array('@delta' => $entity_delta)),
    );
    $form['user_contact']['fields'][$delta]['source'] = array(
      '#type' => 'value',
      '#value' => $entity_type . ':' . $bundle . ':' . $entity_delta,
    );
    $form['user_contact']['fields'][$delta]['destination'] = array(
      '#type' => 'select',
      '#options' => $destination_values,
      '#default_value' => !empty($form_state['values']['user_contact']['fields'][$delta]['destination']) ? $form_state['values']['user_contact']['fields'][$delta]['destination'] : FALSE,
    );
    $form['user_contact']['fields'][$delta]['user_type'] = array(
      '#type' => 'value',
      '#value' => $entity_type . ':' . $bundle . ':' . $entity_delta,
    );
    $form['user_contact']['fields'][$delta]['remove_field'] = array(
      '#type' => 'button',
      '#value' => t('Remove'),
      '#name' => drupal_html_id('user-contact-settings-remove-button-' . $entity_delta),
      '#ajax' => array(
        'callback' => '_crm_core_data_import_user_contact_callback',
        'method' => 'replace',
        'wrapper' => 'user-contact-form-fields-wrapper',
      ),
    );
  }

  /**
   * Performs when import was successful.
   */
  public function postImport($importer, $item) {
    $settings = $importer->getSettings();
    if ((empty($settings['user_contact']['create_automatically']) || $settings['user_contact']['create_automatically'] == 0) && !empty($settings['user_contact']['fields'])) {
      $list = $importer->source_plugin->getEntitiesForUsers($settings['user_contact'], $item, $importer);
      // Process list of the related entities.
      foreach ($list as $item) {
        $this->createContactRelation($item['source_entity'], $item['destination_entity']);
      }
    }
  }

  /**
   * Preprocess import.
   */
  public function preImport($importer, &$migration) {
    if ($migration->getEntityType() == 'user') {
      $importer_settings = $importer->getSettings();

      // Prevent creating contacts automatically.
      if (empty($importer_settings['user_contact']['create_automatically']) || $importer_settings['user_contact']['create_automatically'] == 0) {
        $migration->addFieldMapping('crm_core_no_auto_sync')->defaultValue(TRUE);
      }
    }
  }

  /**
   * Returns available users.
   */
  public function getAvailableUsers($settings) {
    $users = array();
    $entities = $this->entityList($settings);
    foreach ($entities as $entity_info) {
      if ($entity_info['entity_type'] == 'user') {
        $users[$entity_info['entity_type'] . ':' . $entity_info['bundle'] . ':' . $entity_info['delta']] = ucfirst($entity_info['entity_type']) . ' ' . $entity_info['delta'];
      }
    }
    return $users;
  }

  /**
   * Create relation between user and crm core contact.
   *
   * @param user $source_entity
   *   Source entity.
   * @param crm_core_contact $destination_entity
   *   Target entity.
   */
  public function createContactRelation($source_entity, $destination_entity) {
    if ($source_entity && $destination_entity) {
      $endpoints = array(
        array(
          'entity_type' => 'user',
          'entity_bundle' => 'user',
          'entity_id' => $source_entity->uid,
        ),
        array(
          'entity_type' => 'crm_core_contact',
          'entity_bundle' => $destination_entity->type,
          'entity_id' => $destination_entity->contact_id,
        ),
      );
      $relation = relation_create('crm_core_user_sync', $endpoints);
      relation_save($relation);
    }
  }
}

/**
 * Callback for user add button.
 */
function _crm_core_data_import_user_contact_callback(&$form, &$form_state) {
  return $form['user_contact']['fields'];
}
