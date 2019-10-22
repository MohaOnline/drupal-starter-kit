<?php

namespace Drupal\campaignion\CRM\Export;

class KeyedField extends WrapperField {

  protected $subkey;

  public function __construct($key, $subkey) {
    parent::__construct($key);
    $this->subkey = $subkey;
  }

  /**
   * Get one sub-value of the selected field item.
   */
  public function value($delta = 0) {
    $get_value = function($value) {
      return $value ? $value[$this->subkey] : NULL;
    };
    $value = parent::value($delta);
    return is_null($delta) ? array_map($get_value, $value) : $get_value($value);
  }

}
