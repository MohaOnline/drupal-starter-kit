<?php

namespace Drupal\campaignion\CRM\Import\Source;

class ArraySource implements SourceInterface {
  protected $data;
  public function __construct($data) {
    $this->data = $data;
  }

  public function hasKey($key) {
    return array_key_exists($key, $this->data);
  }

  public function value($key) {
    return isset($this->data[$key]) ? $this->data[$key] : NULL;
  }

}
