<?php

namespace Drupal\campaignion_bar;

class ParseException extends \Exception {
  public function __construct($errors) {
    $errors = implode("\n", $errors);
    parent::__construct("Errors:\n$errors\n");
  }
}
