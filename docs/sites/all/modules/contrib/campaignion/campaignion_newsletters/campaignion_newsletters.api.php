<?php

/**
 * @file
 *
 * Hook documentation and examples. The code in this file is never actually
 * executed.
 */

use \Drupal\campaignion_newsletters\Subscription;

/**
 * Alter a newsletter subscription prior to being saved.
 *
 * @param \Drupal\campaignion_newsletters\Subscription $subscription
 *   The subscription that is going to be saved.
 */
function hook_campaignion_newsletters_subscription_presave(Subscription $subscription) {
  if ($subscription->email == 'just-for-testing@example.com') {
    $subscription->delete = TRUE;
  }
}

/**
 * React to a newsletter subscription being saved.
 *
 * @param \Drupal\campaignion_newsletters\Subscription $subscription
 *   The subscription that was inserted into / updated in the database.
 * @param bool $from_provider
 *   TRUE if the change was initiated by the newsletter provider.
 * @param bool $was_new
 *   TRUE if the subscription was new.
 */
function hook_campaignion_newsletters_subscription_saved(Subscription $subscription, $from_provider, $was_new) {
}
