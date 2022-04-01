<?php

namespace Drupal\campaignion_expiry;

use Drupal\campaignion\Contact;
use Drupal\campaignion\ContactTypeManager;
use Drupal\campaignion\CRM\Import\Source\ArraySource;
use Drupal\campaignion_expiry\Anonymizer;
use Upal\DrupalUnitTestCase;

/**
 * Test anonymizing a contact.
 */
class AnonymizerTest extends DrupalUnitTestCase {

  /**
   * Create a test contact that can be anonymized.
   */
  public function setUp() : void {
    parent::setUp();
    $this->contact = Contact::fromBasicData('test@example.com', 'First', 'Last', 'contact');
    $this->contact->save();
  }

  /**
   * Delete our test contact created in set up when the tests are finished.
   */
  public function tearDown() : void {
    $this->contact->delete();
    parent::tearDown();
  }

  /**
   * Test anonymizing our contact.
   */
  public function testContactAnonymization() {
    $anonymizer = new Anonymizer(false);
    $anonymizer->anonymize($this->contact, false);
    $anonymizer->deleteOldRevisions($this->contact);

    $contact = entity_load_single('redhen_contact', $this->contact->contact_id);
    $old_contact = $this->contact;

    // Is a contact record still there?
    $this->assertNotEmpty($contact);
    // Did the first name change to "Anonymous" as specified in the Anonymizer?
    $this->assertEqual($contact->first_name, "Anonymous");
    // Did the revision change?
    $this->assertNotEquals($contact->revision_id, $old_contact->revision_id);
    // Was the old revision deleted?
    $this->assertFalse(entity_revision_load('redhen_contact', $old_contact->revision_id));
  }
}
