<?php
/**
 * @file
 * Destination handler for User.
 */

$plugin = array(
  'label' => t('User'),
  'handler' => array(
    'class' => 'UserDataDestinationHandler',
  ),
);

class UserDataDestinationHandler extends CRMCoreDataDestinationHandler {
  /**
   * Default construct.
   */
  public function __construct() {
    $this->entityType = 'user';
  }

  /**
   * Build array of available fields.
   */
  public function getFields($bundle) {
    $fields = parent::getFields($bundle);
    $fields['name'] = t('Username');
    $fields['mail'] = t('Email');
    $fields['pass'] = t('Password');
    $fields['theme'] = t('Theme');
    $fields['signature'] = t('Signature');
    $fields['signature_format'] = t('Signature format');
    $fields['created'] = t('Created');
    $fields['access'] = t('Access');
    $fields['login'] = t('Login');
    $fields['status'] = t('Status');
    $fields['timezone'] = t('Timezone');
    $fields['language'] = t('Language');
    $fields['picture'] = t('Picture');
    $fields['init'] = t('Init');
    return $fields;
  }

  /**
   * Returns MigrationDestination instance.
   */
  public function migrateDestination($entity_bundle) {
    return new MigrateDestinationUser();
  }

  /**
   * Returns key schema for migration.
   */
  public function migrateDestinationMap() {
    return MigrateDestinationUser::getKeySchema();
  }
}
