<?php

namespace Drupal\campaignion\CRM;

use \Drupal\campaignion\CRM\Import\Source\SourceInterface;
use \Drupal\campaignion\Contact;

class ImporterBase {
  protected $mappings;
  protected $contactType;

  public function __construct(array $mappings, $type = NULL) {
    $this->mappings = $mappings;
    $this->contactType = $type;
  }

  public function import(SourceInterface $source, Contact $contact) {
    $w = $contact->wrap();
    $isNewOrUpdated = empty($contact->contact_id);
    foreach ($this->mappings as $mapper) {
      $isNewOrUpdated = $mapper->import($source, $w) || $isNewOrUpdated;
    }
    return $isNewOrUpdated;
  }

  /**
   * Create or find a contact using a source.
   *
   * @param \Drupal\campaignion\CRM\Import\Source\SourceInterface $source
   *   A contact data source.
   *
   * @return \Drupal\campaignion\Contact
   *   A new or existing contact matching this source.
   */
  public function findOrCreateContact(SourceInterface $source) {
    $email = $source->value('email');
    $lock = "campaignion_findOrCreateContact:{$this->contactType}:$email";
    try {
      // Use a lock to avoid multiple threads creating the same contact.
      while (!lock_acquire($lock, 5)) {
        lock_wait($lock, 30);
      }
      $contact = Contact::fromEmail($source->value('email'), $this->contactType);
      $contact->isNew = FALSE;
      if (!$contact->contact_id) {
        // Create contact immediately, so that successive calls to this function
        // will return the same contact.
        $contact->save();
        $contact->isNew = TRUE;
      }
      return $contact;
    }
    finally {
      lock_release($lock);
    }
  }
}
