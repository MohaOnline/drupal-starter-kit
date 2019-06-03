<?php

namespace Drupal\campaignion\CRM\Export;

class DateField extends WrapperField {
  protected $format;
  public function __construct($key, $format) {
    parent::__construct($key);
    $this->format = $format;
  }
  public function value() {
    if ($timestamp = parent::value()) {
      return strftime($this->format, $timestamp);
    }
  }
}
