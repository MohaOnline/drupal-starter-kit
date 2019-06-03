<?php
/**
 * @file
 * Plugin for contact entity type CiviCRM.
 */

$plugin = array(
  'label' => t('Relationship types'),
  'handler' => array(
    'class' => 'CivicrmRelationshipEntityType',
  ),
  'entity_name' => 'Relationship',
  'source_mapping' => FALSE,
);

class CivicrmRelationshipEntityType extends CivicrmEntityType {
  /**
   * Returns available entity types.
   */
  public function getCivicrmTypes() {
    $types = array();
    $available_types = crm_core_data_import_civicrm_api('relationship_type', 'get');
    foreach ($available_types as $value) {
      if (!empty($value['description'])) {
        $types[$value['id']] = $value['description'];
      }
      else {
        $types[$value['id']] = $value['name_a_b'];
      }
    }
    return $types;
  }

  /**
   * Returns available Drupal entity types.
   */
  public function getDrupalTypes() {
    return array();
  }

  /**
   * Returns available fields.
   */
  public function getEntityFields($type = FALSE) {
    return array();
  }
}
