<?php

namespace Drupal\campaignion;

use \Drupal\campaignion\CRM\Import\Source\SourceInterface;

class Contact extends \RedhenContact {
  public function __construct($values = array()) {
    $objValues = array();
    if (is_object($values)) {
      $objValues = $values;
      $values = array();
    }
    parent::__construct($values);
    foreach ($objValues as $key => $value) {
      $this->$key = $value;
    }
    if (!$this->type) {
      $this->type = static::defaultType();
    }
    if (!$this->created) {
      $this->created = REQUEST_TIME;
    }
    // Make sure all fields are represented as properties on the object.
    // - Saving an empty contact with an existing contact’s ID should erase all
    //   previous contact data.
    // - You can check for a field’s existence simply by checking for its
    //   property on the object.
    foreach (field_info_instances('redhen_contact', $this->type) as $instance) {
      if (!isset($this->{$instance['field_name']})) {
        $this->{$instance['field_name']} = [];
      }
    }
  }

  public static function defaultType() {
    return variable_get('campaignion_contact_type_supporter', 'contact');
  }

  public static function load($id) {
    return \redhen_contact_load($id);
  }

  public static function idByEmail($email, $type = NULL) {
    $type = $type ? $type : static::defaultType();
    $sql = <<<SQL
SELECT entity_id
FROM field_data_redhen_contact_email
  INNER JOIN redhen_contact ON entity_id=contact_id
WHERE redhen_contact_email_value = :email AND type = :bundle AND redhen_state = 1
SQL;
    return db_query($sql, array(':email' => $email, ':bundle' => $type))->fetchField();
  }

  public static function idFromBasicData($email, $first_name = '', $last_name = '', $type = NULL) {
    $contact = static::fromBasicData($email, $first_name, $last_name, $type);
    if (!$contact->contact_id) {
      $contact->save();
    }
    return $contact->contact_id;
  }

  public static function fromBasicData($email, $first_name = '', $last_name = '', $type = NULL) {
    $contact = static::fromEmail($email, $type);
    if (!$contact->contact_id) {
      $contact->first_name = $first_name;
      $contact->last_name = $last_name;
    }
    return $contact;
  }

  public static function byEmail($email, $type = NULL) {
    if ($id = static::idByEmail($email, $type)) {
      return static::load($id);
    }
  }

  public static function fromEmail($email, $type = NULL) {
    if (!$email) {
      throw new NoEmailException("The email address must be a non-empty string. Got '$email' instead.");
    }
    $type = $type ? $type : static::defaultType();
    if (!($contact = static::byEmail($email, $type))) {
      $contact = entity_create('redhen_contact', ['type' => $type]);
      $contact->setEmail($email, 1, 0);
    }
    return $contact;
  }

  public function wrap() {
    return entity_metadata_wrapper('redhen_contact', $this);
  }

  public function exportable($target) {
    if ($exporter = ContactTypeManager::instance()->exporter($target, $this->type)) {
      $exporter->setContact($this);
      return $exporter;
    }
  }
}
