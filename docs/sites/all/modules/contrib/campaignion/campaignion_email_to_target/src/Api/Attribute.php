<?php

namespace Drupal\campaignion_email_to_target\Api;

/**
 * Datatype for representing an attribute as specified by the e2t-service.
 */
class Attribute {
  public $key;
  public $title;
  public $description;

  /**
   * Create an attribute object by passing its data.
   *
   * @param string $key
   *   The key for this attribute (ie. used in the token names).
   * @param string $title
   *   Human readable title for this attribute.
   * @param string $description
   *   More explanation about the attribute.
   */
  public function __construct($key, $title = '', $description = '') {
    $this->key = $key;
    $this->title = $title;
    $this->description = $description;
  }

  /**
   * Create Attribute instance from an array (JSON-object).
   *
   * @param array $data
   *   Associative array as given by the e2t service.
   */
  public static function fromArray(array $data) {
    return new static($data['key'], $data['title'], $data['description']);
  }

}
