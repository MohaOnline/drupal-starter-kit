<?php

namespace Drupal\campaignion\CRM;

use \Drupal\campaignion\Contact;

interface ExporterInterface extends Import\Source\SourceInterface {
  /**
   * Get the contact object the we currently work on.
   *
   * @return \Drupal\campaignion\Contact
   */
  public function getContact();
  /**
   * Get the contact object wrapped with entity_metadata_wrapper().
   *
   * @return \EntityMetadataWrapper
   */
  public function getWrappedContact();
  /**
   * Get a contact value by key.
   *
   * @param string $key
   *   The key to get the value for.
   *
   * @return
   *   The value associated with the key or NULL if there is none.
   */
  public function value($key);
  /**
   * Set the contact wrapped by this exporter.
   *
   * @param \Drupal\campaignion\Contact $contact
   *   The contact that the exporter should work on.
   *
   * @return ExporterInterface
   *   The exporter itself for chainability.
   */
  public function setContact(Contact $contact);
}
