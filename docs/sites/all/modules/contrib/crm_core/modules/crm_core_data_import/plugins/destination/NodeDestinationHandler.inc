<?php
/**
 * @file
 * Destination handler for Node.
 */

$plugin = array(
  'label' => t('Node'),
  'handler' => array(
    'class' => 'NodeContactDataDestinationHandler',
  ),
);

class NodeContactDataDestinationHandler extends CRMCoreDataDestinationHandler {
  /**
   * Default construct.
   */
  public function __construct() {
    $this->entityType = 'node';
  }

  /**
   * Build array of available fields.
   */
  public function getFields($bundle) {
    $fields = parent::getFields($bundle);
    $fields['title'] = t('Title');
    return $fields;
  }

  /**
   * Returns MigrationDestination instance.
   */
  public function migrateDestination($entity_bundle) {
    return new MigrateDestinationNode($entity_bundle);
  }

  /**
   * Returns key schema for migration.
   */
  public function migrateDestinationMap() {
    return MigrateDestinationNode::getKeySchema();
  }
}
