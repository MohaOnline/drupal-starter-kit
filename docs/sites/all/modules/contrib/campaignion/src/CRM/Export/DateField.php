<?php

namespace Drupal\campaignion\CRM\Export;

class DateField extends WrapperField {

  protected $format;

  public function __construct($key, $format) {
    parent::__construct($key);
    $this->format = $format;
  }

  public function value($delta = 0) {
    $format = function ($timestamp) {
      return strftime($this->format, $timestamp);
    };
    $value = parent::value($delta);
    if (isset($value)) {
      return is_null($delta) ? array_map($format, $value) : $format($value);
    }
  }

}
