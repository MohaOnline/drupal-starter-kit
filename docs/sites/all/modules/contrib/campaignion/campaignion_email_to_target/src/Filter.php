<?php

namespace Drupal\campaignion_email_to_target;

use \Drupal\little_helpers\DB\Model;
use \Drupal\little_helpers\Webform\Submission;


class Filter extends Model {
  protected static $table  = 'campaignion_email_to_target_filters';
  protected static $key = ['id'];
  protected static $values = ['message_id', 'weight', 'type', 'config'];
  protected static $serialize = ['config' => TRUE];

  public $id;
  public $message_id;
  public $weight = 0;
  public $type;
  public $config = [];

  public static function fromArray($data) {
    return new static($data);
  }

  public function __construct($data = array(), $new = TRUE) {
    parent::__construct($data, $new);

    if ($this->type == 'target-attribute') {
      if (strpos($this->config['attributeName'], '.') === FALSE) {
        $this->config['attributeName'] = 'contact.' . $this->config['attributeName'];
      }
    }
  }

  /**
   * Update filter data from array.
   */
  public function setData($data) {
    unset($data['id']);
    unset($data['message_id']);
    $this->__construct($data);
  }

  /**
   * Get filters for given message_ids.
   *
   * @param array $ids
   *   Message IDs to get the filters for.
   * @return array
   *   Filters ordered by message_id and weight, and keyed by their Id.
   */
  public static function byMessageIds($ids) {
    // DB queries doesn't work well with empty arrays in IN() clauses.
    if (!$ids) {
      return [];
    }
    $result = db_select(static::$table, 'f')
      ->fields('f')
      ->condition('message_id', $ids)
      ->orderBy('message_id')
      ->orderBy('weight')
      ->execute();
    $filters = [];
    foreach ($result as $row) {
      $filters[$row->id] = new static($row, FALSE);
    }
    return $filters;
  }

  public function toArray() {
    $data = [];
    foreach (array_merge(static::$key, static::$values) as $k) {
      $data[$k] = $this->$k;
    }
    unset($data['weight']);
    unset($data['message_id']);
    return $data;
  }

  public function match($target) {
    if ($this->type == 'target-attribute') {
      $data['contact'] = $target;
      foreach ($target as $key => $sub_array) {
        if (is_array($sub_array)) {
          $data[$key] = $sub_array;
        }
      }
      $name = $this->config['attributeName'];
      $key_exists = NULL;
      $value = drupal_array_get_nested_value($data, explode('.', $name), $key_exists);
      return $key_exists ? $this->matchValue($value) : FALSE;
    }
    return TRUE;
  }

  protected function matchValue($target_value) {
    $value = $this->config['value'];
    switch ($this->config['operator']) {
      case '==':
        return $target_value == $value;
      case '!=':
        return $target_value != $value;
      case 'regexp':
        return (bool) preg_match("/$value/", $target_value);
    }
    return FALSE;
  }

  /**
   * Clear out all IDs in order to create a real copy.
   */
  public function __clone() {
    $this->id = NULL;
    $this->new = TRUE;
  }

}
