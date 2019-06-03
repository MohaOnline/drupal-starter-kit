<?php

namespace Drupal\campaignion\CRM\Import;

use \Drupal\campaignion\CRM\Import\Source\SourceInterface;
use \Drupal\campaignion\Contact;

interface ImporterInterface {
  /**
   * Import all mapped data from the $source into $contact.
   *
   * @param \Drupal\campaignion\CRM\Import\Source\SourceInterface $source
   *   A data-source for the import.
   * @param \Drupal\campaignion\Contact $contact
   *   All mapped data will be stored into this contact.
   *
   * @return bool
   *   Whether or not any changes were made to the contact object.
   */
  public function import(SourceInterface $source, Contact $contact);
  /**
   * Try to get or create the contact by it's email address.
   *
   * @param \Drupal\campaignion\CRM\Import\Source\SourceInterface $source
   *   A data-source for the import.
   *
   * @return \Drupal\campaignion\Contact
   *   A contact object if a contact was found or created. NULL if it the
   *   importer was unable to get an email address from the $source.
   */
  public function findOrCreateContact(SourceInterface $source);
}
