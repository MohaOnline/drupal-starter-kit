<?php

namespace Drupal\campaignion_activity;

use \Drupal\campaignion\ContactTypeManager;
use \Drupal\campaignion\CRM\Import\Source\WebformSubmission as Submission;

class WebformSubmission extends ActivityBase {
  protected $type = 'webform_submission';

  public $sid;
  public $nid;
  public $confirmed = NULL;

  public static function bySubmission($node, $submission) {
    return static::byNidSid($node->nid, $submission->sid);
  }

  public static function load($activity_id) {
    $query = static::buildJoins();
    $query->condition('a.activity_id', $activity_id);
    return $query->execute()->fetchObject(get_called_class());
  }

  public static function byNidSid($nid, $sid) {
    $query = static::buildJoins();
    $query->condition('aw.nid', $nid)
          ->condition('aw.sid', $sid);
    return $query->execute()->fetchObject(get_called_class());
  }

  protected static function buildJoins() {
    $query = db_select('campaignion_activity', 'a')
      ->fields('a');
    $query->innerJoin('campaignion_activity_webform', 'aw', 'aw.activity_id=a.activity_id');
    $query->fields('aw');
    return $query;
  }

  public static function fromSubmission($node, $submission, $data = array()) {
    if ($activity = static::bySubmission($node, $submission)) {
      return $activity;
    }
    $importer = ContactTypeManager::instance()->importer('campaignion_activity');
    $source = new Submission($node, $submission);
    $contact = $importer->findOrCreateContact($source);
    if ($importer->import($source, $contact)) {
      $contact->save();
    }

    $data = array(
      'confirmed' => $source->webform->needsConfirmation() ? NULL : REQUEST_TIME,
      'contact_id' => $contact->contact_id,
      'nid' => $node->nid,
      'sid' => $submission->sid,
    ) + $data;
    return new static($data);
  }

  protected function insert() {
    parent::insert();
    db_insert('campaignion_activity_webform')
      ->fields($this->values(array('activity_id', 'nid', 'sid', 'confirmed')))
      ->execute();
  }

  protected function update() {
    parent::update();
    db_update('campaignion_activity_webform')
      ->fields($this->values(array('nid', 'sid', 'confirmed')))
      ->condition('activity_id', $this->activity_id)
      ->execute();
  }

  // @TODO: Use full objects instead of nid/sid by default instead of always loading them.
  public function node() {
    return node_load($this->nid);
  }

  public function submission() {
    return \Drupal\little_helpers\Webform\Submission::load($this->nid, $this->sid);
  }
}
