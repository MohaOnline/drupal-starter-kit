<?php
/**
 * @file
 * API and hook documentation for the Recurly module.
 */

/**
 * Receive and process a ping from Recurly.com.
 *
 * Any module wishing to respond to a change in a subscription should implement
 * this hook. This hook is called after every ping from Recurly, including the
 * following events:
 *  - A new account has been created.
 *  - A new subscription has been created for an account.
 *  - A subscription is canceled/terminated.
 *  - A subscription has its plan changed.
 *  - A subscription has been invoiced.
 *
 * @param string $subdomain
 *   The Recurly subdomain for which this notification was received.
 * @param object $notification
 *   The XML Recurly notification. This is a raw SimpleXMl parsing of the
 *   notification. See https://docs.recurly.com/api/push-notifications.
 */
function hook_recurly_process_push_notification($subdomain, $notification) {
  // Reset the monthly limits upon account renewals.
  if ($notification->type === 'renewed_subscription_notification') {
    $account_code = $notification->account->account_code;
    if ($local_account = recurly_account_load(array('account_code' => $account_code), TRUE)) {
      // These notifications are SimpleXML objects rather than Recurly objects.
      $next_reset = new DateTime($notification->subscription->current_period_ends_at[0]);
      $next_reset->setTimezone(new DateTimeZone('UTC'));
      $next_reset = $next_reset->format('U');
      mymodule_reset_billing_limits($local_account->entity_id, $next_reset);
    }
    else {
      watchdog('recurly', 'Recurly received a Push notification, but was unable to locate the account in the local database. The push notification contained the following information: @notification', array('@notification' => print_r($notification, 1)), WATCHDOG_ALERT);
    }
  }

  // Upgrade/downgrade notifications.
  if ($notification->type === 'updated_subscription_notification' || $notification->type === 'new_subscription_notification') {
    $account_code = $notification->account->account_code;
    if ($local_account = recurly_account_load(array('account_code' => $account_code), TRUE)) {
      // Upgrade the account by assigning roles, changing fields, etc.
    }
    else {
      watchdog('recurly', 'Recurly received a Push notification, but was unable to locate the account in the local database. The push notification contained the following information: @notification', array('@notification' => print_r($notification, 1)), WATCHDOG_ALERT);
    }
  }
}

/**
 * Provides full URLs to various operations to manage Recurly subscriptions.
 *
 * This hook is implemented by modules that provide alternative ways to manage
 * a subscription. For example the Recurly Hosted Pages module returns URLs that
 * go to Recurly.com itself for managing a subscription, while the Recurly.js
 * module uses URLs on the Drupal site. If not using either Recurly Hosted Pages
 * or Recurly.js, other modules may want to provide custom URLs for managing
 * subscriptions.
 *
 * @param string $operation
 *   A string that indicates the URL should go to a page providing the following
 *   functionality:
 *   - select_plan: A listing of plans available for purchase where the user can
 *      select a plan for sign up.
 *   - change_plan: A page where a user can change a provided subscription's
 *      plan. If a "plan_code" is specified in $context, the page should confirm
 *      the subscription's switch to that plan.
 *   - cancel: A page where a user can cancel a provided subscription.
 *   - reactivate: A page where a user can reactivate a plan that has been
 *      canceled (will not renew) but not terminated entirely.
 *   - update_billing: A page where a user can update their billing
 *      information.
 *   - subscribe: A page where a user can sign up for a given plan code by
 *      entering their credit card information.
 * @param array $context
 *   An array of information that is provided to help assemable the $operation
 *   link. Properties may include:
 *   - entity: The associated entity for a subscription.
 *   - entity_type: The type (user, node, etc.) of the associated entity.
 *   - account_code: If the associated entity has a subscription, the Recurly
 *      account code.
 *   - currency: The currency code for a subscription.
 *   - subscription: The entire Recurly subscription object.
 *   - account: The entire Recurly account object.
 *   - plan_code: The plan code for an action, such as when changing a
 *      subscription plan or signing up for a new subscription.
 *   Not all properties will be available in all $operations.
 *
 * @return string
 *   A string containing the full URL to that particular action. If the module
 *   does not provide a page to an action, NULL should be returned.
 */
function hook_recurly_url_info($operation, $context) {
  // Only provide URLs for built-in page types.
  $recurly_entity_type = variable_get('recurly_entity_type', 'user');
  $parts = _recurly_url_entity_url_parts($context);
  if (empty($recurly_entity_type) || empty($parts)) {
    return;
  }

  switch ($operation) {
    case 'select_plan':
      return url($parts[0] . '/' . $parts[1] . '/subscription/signup');

    case 'change_plan':
      return url($parts[0] . '/' . $parts[1] . '/subscription/id/' . $context['subscription']->uuid . '/change' . (isset($context['plan_code']) ? '/' . $context['plan_code'] : ''));

    case 'cancel':
      return url($parts[0] . '/' . $parts[1] . '/subscription/id/' . $context['subscription']->uuid . '/cancel');

    case 'reactivate':
      return url($parts[0] . '/' . $parts[1] . '/subscription/id/' . $context['subscription']->uuid . '/reactivate');
  }
}
