<?php

namespace Drupal\campaignion;

/**
 * Test contact objects.
 */
class ContactTest extends \DrupalUnitTestCase {

  /**
   * Remove the test contact if needed.
   */
  public function tearDown() {
    if ($contact = Contact::byEmail('test@example.com')) {
      $contact->delete();
    }
    parent::tearDown();
  }

  /**
   * Test metadata wrappers for contacts.
   */
  public function testWrapContact() {
    $contact = Contact::fromBasicData('my@email.com', 'first', 'last');
    $this->assertEqual('first', $contact->wrap()->first_name->value());
  }

  /**
   * Test that properties for fiedls do exist after contact creation.
   */
  public function testFieldProperties() {
    $contact = new Contact();
    $this->assertObjectHasAttribute('source_tag', $contact);
    $this->assertObjectHasAttribute('supporter_tags', $contact);
  }

  /**
   * Test emptying a contact by storing a new empty contact with the same ID.
   */
  public function testAnonymizeContact() {
    $contact = new Contact();
    $contact->setEmail('test@example.com', 1, 0);
    $contact->wrap()->field_title->set('Ms');
    $contact->save();
    $new_contact = new Contact();
    $new_contact->redhen_contact_email = $contact->redhen_contact_email;
    $new_contact->contact_id = $contact->contact_id;
    $new_contact->save();

    $stored_contact = Contact::load($contact->contact_id);
    $this->assertEquals('test@example.com', $stored_contact->email());
    $this->assertEmpty($stored_contact->wrap()->field_title->value());
  }

  /**
   * Test zero creation time is left as is.
   */
  public function testZeroCreationTime() {
    $contact = new Contact(['created' => 0]);
    $this->assertEqual(0, $contact->created);
  }

  /**
   * Test looping over contacts.
   */
  public function testApply() {
    Contact::fromBasicData('test@example.com', 'first', 'last')->save();
    $emails = [];
    // Take note of all contacts’ email addresses.
    Contact::apply(function ($contact) use (&$emails) {
      $emails[] = $contact->email();
    });
    $this->assertEqual('test@example.com', end($emails));
  }

}
