<?php

namespace Drupal\campaignion_wizard;

class Status extends \Drupal\little_helpers\DB\Model {
  protected static $table = 'campaignion_wizard_status';
  protected static $key = array('nid');
  protected static $values = array('step');
  protected static $serial = FALSE;

  public $nid;
  public $step;

  public static function loadOrCreate($nid) {
    $table = self::$table;
    $item = db_query("SELECT * FROM {{$table}} WHERE nid=:nid", array(':nid' => $nid))
      ->fetch();
    if ($item) {
      return new static($item, FALSE);;
    }
    return new static(array('nid' => $nid), TRUE);
  }
}
