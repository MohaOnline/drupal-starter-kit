<?php

/**
 * CRM Contact Type Entity Class.
 */
class CRMContactType extends Entity {

  public $type;
  public $label;
  public $weight = 0;
  public $primary_fields;

  public function __construct($values = array()) {
    parent::__construct($values, 'crm_core_contact_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * @file
 * Controller class for contacts.
 *
 * This extends the DrupalDefaultEntityController class, adding required
 * special handling for contact objects.
 */
class CRMCoreContactTypeController extends EntityAPIControllerExportable {

  public function create(array $values = array()) {
    $values += array(
      'name' => '',
      'description' => '',
    );

    return parent::create($values);
  }

  /**
   * Overrides a method of the EntityAPIControllerExportable class.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);

    // Reset cache entry, important for usecases like install procedure.
    drupal_static_reset('crm_core_contact_types');
  }
}

/**
 * CRMCoreContactTypeFeaturesController class.
 */
class CRMCoreContactTypeFeaturesController extends EntityDefaultFeaturesController {

  /**
   * Overridden to prevent data loss on contact type delete.
   */
  public function revert($module = NULL) {
    if ($defaults = features_get_default($this->type, $module)) {
      foreach ($defaults as $name => $contact_type) {
        db_delete('crm_core_contact_type')
          ->condition('type', $name)
          ->execute();
        crm_core_contact_type_save($contact_type);
      }
    }
  }
}
