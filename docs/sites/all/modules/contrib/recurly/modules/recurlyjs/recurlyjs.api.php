<?php
/**
 * @file
 * Hook documentation for Recurly JS module.
 */

/**
 * Alter a Recurly account object before it is saved during subscription signup.
 *
 * When signing up for a new subscription a new subscription record, which
 * contains the account, and other info, is created via the Recurly API.
 * Developers can use this hook to populate fields on the subscription and
 * nested objects before it's saved.
 *
 * Note, the billing_info is populated after this hook is called in order to
 * ensure the values are not altered as they need to match specific patterns
 * when using the Reucrly JS API.
 *
 * @param Recurly_Subscription $subscription
 *   The Recurly_Subscription object that is about to be created. Alterations to
 *   the subscription made in your code will be used when the account is created.
 * @param object $entity
 *   The entity that the Recurly subscription being created is associated with.
 *
 * @see recurlyjs_subscribe_form_submit()
 */
function hook_recurlyjs_subscription_alter($subscription, $entity) {
  // Fill in the email address field on the account object from the currently
  // logged in users's Drupal account.
  global $user;
  $subscription->account->email = $user->mail;
}

/**
 * React to new subscriptions being created.
 *
 * This hook is triggered when a user subscribes via the subscription form and
 * a new subscription is created in Recurly.
 *
 * @param Recurly_Subscription $subscription
 *   The Recurly_Subscription object for the newly created subscription.
 * @param object $entity
 *   The entity that the Recurly subscription being created is associated with.
 *
 * @see recurlyjs_subscribe_form_submit()
 */
function hook_recurlyjs_new_subscription($subscription, $entity) {
  // Perform an action like log the new subscription as an event in Google
  // Analytics ...
}
