<?php

namespace Drupal\campaignion_email_to_target\Api;

/**
 * Metadata for Email-To-Target datasets.
 */
class Dataset {

  public $key;
  public $title;
  public $description;
  public $attributes;
  public $isCustom;
  public $selectors;

  /**
   * Construct new instance from array as given by the API.
   *
   * @param array $data
   *   Data as it was received from the e2t API.
   */
  public static function fromArray(array $data) {
    $attributes = [];
    foreach ($data['attributes'] as $attribute_data) {
      $attributes[] = Attribute::fromArray($attribute_data);
    }
    $data['attributes'] = $attributes;
    return new static($data);
  }

  /**
   * Create a new instance.
   *
   * @param array $data
   *   Array of attribute values.
   */
  public function __construct(array $data = []) {
    foreach ($data as $key => $value) {
      $attribute = str_replace('_', '', lcfirst(ucwords($key, '_')));
      $this->{$attribute} = $value;
    }
  }

}
