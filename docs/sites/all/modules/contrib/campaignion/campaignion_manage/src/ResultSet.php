<?php

namespace Drupal\campaignion_manage;

class ResultSet extends \Drupal\little_helpers\DB\Model {
  protected static $table = 'campaignion_manage_result_meta';
  protected static $key = array('id');
  protected static $values = array('uid', 'step', 'created');
  protected static $serial = TRUE;
  public $id = NULL;
  public $uid;
  public $step = NULL;
  public $created = NULL;

  public function __construct(array $data = array(), $new = TRUE) {
    $data += array(
      'uid' => $GLOBALS['user']->uid,
      'created' => REQUEST_TIME,
    );
    parent::__construct($data, $new);
  }

  public static function loadOrCreate($step = NULL, $uid = NULL) {
    if (!($obj = static::load($step, $uid))) {
      $data = array('step' => $step);
      if ($uid) {
        $data['uid'] = $uid;
      }
      $obj = new static($data);
      $obj->save();
    }
    return $obj;
  }

  public static function load($step = NULL, $uid = NULL) {
    $uid = $uid ? $uid : $GLOBALS['user']->uid;
    $t = static::$table;
    return db_query("SELECT * FROM {{$t}} WHERE uid=:uid AND step=:step", array(':uid' => $uid, ':step' => $step))
      ->fetchObject(get_called_class());
  }

  public function startOver($step = NULL, $uid = NULL) {
    $uid = $uid ? $uid : $GLOBALS['user']->uid;
    static::purgeAll();
    return new static(array('uid' => $uid, 'step' => $step));
  }

  public function save() {
    if ($this->isNew()) {
      if ($old = static::load($this->uid, $this->step)) {
        $old->delete();
      }
    }
    parent::save();
  }

  public function delete() {
    if ($this->isNew()) {
      return;
    }
    $this->purge();
    parent::delete();
  }

  public function purge() {
    $filter = array(':id' => $this->id);
    db_query('DELETE FROM {campaignion_manage_result} WHERE meta_id=:id', $filter);
  }

  public function resetFromQuery(\SelectQueryInterface $query) {
    $fields = $query->getFields();
    $expressions = $query->getExpressions();
    $this->created = REQUEST_TIME;
    $this->save();
    if (!isset($fields['meta_id']) && !isset($expressions['meta_id'])) {
      $query->addExpression($this->id, 'meta_id');
    }
    $this->purge();
    db_insert('campaignion_manage_result')->from($query)->execute();
  }

  public function joinTo(\SelectQueryInterface $query) {
    $query->innerJoin('campaignion_manage_result', 'cmr', 'cmr.contact_id=r.contact_id AND cmr.meta_id=:meta_id', array(':meta_id' => $this->id));
  }

  public function asQuery($alias = 'r') {
    return db_select('campaignion_manage_result', $alias)
      ->fields($alias, array('contact_id'))
      ->condition("$alias.meta_id", $this->id)
      ->distinct();
  }

  public static function purgeAll($uid = NULL) {
    $uid = $uid ? $uid : $GLOBALS['user']->uid;
    $t = static::$table;
    db_query("DELETE FROM {campaignion_manage_result} WHERE meta_id IN (SELECT id from {{$t}} WHERE uid=:uid)", array(':uid' => $uid));
    db_query("DELETE FROM {{$t}} WHERE uid=:uid", array(':uid' => $uid));
  }

  public function count() {
    $filter = array(':id' => $this->id);
    $result = db_query('SELECT count(*) FROM {campaignion_manage_result} WHERE meta_id=:id', $filter);
    return $result->fetchField();
  }

  public function nextIds($start, $limit) {
    $filter = array(':id' => $this->id, ':start' => $start);
    $result = db_query_range('SELECT contact_id FROM {campaignion_manage_result} WHERE meta_id=:id AND contact_id>:start ORDER BY contact_id', 0, $limit, $filter);
    return $result->fetchCol();
  }
}
