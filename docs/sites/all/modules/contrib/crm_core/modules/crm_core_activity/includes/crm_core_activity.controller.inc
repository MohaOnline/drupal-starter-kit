<?php

/**
 * @file
 * Extends EntityAPIControllerInterface with classes for Activities.
 */

/**
 * CRM Core Activity Entity Class.
 */
class CRMCoreActivityEntity extends Entity {
  protected function defaultLabel() {
    return $this->title;
  }

  protected function defaultUri() {
    return array(
      'path' => 'crm-core/activity/' . $this->identifier(),
      'options' => array(
        'absolute' => TRUE,
      ),
    );
  }
}

/**
 * Controller class for CRM Core Activity.
 */
class CRMCoreActivityController extends EntityAPIController {
  public function create(array $values = array()) {
    $values += array(
      'title' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $GLOBALS['user']->uid,
    );

    return parent::create($values);
  }

  public function save($activity) {
    $activity->changed = time();

    return parent::save($activity);
  }

  /**
   * Update activity object before saving revision.
   */
  protected function saveRevision($entity) {
    $entity->uid = $GLOBALS['user']->uid;
    $entity->is_new_revision = TRUE;

    return parent::saveRevision($entity);
  }
}

/**
 * Provides a class for activities.
 *
 * Use a separate class so we can specify some defaults modules may alter.
 */
class CRMActivityType extends Entity {
  public $type;
  public $label;
  public $weight = 0;
  public $activity_string;

  public function __construct($values = array()) {
    parent::__construct($values, 'crm_core_activity_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * Controller class for CRM Core Activity Type.
 */
class CRMCoreActivityTypeController extends EntityAPIControllerExportable {
  public function create(array $values = array()) {
    $values += array(
      'type' => '',
      'label' => '',
      'description' => '',
      'is_new' => TRUE,
    );

    return parent::create($values);
  }
}

/**
 * CRMCoreActivityTypeFeaturesController class.
 */
class CRMCoreActivityTypeFeaturesController extends EntityDefaultFeaturesController {

  /**
   * Overridden to prevent data loss on activity type delete.
   */
  public function revert($module = NULL) {
    if ($defaults = features_get_default($this->type, $module)) {
      foreach ($defaults as $name => $activity_type) {
        db_delete('crm_core_activity_type')
          ->condition('type', $name)
          ->execute();
        crm_core_activity_type_save($activity_type);
      }
    }
  }
}
