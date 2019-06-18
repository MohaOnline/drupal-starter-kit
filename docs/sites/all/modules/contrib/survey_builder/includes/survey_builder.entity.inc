<?php
/**
 * @file
 * Classes and related functions/hooks for question and question response entity types
 */

/**
 * Base class for all survey builder entities.
 */
class SurveyBuilderEntity extends Entity {

  protected $typeName = 'entity';

  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($this->getInitialValues($values), $this->typeName);
  }

  public function getInitialValues(array $values = array()) {
    global $user;
    if (!empty($values['name_raw'])) {
      $values['name'] = $this->getMachineName($values['name_raw']);
    }
    /*
     * @TODO All the entity properties should be initialised within a create()
     * method of EntityController (sub) class otherwise, when the entity is loaded
     * from db, all the properties values are always overwritten with the
     * ones defined here (EntityAPIController::query() uses PDO::FETCH_CLASS as
     * fetch mode; only PDO::FETCH_CLASS|PDO::FETCH_PROPS_LATE causes the
     * constructor to be called before assigning the results to the properties)
     */
    $values['uid']     = isset($this->uid) ? $this->uid : $user->uid;
    $values['created'] = isset($this->created) ? $this->created : time();
    return $values;
  }

  public function getMachineName($name_raw) {
    $name = preg_replace(array('/\s+/', '/[^a-z0-9_]+/'), array('_', ''), strtolower($name_raw));
    $attempt = 0;
    do {
      if ($attempt == 0) {
        if (strlen($name) > 255) {
          $machine_name = substr($name, 0, 255);
        }
        else {
          $machine_name = $name;
        }
      }
      else {
        $attempt_str = (string)$attempt;
        if (strlen($name . $attempt_str) > 255) {
          $machine_name = substr($name, 0, 255-strlen($attempt_str));
        }
        else {
          $machine_name = $name;
        }
        $machine_name .= $attempt_str;
      }
      $query = new EntityFieldQuery;
      $result = $query
        ->entityCondition('entity_type', $this->typeName)
        ->propertyCondition('name', $machine_name)
        ->execute();
    ++$attempt;
    } while(!empty($result));
    return $machine_name;
  }

  /**
   * Specifies the default uri, which is picked up by uri() by default.
   */
  protected function defaultURI() {
    return array('path' => $this->typeName . '/' . $this->identifier());
  }

}

/**
 * Base metadata class for all survey builder entities.
 */
class SurveyBuilderEntityMetadata extends EntityDefaultMetadataController {

  public function __construct($type, $data = NULL, $info = array()) {
      parent::__construct($type, NULL, $info);
  }

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties =& $info[$this->type]['properties'];
    $properties['uid']['label'] = t('User');
    $properties['uid']['type'] = 'user';
    $properties['created']['type'] = 'date';
    return $info;
  }

}
