<?php

/**
 * @file
 * Recurly front-end page callbacks.
 */

/**
 * Menu callback; Redirects a Recurly account code subscription management page.
 */
function recurly_subscription_redirect($account_code) {
  $account = recurly_account_load(array('account_code' => $account_code), TRUE);
  if ($account) {
    drupal_goto($account->entity_type . '/' . $account->entity_id . '/subscription');
  }
  else {
    return MENU_NOT_FOUND;
  }
}

/**
 * Menu callback; Display a summary of a Recurly account subscriptions.
 */
function recurly_subscription_list_page($entity_type, $entity) {
  $subscriptions = array();

  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Load the account information. This should already be cached by the access
  // check to this page by recurly_subscription_page_access().
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));

  // If the user does not have an account yet, send them to the signup page.
  if (empty($account)) {
    if ($url = recurly_url('select_plan', array('entity_type' => $entity_type, 'entity' => $entity))) {
      drupal_goto($url, array('external' => TRUE));
    }
    else {
      return MENU_NOT_FOUND;
    }
  }

  // Unlikely that we'd have more than 50 subscriptions, but you never know.
  $per_page = 50;
  $subscription_type = variable_get('recurly_subscription_display', 'live');
  $subscription_list = Recurly_SubscriptionList::getForAccount($account->account_code, array('per_page' => $per_page));
  $page_subscriptions = recurly_pager_results($subscription_list, $per_page);

  $subscriptions['subscriptions']['#attached']['css'] = array(
    drupal_get_path('module', 'recurly') . '/css/recurly.css',
  );

  $total_displayed = 0;
  foreach ($page_subscriptions as $subscription) {
    // Do not show subscriptions that are not active if only showing active.
    // TODO: Figure out if Recurly_SubscriptionList can only return active
    // subscriptions to begin with, rather than hiding them like this.
    if ($subscription->state === 'expired' && $subscription_type === 'live') {
      continue;
    }
    $total_displayed++;

    // Determine the state of this subscription.
    $states = recurly_subscription_get_states($subscription, $account);

    // Ensure that 'canceled' is listed before 'in_trial' in the list of
    // possible states, as this can influence what the summary looks like and
    // displaying the summary for a canceled account even when in trial is less
    // confusing for users who have cancelled their in trial account.
    if (in_array('canceled', $states)) {
      sort($states);
    }

    if (variable_get('recurly_subscription_max', '1') === '1') {
      $links = FALSE;
    }
    else {
      $links = recurly_subscription_links($entity_type, $entity, $subscription, $account, $states);
    }

    $plan = $subscription->plan;
    $add_ons = array();
    $total = 0;
    foreach ($subscription->subscription_add_ons as $add_on) {
      // Fully load the add on to get the name attribute.
      $full_add_on = Recurly_Addon::get($plan->plan_code, $add_on->add_on_code);
      $add_ons[$add_on->add_on_code] = array(
        'add_on_code' => $add_on->add_on_code,
        'name' => check_plain($full_add_on->name),
        'quantity' => check_plain($add_on->quantity),
        'cost' => recurly_format_currency($add_on->unit_amount_in_cents, $subscription->currency),
      );
      $total += $add_on->unit_amount_in_cents * $add_on->quantity;
    }
    $total += $subscription->unit_amount_in_cents * $subscription->quantity;

    $message = '';
    foreach ($states as $state) {
      $message = recurly_subscription_state_message($state, array('account' => $account, 'subscription' => $subscription));
      break;
    }

    $subscriptions['subscriptions'][$subscription->uuid] = array(
      '#theme' => 'recurly_subscription_summary',
      '#plan_code' => $plan->plan_code,
      '#plan_name' => check_plain($plan->name),
      '#state_array' => $states,
      '#cost' => recurly_format_currency($subscription->unit_amount_in_cents, $subscription->currency),
      '#quantity' => $subscription->quantity,
      '#add_ons' => $add_ons,
      '#start_date' => recurly_format_date($subscription->activated_at),
      '#end_date' => isset($subscription->expires_at) ? recurly_format_date($subscription->expires_at) : NULL,
      '#current_period_start' => recurly_format_date($subscription->current_period_started_at),
      '#current_period_ends_at' => recurly_format_date($subscription->current_period_ends_at),
      '#total' => recurly_format_currency($total, $subscription->currency),
      '#subscription_links' => $links ? theme('links', array(
        'links' => $links,
        'attributes' => array('class' => array('inline', 'links')))) : NULL,
      '#message' => $message,
      '#subscription' => $subscription,
      '#account' => $account,
    );
  }

  $subscriptions['pager'] = array(
    '#markup' => theme('pager'),
    '#access' => $subscription_list->count() > $per_page,
  );

  drupal_alter('recurly_subscription_list_page', $subscriptions);

  // If the user doesn't have any active subscriptions, redirect to the signup.
  if (count(element_children($subscriptions['subscriptions'])) === 0) {
    drupal_goto($entity_type . '/' . $id . '/subscription/signup');
  }

  return $subscriptions;
}

/**
 * Build a list of links to manage a subscription.
 */
function recurly_subscription_links($entity_type, $entity, $subscription, $account, $states) {
  // Generate the list of links for this subscription.
  $url_context = array(
    'entity_type' => $entity_type,
    'entity' => $entity,
    'subscription' => $subscription,
    'account' => $account,
  );

  $links = array();
  if ($subscription->state === 'active') {
    $links['change'] = array(
      'href' => recurly_url('change_plan', $url_context),
      'external' => TRUE,
      'title' => t('Change plan'),
    );
    $links['cancel'] = array(
      'href' => recurly_url('cancel', $url_context),
      'external' => TRUE,
      'title' => t('Cancel'),
      // Pass in the past_due flag to accurately calculate refunds.
      'query' => in_array('past_due', $states) ? array('past_due' => '1') : NULL,
    );
  }
  elseif ($subscription->state === 'canceled') {
    $links['reactivate'] = array(
      'href' => recurly_url('reactivate', $url_context),
      'external' => TRUE,
      'title' => t('Reactivate'),
    );
  }
  // Allow other modules to provide links, perhaps "suspend" for example.
  drupal_alter('recurly_subscription_links', $links);

  return $links;
}

/**
 * Returns a message for a subscription if the subscription state is not active.
 */
function recurly_subscription_state_message($state, $context) {
  switch ($state) {
    case 'active':
      return '';

    case 'closed':
      return t('This account is closed.');

    case 'in_trial':
      return t('Currently in trial period.');

    case 'past_due':
      $url = recurly_url('update_billing', $context);
      if ($url) {
        return t('This account is past due. Please <a href="!url">update your billing information</a>.', array('!url' => $url));
      }
      else {
        return t('This account is past due. Please contact an administrator to update your billing information.');
      }
    case 'canceled':
      $url = recurly_url('reactivate', $context);
      if ($url) {
        return t('This plan is canceled and will not renew.') . ' ' . t('You may  <a href="!url">reactivate the plan</a> to resume billing.', array('!url' => $url));
      }
      else {
        return t('This plan is canceled and will not renew.');
      }
    case 'expired':
      $url = recurly_url('select_plan', $context);
      if ($url) {
        return t('This plan has expired.') . ' ' . t('Please <a href="!url">purchase a new subscription</a>.', array('!url' => $url));
      }
      else {
        return t('This plan has expired.');
      }
    case 'pending_subscription':
      return t('This plan will be changed to @plan on @date.', array('@plan' => $context['subscription']->pending_subscription->plan->name, '@date' => recurly_format_date($context['subscription']->current_period_ends_at)));

    case 'future':
      return t('This plan has not started yet. Please contact support if you have any questions.');

    default:
      return '';

  }
}

/**
 * Get a list of all states in which a subscription exists currently.
 *
 * @param object $subscription
 *   A Recurly subscription object.
 * @param object $account
 *   A Recurly account object.
 *
 * @return array
 *   List of all states in which a subscription exists currently.
 */
function recurly_subscription_get_states($subscription, $account) {
  static $past_due = array();
  $states = array();

  // Determine if in a trial.
  if ($subscription->trial_started_at && $subscription->trial_ends_at) {
    $subscription->trial_started_at->setTimezone(new DateTimeZone('UTC'));
    $subscription->trial_ends_at->setTimezone(new DateTimeZone('UTC'));
    $start = $subscription->trial_started_at->format('U');
    $end = $subscription->trial_ends_at->format('U');
    if (REQUEST_TIME > $start && REQUEST_TIME < $end) {
      $states[] = 'in_trial';
    }
  }

  // Determine if non-renewing.
  if (!empty($subscription->total_billing_cycles)) {
    $states[] = 'non_renewing';
  }

  // Retrieve past due subscriptions.
  if (!isset($past_due[$account->account_code])) {
    $subscriptions = Recurly_SubscriptionList::getForAccount($account->account_code, array('state' => 'past_due'));
    $past_due[$account->account_code] = array();
    foreach ($subscriptions as $past_due_subscription) {
      $past_due[$account->account_code][] = $past_due_subscription->uuid;
    }
  }
  if (in_array($subscription->uuid, $past_due[$account->account_code])) {
    $states[] = 'past_due';
  }

  // Subscriptions that have pending changes.
  if (!empty($subscription->pending_subscription)) {
    $states[] = 'pending_subscription';
  }

  $states[] = $subscription->state;
  return $states;
}

/**
 * Menu callback; Show a list of available plans to which a user may subscribe.
 *
 * This menu callback is used both for new subscriptions and for updating
 * existing subscriptions.
 *
 * @param string $entity_type
 *   The type of the entity whose subscription is being changed.
 * @param object $entity
 *   The entity whose subscription is being changed.
 * @param string $currency
 *   If this is a new subscription, the currency to be used.
 * @param int $subscription_id
 *   The UUID of the current subscription if changing the plan on an existing
 *   subscription.
 *
 * @return string
 *   If fail to initialize specified account settings returns error message,
 *   else return the list of available plans.
 */
function recurly_subscription_plan_select($entity_type, $entity, $currency = NULL, $subscription_id = NULL) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // If loading an existing subscription.
  if ($subscription_id) {
    if ($subscription_id === 'latest') {
      list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
      $local_account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id), TRUE);
      $subscriptions = recurly_account_get_subscriptions($local_account->account_code, 'active');
      $subscription = reset($subscriptions);
      $subscription_id = $subscription->uuid;
      $currency = $subscription->plan->currency;
      $mode = 'change';
    }
    else {
      try {
        $subscription = Recurly_Subscription::get($subscription_id);
        $subscriptions[$subscription->uuid] = $subscription;
        $currency = $subscription->plan->currency;
        $mode = 'change';
      }
      catch (Recurly_NotFoundError $e) {
        drupal_set_message(t('Subscription not found'));
        return MENU_NOT_FOUND;
      }
    }
  }
  // If signing up to a new subscription, ensure the user doesn't have a plan.
  else {
    $subscriptions = array();
    $currency = isset($currency) ? $currency : variable_get('recurly_default_currency', 'USD');
    $mode = 'signup';
    list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
    $account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));
    if ($account) {
      $subscriptions = recurly_account_get_subscriptions($account->account_code, 'active');
    }
  }

  // Make the list of subscriptions based on plan keys, rather than uuid.
  $plan_subscriptions = array();
  foreach ($subscriptions as $subscription) {
    $plan_subscriptions[$subscription->plan->plan_code] = $subscription;
  }

  $all_plans = recurly_subscription_plans();
  $enabled_plan_keys = variable_get('recurly_subscription_plans', array());
  $enabled_plans = array();
  foreach ($enabled_plan_keys as $plan_code => $enabled) {
    foreach ($all_plans as $plan) {
      if ($enabled && $plan_code == $plan->plan_code) {
        $enabled_plans[$plan_code] = $plan;
      }
    }
  }
  return theme(
    array(
      'recurly_subscription_plan_select__' . $mode,
      'recurly_subscription_plan_select',
    ),
    array(
      'plans' => $enabled_plans,
      'entity_type' => $entity_type,
      'entity' => $entity,
      'currency' => $currency,
      'mode' => $mode,
      'subscriptions' => $plan_subscriptions,
      'subscription_id' => $subscription_id,
    ));
}

/**
 * Shared preprocess function for the presentation of the signup & change page.
 */
function template_preprocess_recurly_subscription_plan_select(&$variables) {
  $plans = $variables['plans'];
  $currency = $variables['currency'];
  $entity_type = $variables['entity_type'];
  $entity = $variables['entity'];
  $subscriptions = $variables['subscriptions'];
  $subscription_id = $variables['subscription_id'];

  $current_subscription = NULL;
  foreach ($subscriptions as $subscription) {
    if ($subscription->uuid === $subscription_id) {
      $current_subscription = $subscription;
      break;
    }
  }

  // If currency is undefined, use the subscription currency.
  if ($current_subscription && empty($currency)) {
    $currency = $current_subscription->currency;
    $variables['currency'] = $currency;
  }

  // Prepare an easy to loop-through list of subscriptions.
  $variables['filtered_plans'] = array();
  foreach ($plans as $plan_code => $plan) {
    $setup_fee_amount = NULL;
    foreach ($plan->setup_fee_in_cents as $setup_currency) {
      if ($setup_currency->currencyCode === $currency) {
        $setup_fee_amount = recurly_format_currency($setup_currency->amount_in_cents, $setup_currency->currencyCode, TRUE);
        break;
      }
    }
    $unit_amount = NULL;
    foreach ($plan->unit_amount_in_cents as $unit_currency) {
      if ($unit_currency->currencyCode === $currency) {
        $unit_amount = recurly_format_currency($unit_currency->amount_in_cents, $unit_currency->currencyCode, TRUE);
        break;
      }
    }
    list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
    $variables['filtered_plans'][$plan_code] = array(
      'plan_code' => check_plain($plan_code),
      'name' => check_plain($plan->name),
      'description' => check_plain($plan->description),
      'setup_fee' => $setup_fee_amount,
      'amount' => $unit_amount,
      'plan_interval' => recurly_format_price_interval($unit_amount, $plan->plan_interval_length, $plan->plan_interval_unit, TRUE),
      'trial_interval' => $plan->trial_interval_length ? recurly_format_price_interval(NULL, $plan->trial_interval_length, $plan->trial_interval_unit, TRUE) : NULL,
      'signup_url' => recurly_url('subscribe', array(
        'entity_type' => $entity_type,
        'entity' => $entity,
        'plan_code' => $plan_code,
        'currency' => $currency,
      )),
      'change_url' => $current_subscription ? recurly_url('change_plan',
        array(
          'entity_type' => $entity_type,
          'entity' => $entity,
          'subscription' => $current_subscription,
          'plan_code' => $plan_code,
        )) : NULL,
      'selected' => FALSE,
    );

    // If we have a pending subscription, make that its shown as selected rather
    // than the current active subscription. This should allow users to switch
    // back to a previous plan after making a pending switch to another one.
    foreach ($subscriptions as $subscription) {
      if (!empty($subscription->pending_subscription)) {
        if ($subscription->pending_subscription->plan->plan_code === $plan_code) {
          $variables['filtered_plans'][$plan_code]['selected'] = TRUE;
        }
      }
      elseif ($subscription->plan->plan_code === $plan_code) {
        $variables['filtered_plans'][$plan_code]['selected'] = TRUE;
      }
    }
  }

  // Check if this is an account that is creating a new subscription, either
  // because there are no subscriptions yet, or only expired subscriptions.
  $variables['no_active_subscriptions'] = FALSE;
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));
  if ($account) {
    $variables['no_active_subscriptions'] = empty($subscriptions);
  }
}

/**
 * Page callback; Prepare call to recurly_subscription_plan_change_confirm().
 */
function recurly_subscription_plan_change_page($entity_type, $entity, $subscription_id, $new_plan_code) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Load the subscription.
  try {
    $subscription = Recurly_Subscription::get($subscription_id);
  }
  catch (Recurly_NotFoundError $e) {
    drupal_set_message(t('Subscription not found.'));
    return MENU_NOT_FOUND;
  }

  // Load the old plan.
  try {
    $previous_plan = Recurly_Plan::get($subscription->plan->plan_code);
  }
  catch (Recurly_NotFoundError $e) {
    drupal_set_message(t('Plan code "@plan" not found.', array('@plan' => $subscription->plan->plan_code)));
    return MENU_NOT_FOUND;
  }

  // Load the new plan.
  try {
    $new_plan = Recurly_Plan::get($new_plan_code);
  }
  catch (Recurly_NotFoundError $e) {
    drupal_set_message(t('Plan code "@plan" not found.', array('@plan' => $new_plan_code)));
    return MENU_NOT_FOUND;
  }

  return drupal_get_form('recurly_subscription_plan_change_confirm', $entity_type, $entity, $subscription, $previous_plan, $new_plan);
}

/**
 * Form callback; Display a confirmation form for changing a subscription plan.
 */
function recurly_subscription_plan_change_confirm($form, $form_state, $entity_type, $entity, $subscription, $previous_plan, $new_plan) {
  // Note that currently Recurly does not have the ability to change the
  // subscription currency after it has started.
  // See http://docs.recurly.com/currencies
  $currency = $subscription->currency;
  $previous_amount = $previous_plan->unit_amount_in_cents[$currency]->amount_in_cents;
  $new_amount = $new_plan->unit_amount_in_cents[$currency]->amount_in_cents;

  drupal_set_title(t('Confirm switch to @plan?', array('@plan' => $new_plan->name)), FALSE);

  $form['#entity_type'] = $entity_type;
  $form['#entity'] = $entity;
  $form['#subscription'] = $subscription;
  $form['#previous_plan'] = $previous_plan;
  $form['#new_plan'] = $new_plan;

  if ($new_amount >= $previous_amount) {
    $timeframe = variable_get('recurly_subscription_upgrade_timeframe', 'now');
  }
  else {
    $timeframe = variable_get('recurly_subscription_downgrade_timeframe', 'renewal');
  }
  $form['timeframe'] = array(
    '#type' => 'radios',
    '#title' => t('Changes take effect'),
    '#options' => array(
      'now' => t('Immediately'),
      'renewal' => t('On next renewal'),
    ),
    '#description' => t('If changes take effect immediately, the price difference will either result in a credit applied when the subscription renews or will trigger a prorated charge within the hour.'),
    '#default_value' => $timeframe,
    '#access' => user_access('administer recurly'),
  );

  // TODO: We could potentially calculate the charge/credit amount here, but
  // math gets messy when switching between plans with different lengths.
  if ($timeframe === 'now') {
    $timeframe_message = '<p>' . t('The new plan will take effect immediately and a prorated charge (or credit) will be applied to this account.') . '</p>';
  }
  else {
    $timeframe_message = '<p>' . t('The new plan will take effect on the next billing cycle.') . '</p>';
  }
  $form['timeframe_help'] = array(
    '#markup' => $timeframe_message,
    '#access' => !user_access('administer recurly'),
  );
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change plan'),
  );

  // Add a cancel option to the confirmation form.
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $entity_type . '/' . $id . '/subscription/change',
  );
  return $form;
}

/**
 * Submit handler for recurly_subscription_plan_change_confirm().
 */
function recurly_subscription_plan_change_confirm_submit($form, &$form_state) {
  $entity = $form['#entity'];
  $entity_type = $form['#entity_type'];
  $subscription = $form['#subscription'];
  $new_plan = $form['#new_plan'];
  $timeframe = $form_state['values']['timeframe'];
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Update the plan.
  $subscription->plan_code = $new_plan->plan_code;
  try {
    if ($timeframe === 'now') {
      $subscription->updateImmediately();
    }
    else {
      $subscription->updateAtRenewal();
    }
  }
  catch (Recurly_Error $e) {
    drupal_set_message(t('The plan could not be updated because the billing service encountered an error.'));
    return;
  }

  $message = t('Plan changed to @plan!', array('@plan' => $new_plan->name));
  if ($timeframe !== 'now') {
    $message .= ' ' . t('Changes will become active starting <strong>@date</strong>.', array('@date' => recurly_format_date($subscription->current_period_ends_at)));
  }
  drupal_set_message($message);
  $form_state['redirect'] = $entity_type . '/' . $id . '/subscription';
}

/**
 * Menu callback; Present confirmation form for canceling an account.
 */
function recurly_subscription_cancel_page($entity_type, $entity, $subscription_id) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Load the subscription.
  if ($subscription_id === 'latest') {
    list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
    $local_account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id), TRUE);
    $subscriptions = recurly_account_get_subscriptions($local_account->account_code, 'active');
    $subscription = reset($subscriptions);
  }
  else {
    try {
      $subscription = Recurly_Subscription::get($subscription_id);
    }
    catch (Recurly_NotFoundError $e) {
      drupal_set_message(t('Subscription not found'));
      return MENU_NOT_FOUND;
    }
  }

  return drupal_get_form('recurly_subscription_cancel_confirm', $entity_type, $entity, $subscription);
}

/**
 * Form callback; Display a confirmation form for changing a subscription plan.
 */
function recurly_subscription_cancel_confirm($form, $form_state, $entity_type, $entity, $subscription) {
  drupal_set_title(t('Really cancel @plan?', array('@plan' => $subscription->plan->name)), FALSE);

  $form['#entity_type'] = $entity_type;
  $form['#entity'] = $entity;
  $form['#subscription'] = $subscription;

  $past_due = isset($_GET['past_due']) && $_GET['past_due'] === '1';
  $admin_access = user_access('administer recurly');
  $in_trial = recurly_subscription_in_trial($subscription);

  // If in a trial, only cancel the account instead of terminating.
  if ($in_trial && !$admin_access) {
    $cancel_behavior = 'cancel';
  }
  else {
    $cancel_behavior = variable_get('recurly_subscription_cancel_behavior', 'cancel');
  }

  $form['cancel'] = array(
    '#access' => $admin_access || $cancel_behavior === 'cancel',
  );
  $form['cancel']['description'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('Canceling a subscription will cause it not to renew. If you cancel the subscription, it will continue until <strong>@date</strong>. On that date, the subscription will expire and not be invoiced again. The subscription can be reactivated before it expires.', array('@date' => recurly_format_date($subscription->current_period_ends_at)))  . '</p>',
  );
  $form['cancel']['actions'] = array(
    '#type' => 'actions',
  );
  $form['cancel']['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel at Renewal'),
  );

  $form['terminate'] = array(
    '#access' => $admin_access || $cancel_behavior !== 'cancel',
  );
  $form['terminate']['refund_amount'] = array(
    '#type' => 'radios',
    '#title' => t('Refund amount'),
    '#options' => array(
      'terminate_none' => t('@amount - None', array('@amount' => recurly_format_currency(0, $subscription->currency))),
    ),
    '#default_value' => $cancel_behavior === 'cancel' ? NULL : $cancel_behavior,
    '#required' => TRUE,
    '#weight' => 1,
    '#access' => $admin_access,
  );

  if (!$past_due && $prorated_amount = recurly_subscription_calculate_refund($subscription, 'prorated')) {
    $form['terminate']['refund_amount']['#options']['terminate_prorated'] = t('@amount - Prorated', array('@amount' => recurly_format_currency($prorated_amount, $subscription->currency)));
  }
  if (!$past_due && $full_amount = recurly_subscription_calculate_refund($subscription, 'full')) {
    $form['terminate']['refund_amount']['#options']['terminate_full'] = t('@amount - Full', array('@amount' => recurly_format_currency($full_amount, $subscription->currency)));
  }

  $form['terminate']['admin_description'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('If you would like the subscription to end immediately, you may terminate the subscription now. Optionally, you may also issue a refund for the time remaining (prorated) or for the full amount. New subscriptions to this account will need to sign up for a new plan.') . '</p>',
    '#access' => $admin_access,
  );

  // Use a more friendly description of the process for non-administrators.
  $friendly_description = t('This subscription will be ended immediately. If you would like to subscribe again, you will need to start a new subscription.');
  if ($past_due) {
    $friendly_description .= '';
  }
  elseif ($cancel_behavior === 'terminate_prorated') {
    $friendly_description .= ' ' . t('A refund of @amount will be credited to your account.', array('@amount' => recurly_format_currency($prorated_amount, $subscription->currency)));
  }
  elseif ($cancel_behavior === 'terminate_full') {
    $friendly_description .= ' ' . t('A refund of @amount will be credited to your account.', array('@amount' => recurly_format_currency($full_amount, $subscription->currency)));
  }
  $form['terminate']['user_description'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . $friendly_description . '</p>',
    '#access' => !$admin_access,
  );
  $form['terminate']['actions'] = array(
    '#type' => 'actions',
    '#weight' => 2,
  );
  $form['terminate']['actions']['terminate'] = array(
    '#type' => 'submit',
    '#value' => $admin_access ? t('Terminate Immediately') : t('Cancel Plan'),
  );

  // Add a cancel option to the confirmation form.
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $entity_type . '/' . $id . '/subscription',
  );

  return $form;
}

/**
 * Submit handler for recurly_subscription_plan_change_confirm().
 */
function recurly_subscription_cancel_confirm_submit($form, &$form_state) {
  $entity = $form['#entity'];
  $entity_type = $form['#entity_type'];
  $subscription = $form['#subscription'];
  $clicked_button = $form_state['values']['op'];
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  if ($form['cancel']['actions']['cancel']['#value'] === $clicked_button) {
    try {
      $subscription->cancel();
      drupal_set_message(t('Plan @plan canceled! It will expire on @date.', array('@plan' => $subscription->plan->name, '@date' => recurly_format_date($subscription->current_period_ends_at))));
      $form_state['redirect'] = $entity_type . '/' . $id . '/subscription';
    }
    catch (Recurly_Error $e) {
      drupal_set_message(t('The plan could not be canceled because the billing service encountered an error.'), 'error');
      return;
    }
  }
  elseif ($form['terminate']['actions']['terminate']['#value'] === $clicked_button) {
    try {
      switch ($form_state['values']['refund_amount']) {
        case 'terminate_none':
          $subscription->terminateWithoutRefund();
          break;

        case 'terminate_prorated':
          $subscription->terminateAndPartialRefund();
          break;

        case 'terminate_full':
          $subscription->terminateAndRefund();
          break;
      }
      drupal_set_message(t('Plan @plan terminated!', array('@plan' => $subscription->plan->name)));
      $form_state['redirect'] = $entity_type . '/' . $id . '/subscription';
    }
    catch (Recurly_Error $e) {
      drupal_set_message(t('The plan could not be terminated because the billing service encountered an error: "@message"', array('@message' => $e->getMessage())), 'error');
      return;
    }
  }

}

/**
 * Preprocess variables for recurly-subscription-cancel-confirm.tpl.php.
 */
function template_preprocess_recurly_subscription_cancel_confirm(&$variables) {
  $variables['subscription'] = $variables['form']['#subscription'];
  $variables['past_due'] = isset($_GET['past_due']) && $_GET['past_due'] === '1';
}


/**
 * Menu callback; Reactivate a subscription immediately and redirect.
 */
function recurly_subscription_reactivate($entity_type, $entity, $subscription_id) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Load the subscription.
  if ($subscription_id === 'latest') {
    list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
    $local_account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id), TRUE);
    $subscriptions = recurly_account_get_subscriptions($local_account->account_code, 'active');
    $subscription = reset($subscriptions);
  }
  else {
    try {
      $subscription = Recurly_Subscription::get($subscription_id);
    }
    catch (Recurly_NotFoundError $e) {
      drupal_set_message(t('Subscription not found'));
      return MENU_NOT_FOUND;
    }
  }

  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  try {
    $subscription->reactivate();
    drupal_set_message(t('Plan @plan reactivated! Normal billing will resume on @date.', array('@plan' => $subscription->plan->name, '@date' => recurly_format_date($subscription->current_period_ends_at))));
  }
  catch (Recurly_Error $e) {
    drupal_set_message(t('The plan could not be reactivated because the billing service encountered an error.'));
    return;
  }

  drupal_goto($entity_type . '/' . $id . '/subscription');
}

/**
 * Menu callback; Display a list of all invoices for a user.
 */
function recurly_invoices_page($entity_type, $entity) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Load the account information. This should already be cached by the access
  // check to this page by recurly_subscription_page_access().
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));

  $per_page = 20;
  $invoice_list = Recurly_InvoiceList::getForAccount($account->account_code, array('per_page' => $per_page));
  $invoices = recurly_pager_results($invoice_list, $per_page);

  return theme('recurly_invoice_list',
    array(
      'invoices' => $invoices,
      'entity_type' => $entity_type,
      'entity' => $entity,
      'per_page' => $per_page,
      'total' => $invoice_list->count(),
    ));
}

/**
 * Preprocess variables for the recurly-invoice-list.tpl.php file.
 */
function template_preprocess_recurly_invoice_list(&$variables) {
  $invoices = $variables['invoices'];
  $entity_type = $variables['entity_type'];
  $entity = $variables['entity'];
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  $header = array(t('Number'), t('Date'), t('Total'));
  $rows = array();
  foreach ($invoices as $invoice) {
    $status = '';
    if ($invoice->state === 'past_due') {
      $status = ' (' . t('Past due') . ')';
    }
    elseif ($invoice->state === 'failed') {
      $status = ' (' . t('Failed') . ')';
    }

    $row = array();
    $row[] = l($invoice->invoice_number . $status, $entity_type . '/' . $id . '/subscription/invoices/' . $invoice->invoice_number);
    $row[] = recurly_format_date($invoice->created_at);
    $row[] = recurly_format_currency($invoice->total_in_cents, $invoice->currency);
    $rows[] = array(
      'data' => $row,
      'class' => array(check_plain($invoice->state)),
    );
  }

  $variables['table'] = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('class' => array('invoice-list')),
    'sticky' => FALSE,
  );
}

/**
 * Menu callback; Display an individual invoice.
 */
function recurly_invoice_page($entity_type, $entity, $invoice_number) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Load the account information. This should already be cached by the access
  // check to this page by recurly_subscription_page_access().
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));

  // Load the invoice.
  try {
    $invoice = Recurly_Invoice::get($invoice_number);
  }
  catch (Recurly_NotFoundError $e) {
    drupal_set_message(t('Invoice not found'));
    return MENU_NOT_FOUND;
  }

  // Load the invoice account.
  $invoice_account = $invoice->account->get();

  // Ensure that the user account is the same as the invoice account.
  if (empty($account) || $invoice_account->account_code !== $account->account_code) {
    return MENU_NOT_FOUND;
  }

  drupal_set_title(t('Invoice #@number', array('@number' => $invoice->invoice_number)));
  return theme('recurly_invoice',
    array(
      'invoice' => $invoice,
      'invoice_account' => $invoice_account,
      'entity_type' => $entity_type,
      'entity' => $entity,
    ));
}

/**
 * Menu callback; Display form for redeeming a coupon.
 */
function recurly_subscription_redeem_coupon($form, &$form_state, $entity_type, $entity) {
  $form['#entity_type'] = $entity_type;
  $form['#entity'] = $entity;
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Check the user's account for any current coupons.
  $account = recurly_account_load(array('entity_type' => $form['#entity_type'], 'entity_id' => $id));
  $confirming_replacement_coupon = isset($form_state['confirm']) && isset($form_state['coupon']) && isset($form_state['existing_coupon']);
  $form_state['account'] = $account;

  // The output of this form varies based on if the user has an existing coupon,
  // and if they need to confirm replacing their coupon with a new one.
  if ($confirming_replacement_coupon) {
    $form_state['confirmed'] = TRUE;
    $help = '<p>' . t('Your account already has a coupon that will be applied to your next invoice. Are you sure you want to replace your existing coupon "!old_coupon" with "!new_coupon"? You may not be able to use your previous coupon again.',
      array(
        '!old_coupon' => recurly_format_coupon($form_state['existing_coupon'], $form_state['existing_redemption']->currency),
        '!new_coupon' => recurly_format_coupon($form_state['coupon'], $form_state['values']['coupon_currency']))) . '</p>';
  }
  elseif ($account->redemption) {
    $form_state['existing_redemption'] = $account->redemption->get();
    $form_state['existing_coupon'] = $form_state['existing_redemption']->coupon->get();

    $help = '<p>' . t('Your next invoice will have the following coupon applied:') . ' <strong>' . recurly_format_coupon($form_state['existing_coupon'], $form_state['existing_redemption']->currency) . '</strong></p>';
    $help .= '<p>' . t('Please note that only one coupon can be redeemed per invoice.') . '</p>';
  }
  else {
    $help = '<p>' . t('Enter a coupon code below and it will be applied to your next invoice.') . '</p>';
  }

  $form['help'] = array(
    '#markup' => $help,
  );
  $form['coupon_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon code'),
    '#required' => TRUE,
    '#default_value' => isset($form_state['coupon']) ? $form_state['coupon']->coupon_code : '',
    '#access' => !$confirming_replacement_coupon,
    '#size' => 20,
  );
  $form['coupon_currency'] = array(
    '#type' => 'select',
    '#title' => t('Coupon currency'),
    '#options' => drupal_map_assoc(array_keys(recurly_currency_list())),
    '#default_value' => variable_get('recurly_default_currency', 'USD'),
    '#description' => t('If your coupon specifies a currency, select it here. Not all coupons work in all currencies.'),
    '#access' => !$confirming_replacement_coupon,
  );
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $confirming_replacement_coupon ? t('Replace previous coupon') : t('Redeem coupon'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'markup',
    '#markup' => '<a href="' . recurly_url('redeem_coupon', array('entity_type' => $entity_type, 'entity' => $entity)) . '">' . t('Cancel') . '</a>',
    '#access' => $confirming_replacement_coupon,
  );
  return $form;
}

/**
 * Validation handler for recurly_subscription_redeem_coupon().
 */
function recurly_subscription_redeem_coupon_validate($form, &$form_state) {
  // Query Recurly to make sure this is a valid coupon code.
  try {
    $coupon = Recurly_Coupon::get($form_state['values']['coupon_code']);
    $form_state['coupon'] = $coupon;
  }
  catch (Recurly_NotFoundError $e) {
    form_set_error('coupon_code', t('The coupon code you have entered is not valid.'));
    return;
  }

  // Check that the coupon is available in the specified currency.
  if ($form_state['coupon'] && !in_array($form_state['coupon']->discount_type, array('percent', 'free_trial'))) {
    if (!$form_state['coupon']->discount_in_cents->offsetExists($form_state['values']['coupon_currency'])) {
      form_set_error('coupon_currency', t('The coupon code you have entered is not valid in @currency currency.', array('@currency' => $form_state['values']['coupon_currency'])));
      return;
    }
  }
}

/**
 * Submit handler for recurly_subscription_redeem_coupon().
 */
function recurly_subscription_redeem_coupon_submit($form, &$form_state) {
  $coupon = $form_state['coupon'];
  $account = $form_state['account'];

  if ($account && $coupon && $account->redemption) {
    // If the user already has a coupon, rebuild the form and ask confirmation.
    if (empty($form_state['confirmed'])) {
      drupal_set_message('You already have an active coupon, are you sure you want to replace it?', 'warning');
      $form_state['confirm'] = TRUE;
      $form_state['rebuild'] = TRUE;
      return;
    }
    // If confirmed, delete the existing coupon before redeeming the new one.
    else {
      $form_state['existing_redemption']->delete();
    }
  }

  // Now redeem the new coupon.
  $response = $coupon->redeemCoupon($account->account_code, $form_state['values']['coupon_currency']);

  // If the response is NULL that means for one reason or another the coupon
  // could not be applied. This is most likely because the code has already
  // reached the maximum number of redemptions or has expired.
  if (is_null($response)) {
    drupal_set_message(t('Unable to redeem the coupon @code, the coupon may no longer be valid.', array('@code' => $coupon->coupon_code)), 'error');
  }
  else {
    drupal_set_message(t('The coupon !coupon has been applied to your account and will be redeemed the next time your subscription renews.', array('!coupon' => recurly_format_coupon($coupon, $form_state['values']['coupon_currency']))));
  }
}

/**
 * Preprocess variables for invoice pages.
 */
function template_preprocess_recurly_invoice(&$variables) {
  $invoice = $variables['invoice'];
  $invoice_account = $variables['invoice_account'];
  $billing_info = isset($invoice->billing_info) ? $invoice->billing_info->get() : NULL;
  list($id, $vid, $bundle) = entity_extract_ids($variables['entity_type'], $variables['entity']);

  $due_amount = $invoice->state !== 'collected' ? $invoice->total_in_cents : 0;
  $paid_amount = $invoice->state === 'collected' ? $invoice->total_in_cents : 0;
  $variables += array(
    'invoice_date' => recurly_format_date($invoice->created_at),
    'pdf_path' => $variables['entity_type'] . '/' . $id . '/subscription/invoices/' . $invoice->invoice_number . '/pdf',
    'subtotal' => recurly_format_currency($invoice->subtotal_in_cents, $invoice->currency),
    'total' => recurly_format_currency($invoice->total_in_cents, $invoice->currency),
    'due' => recurly_format_currency($due_amount, $invoice->currency),
    'paid' => recurly_format_currency($paid_amount, $invoice->currency),
    'billing_info' => isset($billing_info),
    'line_items' => array(),
    'transactions' => array(),
  );
  if ($billing_info) {
    $variables += array(
      'first_name' => check_plain($billing_info->first_name),
      'last_name' => check_plain($billing_info->last_name),
      'address1' => check_plain($billing_info->address1),
      'address2' => isset($billing_info->address2) ? check_plain($billing_info->address2) : NULL,
      'city' => check_plain($billing_info->city),
      'state' => check_plain($billing_info->state),
      'zip' => check_plain($billing_info->zip),
      'country' => check_plain($billing_info->country),
    );
  }
  foreach ($invoice->line_items as $line_item) {
    $variables['line_items'][$line_item->uuid] = array(
      'start_date' => recurly_format_date($line_item->start_date),
      'end_date' => recurly_format_date($line_item->end_date),
      'description' => check_plain($line_item->description),
      'amount' => recurly_format_currency($line_item->total_in_cents, $line_item->currency),
    );
  }
  $transaction_total = 0;
  foreach ($invoice->transactions as $transaction) {
    $variables['transactions'][$transaction->uuid] = array(
      'date' => recurly_format_date($transaction->created_at),
      'description' => recurly_format_transaction_status($transaction->status),
      'amount' => recurly_format_currency($transaction->amount_in_cents, $transaction->currency),
    );
    if ($transaction->status == 'success') {
      $transaction_total += $transaction->amount_in_cents;
    }
    else {
      $variables['transactions'][$transaction->uuid]['amount'] = '(' . $variables['transactions'][$transaction->uuid]['amount'] . ')';
    }
  }
  $variables['transactions_total'] = recurly_format_currency($transaction_total, $invoice->currency);

  if ($invoice->state !== 'collected') {
    $url = recurly_url('update_billing', array('account' => $invoice_account));
    if ($url) {
      $variables['error_message'] = t('This invoice is past due! Please <a href="!url">update your billing information</a>.', array('!url' => $url));
    }
    else {
      $variables['error_message'] = t('This invoice is past due! Please contact an administrator to update your billing information.');
    }
  }
}

/**
 * Deliver an invoice PDF file from Recurly.com.
 */
function recurly_invoice_pdf($entity_type, $entity, $invoice_number) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Load the account information. This should already be cached by the access
  // check to this page by recurly_subscription_page_access().
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));

  // Load the invoice.
  try {
    $invoice = Recurly_Invoice::get($invoice_number);
    $pdf = Recurly_Invoice::getInvoicePdf($invoice_number);
  }
  catch (Recurly_NotFoundError $e) {
    drupal_set_message(t('Invoice not found'));
    return MENU_NOT_FOUND;
  }

  // Load the invoice account.
  $invoice_account = $invoice->account->get();

  // Ensure that the user account is the same as the invoice account.
  if (empty($account) || $invoice_account->account_code !== $account->account_code) {
    return MENU_NOT_FOUND;
  }

  if (!empty($pdf)) {
    if (headers_sent()) {
      die("Unable to stream pdf: headers already sent");
    }

    drupal_add_http_header('Content-Type', 'application/pdf', TRUE);
    drupal_add_http_header('Content-Disposition', 'inline; filename="' . $invoice_number . '.pdf"', TRUE);
    drupal_send_headers();

    // I guess below is not necessary plus filesize was not working anyway?!
    // drupal_add_http_header('Content-Transfer-Encoding', 'binary', TRUE);
    // drupal_add_http_header('Content-Length', filesize($pdf), TRUE);
    print $pdf;
    drupal_exit();
  }
}

/**
 * Utility function to retrieve a specific page of results from a Recurly_Pager.
 *
 * @param object $pager_object
 *   Any object that extends a Recurly_Pager object, such as a
 *   Recurly_InvoiceList, Recurly_SubscriptionList, or Recurly_TransactionList.
 * @param int $per_page
 *   The number of items to display per page.
 * @param int $page_num
 *   The desired page number to display. Usually automatically determined from
 *   the URL.
 *
 * @return array
 *   Specific page of results from a Recurly_Pager.
 */
function recurly_pager_results($pager_object, $per_page, $page_num = NULL) {
  if (!isset($page_num)) {
    $page_num = isset($_GET['page']) ? (int) $_GET['page'] : 0;
  }

  // Fast forward the list to the current page.
  $start = $page_num * $per_page;
  for ($n = 0; $n < $start; $n++) {
    $pager_object->next();
  }

  // Populate $page_results with the current page.
  $total = $pager_object->count();
  $page_end = min($start + $per_page, $total);
  $page_results = array();
  for ($n = $start; $n < $page_end; $n++) {
    $item = $pager_object->current();
    $page_results[$item->uuid] = $item;
    $pager_object->next();
  }

  pager_default_initialize($total, $per_page);

  return $page_results;
}
