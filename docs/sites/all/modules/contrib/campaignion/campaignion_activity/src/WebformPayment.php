<?php

namespace Drupal\campaignion_activity;

class WebformPayment extends WebformSubmission {
  public $type = 'webform_payment';

  protected static function buildJoins() {
    $query = parent::buildJoins();
    $query->innerJoin('campaignion_activity_payment', 'ap', 'ap.activity_id=a.activity_id');
    $query->fields('ap');
    return $query;
  }

  protected function insert() {
    parent::insert();
    db_merge('campaignion_activity_payment')
      ->key(['activity_id' => $this->activity_id])
      ->fields($this->values(['pid']))
      ->execute();
  }

  protected function update() {
    parent::update();
    db_merge('campaignion_activity_payment')
      ->key(['activity_id' => $this->activity_id])
      ->fields($this->values(['pid']))
      ->execute();
  }
}
