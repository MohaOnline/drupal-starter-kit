<?php

/**
 * @file
 * Hook implementations for tracking newsletters subscription changes.
 */

use Drupal\campaignion\Contact;
use \Drupal\campaignion_activity\NewsletterSubscription;
use \Drupal\campaignion_newsletters\Subscription;

/**
 * Implements campaignion_newsletters_subscription_saved().
 */
function campaignion_activity_campaignion_newsletters_subscription_saved(Subscription $subscription, $from_provider, $was_new) {
  if ($was_new) {
    NewsletterSubscription::fromSubscription($subscription, 'subscribe', $from_provider)->save();
  }
}

/**
 * Implements campaignion_newsletters_subscription_deleted().
 */
function campaignion_activity_campaignion_newsletters_subscription_deleted(Subscription $subscription, $from_provider) {
  if ($contact = Contact::byEmail($subscription->email)) {
    NewsletterSubscription::fromSubscription($subscription, 'unsubscribe', $from_provider)->save();
  }
}
