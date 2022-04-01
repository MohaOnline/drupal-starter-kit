<?php

namespace Drupal\campaignion_activity;

use \Drupal\campaignion\Contact;

class ActivityBase implements ActivityInterface {
  public $activity_id = NULL;
  public $contact_id;
  public $type;
  public $created;
  protected $original = NULL;

  public function __construct($data = array()) {
    foreach ($data as $k => &$v) {
      $this->{$k} = &$v;
    }
    if (!isset($this->created)) {
      $this->created = time();
    }
    if ($this->activity_id) {
      $this->original = clone $this;
    }
  }

  /**
   * Load an activity by it’s id.
   *
   * @param int $activity_id
   *   The ID of the activity to load.
   */
  public static function load($activity_id) {
    $query = static::buildJoins();
    $query->condition('a.activity_id', $activity_id);
    return $query->execute()->fetchObject(static::class);
  }

  /**
   * Build the database query for getting activities.
   */
  protected static function buildJoins() {
    $query = db_select('campaignion_activity', 'a')
      ->fields('a');
    return $query;
  }

  public function save() {
    if (isset($this->activity_id)) {
      $this->update();
    } else {
      $this->insert();
    }
    // Let other modules react on saving an activity.
    module_invoke_all('campaignion_activity_save', $this, $this->original);
  }

  public function delete() {
    if (isset($this->activity_id)) {
      db_delete('campaignion_activity')->condition('activity_id', $this->activity_id)->execute();
    }
  }

  protected function values($keys) {
    $data = array();
    foreach ($keys as $k) {
      $data[$k] = isset($this->{$k}) ? $this->{$k} : NULL;
    }
    return $data;
  }

  protected function update() {
    db_update('campaignion_activity')
      ->condition('activity_id', $this->activity_id)
      ->fields($this->values(array('contact_id', 'type', 'created')))
      ->execute();
  }
  protected function insert() {
    $this->activity_id = db_insert('campaignion_activity')
      ->fields($this->values(array('contact_id', 'type', 'created')))
      ->execute();
  }

  public function contact() {
    return Contact::load($this->contact_id);
  }
}
