<?php

namespace Drupal\campaignion_activity;

class WebformPayment extends WebformSubmission {
  protected $type = 'webform_payment';

  protected static function buildJoins() {
    $query = parent::buildJoins();
    $query->innerJoin('campaignion_activity_payment', 'ap', 'ap.activity_id=a.activity_id');
    $query->fields('ap');
    return $query;
  }

  public static function byPayment(\Payment $payment) {
    $query = static::buildJoins();
    $query->condition('ap.pid', $payment->pid);
    return $query->execute()->fetchObject(get_called_class());
  }

  public static function fromPayment(\Payment $payment, $data = array()) {
    $data['pid'] = $payment->pid;
    $submission = $payment->contextObj->getSubmission();
    return static::fromSubmission($submission->getNode(), $submission->unwrap(), $data);
  }

  protected function insert() {
    parent::insert();
    db_insert('campaignion_activity_payment')
      ->fields($this->values(array('activity_id', 'pid')))
      ->execute();
  }

  protected function update() {
    parent::update();
    db_update('campaignion_activity_payment')
      ->fields($this->values(array('pid')))
      ->condition('activity_id', $this->activity_id)
      ->execute();
  }
}
