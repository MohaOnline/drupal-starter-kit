<?php

namespace Drupal\campaignion\CRM\Import\Field;

class Name extends Field {
  public function preprocessField($value) {
    return ucwords(strtolower($value));
  }
}
