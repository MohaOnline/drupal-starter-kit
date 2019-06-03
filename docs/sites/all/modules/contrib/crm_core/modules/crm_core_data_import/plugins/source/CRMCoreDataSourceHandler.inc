<?php
/**
 * @file
 * Default source handler for CRM Core Data Import
 */

abstract class CRMCoreDataSourceHandler {
  public $sourceMapping = FALSE;
  public $settings;

  /**
   * Configuration form for source plugin.
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
  public abstract function configForm(&$form, &$form_state, $settings);

  /**
   * Validation handler for configForm().
   */
  public abstract function configFormValidate(&$form, &$form_state, $settings);

  /**
   * Submission handler for configForm().
   */
  public abstract function configFormSubmit(&$form, &$form_state, $settings);

  /**
   * Configuration form for source mapping.
   */
  public function sourceMappingForm(&$form, &$form_state, $settings) {}

  /**
   * Validation handler for sourceMappingForm().
   */
  public function sourceMappingFormValidate(&$form, &$form_state, $settings) {}

  /**
   * Submission handler for sourceMappingForm().
   */
  public function sourceMappingFormSubmit(&$form, &$form_state, $settings) {}

  /**
   * Returns available fields for mapping.
   */
  public abstract function getFields($settings);

  /**
   * Returns MigrationSource instance.
   */
  public abstract function migrateSource($settings, $entity_type, $entity_bundle, $mapping);

  /**
   * Returns related list entities from migration results for relationships.
   */
  public abstract function getEntitiesForRelationships($settings, $item, $importer);

  /**
   * Returns related list entities from migration results for references.
   */
  public abstract function getEntitiesForReferences($settings, $item, $importer);

  /**
   * Returns related list entities from migration results for users.
   */
  public abstract function getEntitiesForUsers($settings, $item, $importer);

  /**
   * Alter for field mapping form.
   */
  public function fieldMappingFormAlter(&$form, &$form_state) {}

  /**
   * Alter for source fields on the mapping form.
   */
  public function mappingSourceFieldsAlter(&$fields, $entity_type, $entity_bundle, $importer) {}
}
