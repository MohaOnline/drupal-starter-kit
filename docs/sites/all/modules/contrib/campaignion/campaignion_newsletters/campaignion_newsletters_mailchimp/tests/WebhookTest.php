<?php

namespace Drupal\campaignion_newsletters_mailchimp;

use Drupal\campaignion_newsletters\Subscription;

/**
 * Test the webhook.
 */
class WebhookTest extends \DrupalWebTestCase {

  public function setUp() : void {
    $this->subscription = Subscription::byData(4711, 'mc-webhook-test@example.com');
    $this->subscription->save(TRUE);
    $this->post = $_POST;
  }

  /**
   * Test an unsubscribe via MailChimp.
   */
  public function testWebhookUnsubscribe() {
    $_POST = [
      'type' => 'unsubscribe',
      'data' => ['email' => 'mc-webhook-test@example.com'],
    ];
    $answer = campaignion_newsletters_mailchimp_webhook(4711);
    // Check that the subscription was deleted by the webhook.
    $this->assertTrue(Subscription::byData(4711, 'mc-webhook-test@example.com')->isNew());
    $this->assertEqual(['status' => 'OK'], $answer);
  }

  /**
   * Test a test request from MailChimp.
   *
   * MailChimp pings the webhook with a GET-request to test it’s availability.
   */
  public function testWebhookGet() {
    $answer = campaignion_newsletters_mailchimp_webhook(4711);
    $this->assertFalse(Subscription::byData(4711, 'mc-webhook-test@example.com')->isNew());
    $this->assertEqual(['status' => 'OK'], $answer);
  }

  public function tearDown() : void {
    $_POST = $this->post;
    $this->subscription->delete();
  }

}

