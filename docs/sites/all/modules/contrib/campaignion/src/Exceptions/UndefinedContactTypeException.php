<?php

namespace Drupal\campaignion\Exceptions;

class UndefinedContactTypeException extends \Exception {
  public function __construct($type) {
    $error = 'No contact-type definition found for contact-type "@type". Contact types must be provided via hook_campaignion_contact_info()';
    parent::__construct(format_string($error, array('@type' => $type)));
  }
}
