<?php
/**
 * @file
 * Default CiviCRM source plugin.
 */

abstract class CivicrmEntityType {
  /**
   * Returns available CiviCRM entity types.
   */
  public abstract function getCivicrmTypes();

  /**
   * Returns available Drupal entity types.
   */
  abstract public function getDrupalTypes();

  /**
   * Returns available fields.
   */
  abstract public function getEntityFields($entity_name);

  /**
   * Returns entity name of current plugin.
   */
  public function getEntityName() {
    ctools_include('plugins');
    $plugin = ctools_get_plugins('crm_core_data_import', 'crm_core_data_import_source_civicrm_entity_type');
    return !empty($plugin[get_class($this)]['entity_name']) ? $plugin[get_class($this)]['entity_name'] : FALSE;
  }

  /**
   * Fetch field from CiviCRM.
   */
  public function fetchFields($entity_type_name, $entity_type_value, $entity_type_field = FALSE) {
    $fields = array();
    $options = array();
    if (!empty($entity_type_field)) {
      $options[$entity_type_field] = $entity_type_value;
    }
    $entity_name = $this->getEntityName();
    $entity_label = $entity_name . ':' . $entity_type_name;

    // Both this activity types returns not full field list if requested by
    // 'Activity' entity name.
    // @todo fix for the additional fields in contributions and participants.
    if ($entity_type_name == 'Event Registration') {
      $entity_name = 'participant';
    }

    if ($entity_type_name == 'Contribution') {
      $entity_name = 'contribution';
    }

    $entity = crm_core_data_import_civicrm_api($entity_name, 'getfields', $options);
    if (!empty($entity)) {
      foreach ($entity as $name => $value) {
        if (!empty($value['name'])) {
          $key = $value['name'];
        }
        else {
          $key = $name;
        }

        $field_label = $key;
        if (!empty($value['label'])) {
          $field_label .= ' - ' . $value['label'];
        }
        $fields[$entity_label][$entity_label . ':' . $key] = $field_label;
      }
    }
    return $fields;
  }

  /**
   * Attach extra fields.
   */
  public function attachExtraFields(&$fields, $entity_name, $entity_type, $extra_fields) {
    $entity_label = $entity_name . ':' . $entity_type;
    foreach ($extra_fields as $field) {
      if (!isset($fields[$entity_label][$entity_label . ':' . $field])) {
        $fields[$entity_label][$entity_label . ':' . $field] = $field;
      }
    }
  }

  /**
   * Returns entity bundles.
   */
  public function getBundles($entity_type) {
    $types = array();
    $entity_info = entity_get_info($entity_type);
    if (!empty($entity_info['bundles'])) {
      foreach ($entity_info['bundles'] as $bundle_name => $bundle) {
        $types[$bundle_name] = $bundle['label'];
      }
    }
    return $types;
  }
}
