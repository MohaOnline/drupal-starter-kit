<?php

namespace Drupal\campaignion_activity;

use Drupal\campaignion_newsletters\Subscription;

/**
 * Test newsletter subscription activities.
 */
class NewsletterSubscriptionTest extends \DrupalWebTestCase {

  /**
   * Set up.
   */
  public function setUp() {
    parent::setUp(['campaignion_activity', 'campaignion_newsletters']);
    db_delete('campaignion_activity')->execute();
    db_delete('campaignion_activity_newsletter_subscription')->execute();
  }

  /**
   * Tear down.
   */
  public function tearDown() {
    db_delete('campaignion_newsletters_subscriptions')->execute();
    db_delete('campaignion_newsletters_queue')->execute();
    db_delete('campaignion_activity')->execute();
    db_delete('campaignion_activity_newsletter_subscription')->execute();
  }

  /**
   * Save and delete a subscription.
   */
  public function testSavingAndDeletingSubscription() {
    $email = 'bydataduplicate@test.com';
    $list_id = 4711;
    $s = Subscription::byData($list_id, $email);
    $s->save();

    $count = db_select('campaignion_activity_newsletter_subscription')
      ->condition('list_id', 4711)
      ->condition('action', 'subscribe')
      ->countQuery()->execute()->fetchField();
    $this->assertEqual(1, $count);

    // Simulate provider delete.
    $s->delete(TRUE);

    $count = db_select('campaignion_activity_newsletter_subscription')
      ->condition('list_id', 4711)
      ->condition('action', 'unsubscribe')
      ->countQuery()->execute()->fetchField();
    $this->assertEqual(1, $count);
  }

}
