<?php
/**
 * @file
 * Destination handler for CRM Core Activity.
 */

$plugin = array(
  'label' => t('Activity'),
  'handler' => array(
    'class' => 'CRMCoreActivityDataDestinationHandler',
  ),
);

class CRMCoreActivityDataDestinationHandler extends CRMCoreDataDestinationHandler {
  /**
   * Default construct.
   */
  public function __construct() {
    $this->entityType = 'crm_core_activity';
  }

  /**
   * Returns MigrationDestination instance.
   */
  public function migrateDestination($entity_bundle) {
    return new MigrateDestinationCRMCoreActivity($entity_bundle);
  }

  /**
   * Returns key schema for migration.
   */
  public function migrateDestinationMap() {
    return MigrateDestinationCRMCoreActivity::getKeySchema();
  }

  /**
   * Build array of available fields.
   */
  public function getFields($bundle) {
    $fields = parent::getFields($bundle);
    $fields['title'] = t('title');
    return $fields;
  }
}
