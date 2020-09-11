<?php

namespace Drupal\campaignion_donation;

use Drupal\little_helpers\Webform\Submission;
use Upal\DrupalWebTestCase;

/**
 * Test setting the donor segmentation data based on submissions.
 */
class DonorSegmentationTest extends DrupalWebTestCase {

  /**
   * Generate a set of empty test data.
   */
  protected function emptyTestData() {
    $contact = entity_create('redhen_contact', ['type' => 'contact']);
    $payment = entity_create('payment', []);
    $submission = (object) [
      'payments' => [$payment],
    ];
    $node = (object) ['webform' => ['components' => []]];
    $submission = new Submission($node, $submission);
    return [$contact, $node, $submission, $payment];
  }

  /**
   * Test importing a one-off payment.
   */
  public function testOneOffEmptyContact() {
    list($contact, $node, $submission, $payment) = $this->emptyTestData();
    $payment->setLineItem(new \PaymentLineItem([
      'name' => 'foo',
      'currency' => 'XXX',
      'amount' => 2.0,
    ]));

    // Payment status is not yet a success → No import.
    $changed = campaignion_donation_campaignion_action_contact_alter($contact, $submission, $node);
    $this->assertFalse($changed);

    $payment->setStatus(new \PaymentStatusItem(PAYMENT_STATUS_SUCCESS, 12345));

    $changed = campaignion_donation_campaignion_action_contact_alter($contact, $submission, $node);
    $this->assertTrue($changed);
    $field = $contact->wrap()->donation_latest_one_off;
    $this->assertEqual(2.0, $field->donation_value->value());
    $this->assertEqual(12345, $field->donation_date->value());
    $this->assertNull($contact->wrap()->donation_latest_regular->value());

    // Importing the same data again should yield false.
    $changed = campaignion_donation_campaignion_action_contact_alter($contact, $submission, $node);
    $this->assertFalse($changed);
  }

  /**
   * Test importing a successful mixed payment updating a contact.
   */
  public function testMixedPaymentWithExistingContact() {
    list($contact, $node, $submission, $payment) = $this->emptyTestData();
    $item = entity_create('field_collection_item', ['field_name' => 'donation_latest_one_off']);
    $item->setHostEntity('redhen_contact', $contact);
    $wrapped_item = entity_metadata_wrapper('field_collection_item', $item);
    $wrapped_item->donation_value->set(2.0);
    $wrapped_item->donation_date->set(12345);
    $payment->setStatus(new \PaymentStatusItem(PAYMENT_STATUS_SUCCESS, 12345));
    $payment->setLineItem(new \PaymentLineItem([
      'name' => 'foo',
      'currency' => 'XXX',
      'amount' => 2.0,
    ]));
    $payment->setLineItem(new \PaymentLineItem([
      'name' => 'bar',
      'currency' => 'XXX',
      'amount' => 3.0,
      'recurrence' => (object) ['interval_unit' => 'monthly'],
    ]));

    $changed = campaignion_donation_campaignion_action_contact_alter($contact, $submission, $node);
    $this->assertTrue($changed);
    $field = $contact->wrap()->donation_latest_one_off;
    $this->assertEqual(2.0, $field->donation_value->value());
    $this->assertEqual(12345, $field->donation_date->value());

    $field = $contact->wrap()->donation_latest_regular;
    $this->assertEqual(3.0, $field->donation_value->value());
    $this->assertEqual(12345, $field->donation_date->value());

    // Importing the same data again should yield false.
    $changed = campaignion_donation_campaignion_action_contact_alter($contact, $submission, $node);
    $this->assertFalse($changed);
  }

}
