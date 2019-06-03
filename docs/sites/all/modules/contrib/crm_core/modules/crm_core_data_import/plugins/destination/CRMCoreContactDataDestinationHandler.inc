<?php
/**
 * @file
 * Destination handler for CRM Core Contact.
 */

$plugin = array(
  'label' => t('Contact'),
  'handler' => array(
    'class' => 'CRMCoreContactDataDestinationHandler',
  ),
);

class CRMCoreContactDataDestinationHandler extends CRMCoreDataDestinationHandler {
  /**
   * Default construct.
   */
  public function __construct() {
    $this->entityType = 'crm_core_contact';
  }

  /**
   * Returns MigrationDestination instance.
   */
  public function migrateDestination($entity_bundle) {
    return new MigrateDestinationCRMCoreContact($entity_bundle);
  }

  /**
   * Returns key schema for migration.
   */
  public function migrateDestinationMap() {
    return MigrateDestinationCRMCoreContact::getKeySchema();
  }

  /**
   * Build array of available fields.
   */
  public function getFields($bundle) {
    $fields = parent::getFields($bundle);
    return $fields;
  }
}
