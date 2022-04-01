<?php

namespace Drupal\campaignion\CRM;

use \Drupal\campaignion\Contact;
use \Drupal\campaignion\CRM\Import\Source\SourceInterface;

class ExporterBase implements ExporterInterface {
  protected $map;
  protected $contact;
  protected $wrappedContact;
  public function __construct($mappings) {
    $this->map = $mappings;
    foreach ($this->map as $mapper) {
      $mapper->setExporter($this);
    }
  }

  public function getContact() {
    return $this->contact;
  }

  public function getWrappedContact() {
    return $this->wrappedContact;
  }

  /**
   * Check whether the exporter might have data for a given key.
   */
  public function hasKey($key) {
    return isset($this->map[$key]);
  }

  /**
   * Get the data for a key.
   */
  public function value($key) {
    if (isset($this->map[$key])) {
      return $this->map[$key]->value();
    }
    return NULL;
  }

  public function setContact(Contact $contact) {
    $this->contact = $contact;
    $this->wrappedContact = $contact->wrap();
    return $this;
  }

  public function getLanguageCode() {
    return NULL;
  }

}
