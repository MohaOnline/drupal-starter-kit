<?php

namespace Drupal\campaignion_expiry;

use Drupal\campaignion\Contact;

/**
 * Anonymization service for redhen contacts.
 */
class Anonymizer {

  /**
   * Anonymize a single contact.
   *
   * @param bool $keep_mp_fields
   *   If TRUE the MP data is copied to the anonymized contacts, otherwise it is
   *   deleted.
   */
  public function __construct($contact, bool $keep_mp_fields = FALSE) {
    $this->contact = $contact;
    $this->keepMpFields = $keep_mp_fields;
    $this->entityController = entity_get_controller('redhen_contact');
  }

  /**
   * Create a new anonymous contact and copy over some data we want to keep.
   *
   * @param \Drupal\campaignion\Contact $contact
   *   Contact that is being anonymized.
   */
  public function anonymize(Contact $contact) {
    $new_contact = new Contact();
    $new_contact->contact_id = $contact->contact_id;
    $new_contact->redhen_state = REDHEN_STATE_ARCHIVED;
    $new_contact->setEmail("{$contact->contact_id}@deleted");

    $new_contact->first_name = 'Anonymous';
    $new_contact->last_name = $contact->contact_id;

    // Copy first country of $contact to $new_contact.
    foreach ($contact->wrap()->field_address->value() as $item) {
      if (!empty($item['country'])) {
        $new_contact->wrap()->field_address->set([['country' => $item['country']]]);
        break;
      }
    }

    // Copy tags of $contact to $new_contact.
    $new_contact->supporter_tags = $contact->supporter_tags;
    $new_contact->campaign_tag = $contact->campaign_tag;
    $new_contact->source_tag = $contact->source_tag;

    // Copy MP fields of $contact to $new_contact.
    if ($this->keepMpFields) {
      $new_contact->mp_constituency = $contact->mp_constituency;
      $new_contact->mp_country = $contact->mp_country;
      $new_contact->mp_party = $contact->mp_party;
      $new_contact->mp_salutation = $contact->mp_salutation;
    }

    $new_contact->created = $contact->created;
    $new_contact->log = "Contact “{$contact->contact_id}” and has been anonymized";
    $new_contact->save();

    $this->entityController->resetCache([$contact->contact_id]);
  }

  /**
   * Delete all but the last revision of a contact.
   *
   * @param \Drupal\campaignion\Contact $contact
   *   The contact which’s revisions should be removed.
   */
  public function deleteOldRevisions(Contact $contact) {
    $sql_revisions = <<<SQL
SELECT revision_id
FROM redhen_contact_revision
WHERE contact_id=:current_contact_id
ORDER BY revision_id;
SQL;

    $current_contact_id = $contact->contact_id;
    $revisions = db_query($sql_revisions, [':current_contact_id' => $current_contact_id])->fetchCol();
    foreach ($revisions as $revision) {
      entity_revision_delete('redhen_contact', $revision);
    }
  }
}
