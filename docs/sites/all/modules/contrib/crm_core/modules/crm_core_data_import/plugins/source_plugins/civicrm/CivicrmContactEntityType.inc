<?php
/**
 * @file
 * Plugin for contact entity type CiviCRM.
 */

$plugin = array(
  'label' => t('Contact types'),
  'handler' => array(
    'class' => 'CivicrmContactEntityType',
  ),
  'entity_name' => 'Contact',
  'source_mapping' => TRUE,
);

class CivicrmContactEntityType extends CivicrmEntityType {
  /**
   * Returns available CiviCRM entity types.
   */
  public function getCivicrmTypes() {
    $types = array();
    $available_types = crm_core_data_import_civicrm_api('contact_type', 'get');
    foreach ($available_types as $value) {
      $types[$value['name']] = $value['name'];
    }
    return $types;
  }

  /**
   * Returns available Drupal entity types.
   */
  public function getDrupalTypes() {
    return $this->getBundles('crm_core_contact');
  }

  /**
   * Returns available fields.
   */
  public function getEntityFields($entity_type) {
    $fields = $this->fetchFields($entity_type, $entity_type, 'contact_type');

    $entity_name = $this->getEntityName();
    $entity_label = $entity_name . ':' . $entity_type;

    $field_collection_items = _crm_core_data_import_contact_field_collection_fields();
    foreach ($field_collection_items as $field_collection_item) {
      $sub_fields = crm_core_data_import_civicrm_api($field_collection_item, 'getfields');
      foreach ($sub_fields as $sub_field) {
        if (empty($sub_field['custom_group_id'])) {
          $key = $field_collection_item . ':' . $sub_field['name'];
          $fields[$entity_label][$entity_label . ':' . $key] = $key;
        }
      }
    }

    return $fields;
  }
}
