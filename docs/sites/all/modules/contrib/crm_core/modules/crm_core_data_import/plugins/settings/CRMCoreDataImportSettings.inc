<?php
/**
 * @file
 * Default settings handler for CRM Core Data Import.
 */

abstract class CRMCoreDataImportSettings {
  /**
   * Returns TRUE if conditions match for settings.
   *
   * @param object $importer
   *   Importer.
   *
   * @return bool
   *   Display flag.
   */
  public abstract function displayConditions($importer);

  /**
   * Configuration form for settings plugin.
   *
   * @param array $form
   *   Form elements.
   *
   * @param array $form_state
   *   Form state.
   *
   * @param object $importer
   *   Importer instance.
   *
   * @param string $label
   *   Label of plugin.
   */
  public abstract function configForm(&$form, &$form_state, $importer, $label);

  /**
   * Validation handler for configForm().
   */
  public abstract function configFormValidate(&$form, &$form_state, $settings);

  /**
   * Submission handler for configForm().
   */
  public abstract function configFormSubmit(&$form, &$form_state, $settings);

  /**
   * Post import handler.
   */
  public function postImport($importer, $item) {}

  /**
   * Pre import handler.
   */
  public function preImport($importer, &$migration) {}

  /**
   * List of entities.
   */
  public function entityList($settings) {
    $entities = array();
    $mapping = $settings['mapping'];
    foreach ($mapping as $key => $value) {
      list($entity_type, $entity_bundle, $entity_delta) = explode(':', $key);
      $entities[$key] = array(
        'entity_type' => crm_core_data_import_get_destination_entity_type($entity_type),
        'bundle' => $entity_bundle,
        'delta' => $entity_delta,
      );
    }
    return $entities;
  }

  /**
   * Returns entity from mapping which match to entity type and bundle.
   */
  public function getMatchMappingEntity($entity_type, $entity_bundle, $settings) {
    $match = array();
    if (!empty($settings['mapping'])) {
      foreach ($settings['mapping'] as $key => $mapping) {
        if (is_array($mapping)) {
          list($entity_controlle_type, $current_entity_bundle, $bundle_delta) = explode(':', $key);
          $current_entity_type = crm_core_data_import_get_destination_entity_type($entity_controlle_type);
          if ($entity_type == $current_entity_type && $entity_bundle == $current_entity_bundle) {
            $match[$current_entity_type . ':' . $current_entity_bundle . ':' . $bundle_delta] = array(
              'entity_type' => $current_entity_type,
              'entity_controller_type' => $entity_controlle_type,
              'entity_bundle' => $current_entity_bundle,
              'delta' => $bundle_delta,
            );
          }
        }
      }
    }
    return $match;
  }

  /**
   * Clear form values.
   */
  public function cleanFormValues($values) {
    foreach ($values as $field_key => $field_value) {
      if (!empty($values[$field_key]['remove_field'])) {
        unset($values[$field_key]['remove_field']);
      }
    }
    return $values;
  }
}
