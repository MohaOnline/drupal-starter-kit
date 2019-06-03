<?php
/**
 * @file
 * Default destination handler for CRM Core Data Import
 */

abstract class CRMCoreDataDestinationHandler {
  /**
   * Type of entity.
   * @var string
   */
  public $entityType;
  public $bundle;

  /**
   * Returns MigrationDestination instance.
   *
   * @param setings $entity_bundle
   *   Entity bundle.
   *
   * @return MigrationDestination
   *   MigrationDestination for migration.
   */
  public abstract function migrateDestination($entity_bundle);

  /**
   * Returns key schema for migration.
   *
   * @return array
   *   Key schema for migration.
   */
  public abstract function migrateDestinationMap();

  /**
   * Returns available bundles and their labels.
   */
  public function getBundles() {
    return $this->getEntityBundles();
  }

  /**
   * Build array of available bundles.
   *
   * @return array
   *   Available bundles.
   */
  public function getEntityBundles() {
    $bundles_result = array();
    $bundles = field_info_bundles($this->entityType);
    foreach ($bundles as $key => $bundle) {
      $bundles_result[$key] = $bundle['label'];
    }

    return $bundles_result;
  }

  /**
   * Build array of available fields.
   *
   * There you can define additional fields.
   */
  public function getFields($bundle) {
    $fields_result = array();
    $fields = migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $bundle);
    foreach ($fields as $field_key => $field) {
      $fields_result[$field_key] = $field_key;
    }

    return $fields_result + $this->attachFieldCollectionFields($bundle);
  }

  /**
   * Attach field collection fields to destination fields.
   */
  public function attachFieldCollectionFields($bundle) {
    $fields = array();
    $fields_info = field_info_instances($this->entityType, $bundle);
    foreach ($fields_info as $field_info) {
      if (!empty($field_info['widget']['module']) && $field_info['widget']['module'] == 'field_collection') {
        $field_collection_fields = migrate_handler_invoke_all('Entity', 'fields', 'field_collection_item', $field_info['field_name']);
        foreach ($field_collection_fields as $field_collection_field_key => $field_collection_field) {
          $label = $field_info['field_name'] . ':' . $field_collection_field_key;
          $fields[$label] = $label;
        }
      }
    }

    return $fields;
  }
}
