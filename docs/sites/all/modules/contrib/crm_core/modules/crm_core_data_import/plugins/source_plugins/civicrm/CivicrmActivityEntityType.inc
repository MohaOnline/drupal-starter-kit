<?php
/**
 * @file
 * Plugin for contact entity type CiviCRM.
 */

$plugin = array(
  'label' => t('Activity types'),
  'handler' => array(
    'class' => 'CivicrmActivityEntityType',
  ),
  'entity_name' => 'Activity',
  'source_mapping' => TRUE,
);

class CivicrmActivityEntityType extends CivicrmEntityType {
  /**
   * Returns available CiviCRM entity types.
   */
  public function getCivicrmTypes() {
    $types = array();
    $available_types = crm_core_data_import_civicrm_api('activity_type', 'get');
    foreach ($available_types as $value) {
      $types[$value] = $value;
    }
    return $types;
  }

  /**
   * Returns available Drupal entity types.
   */
  public function getDrupalTypes() {
    return $this->getBundles('crm_core_activity');
  }

  /**
   * Returns available fields.
   */
  public function getEntityFields($entity_type) {
    $activity_type = crm_core_data_get_civicrm_activity_type($entity_type);
    $fields = $this->fetchFields($entity_type, $activity_type['value'], 'activity_type_id');
    $this->attachExtraFields($fields, $this->getEntityName(), $entity_type, $this->getExtraFields());
    return $fields;
  }

  /**
   * Returns extra fields which not listed by 'getfields' CiviCRM API method.
   */
  public function getExtraFields() {
    return array(
      'assignee_contact_id',
      'target_contact_id',
      'status',
      'priority',
    );
  }
}
