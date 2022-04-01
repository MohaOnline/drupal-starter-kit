<?php

namespace Drupal\campaignion_activity;

use Drupal\campaignion\CRM\Import\Source\ArraySource;
use Drupal\campaignion\ContactTypeManager;
use Drupal\little_helpers\Webform\Submission;
use Upal\DrupalUnitTestCase;

/**
 * Test newsletter subscription activities.
 */
class WebformSubmissionTest extends DrupalUnitTestCase {

  /**
   * Set up.
   */
  public function setUp() : void {
    parent::setUp();
    $source = new ArraySource([
      'email' => 'test@example.com',
      'first_name' => 'First',
      'last_name' => 'Last',
    ]);
    $importer = ContactTypeManager::instance()->importer('campaignion_action_taken');
    $this->contact = $importer->findOrCreateContact($source);
  }

  /**
   * Tear down.
   */
  public function tearDown() : void {
    $this->contact->delete();
    parent::tearDown();
  }

  /**
   * Test creating a simple webform submission activity.
   */
  public function testSubmission() {
    $node = (object) ['nid' => 420001, 'webform' => ['components' => []]];
    $submission = (object) [
      'nid' => $node->nid,
      'sid' => 420002,
      'confirmed' => 0,
      'contact' => $this->contact,
      'submitted' => $submitted_time = time() - 10,
    ];
    $submission = new Submission($node, $submission);
    $queued_time = time() - 5;
    campaignion_activity_campaignion_action_taken(NULL, $submission, $queued_time);

    $activity = WebformSubmission::byNidSid($submission->nid, $submission->sid);
    $this->assertNotEmpty($activity);
    $this->assertEqual($this->contact->contact_id, $activity->contact_id);
    $this->assertEqual($submitted_time, $activity->created);
  }

  /**
   * Test creating a webform submission and payment activity.
   */
  public function testSubmissionWithPayment() {
    $node = (object) ['nid' => 420001, 'webform' => ['components' => []]];
    $submission = (object) [
      'nid' => $node->nid,
      'sid' => 420002,
      'submitted' => 0,
      'confirmed' => 0,
      'contact' => $this->contact,
      'payments' => [$payment = new \Payment(['pid' => 420003])],
    ];
    $submission = new Submission($node, $submission);
    $payment->setStatus(new \PaymentStatusItem(PAYMENT_STATUS_SUCCESS));
    campaignion_activity_campaignion_action_taken(NULL, $submission, time());

    $activity = WebformSubmission::byNidSid($submission->nid, $submission->sid);
    $this->assertNotEmpty($activity);
    $this->assertEqual($this->contact->contact_id, $activity->contact_id);

    $payment_activity = WebformPayment::byNidSid($submission->nid, $submission->sid);
    $this->assertEqual($activity->activity_id, $payment_activity->activity_id);
    $this->assertEqual($payment->pid, $payment_activity->pid);
  }

}
