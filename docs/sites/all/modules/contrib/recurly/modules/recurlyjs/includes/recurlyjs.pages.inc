<?php

/**
 * @file
 * Callbacks for the Recurly.js module.
 */

/**
 * Form callback; Modifies the Recurly form at admin/config/services/recurly.
 */
function _recurlyjs_form_recurly_settings_form_alter(&$form, &$form_state) {
  $form['recurlyjs'] = array(
    '#title' => t('Recurly.js settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );
  $form['recurlyjs']['recurlyjs_address_requirement'] = array(
    '#title' => t('Address requirement level'),
    '#type' => 'select',
    '#options' => array(
      'full' => t('Full'),
      'zipstreet' => t('Zip code and street'),
      'zip' => t('Zip code only'),
      'none' => t('None'),
    ),
    '#default_value' => variable_get('recurlyjs_address_requirement', 'full'),
    '#description' => t('Choose the level of address information required. Collecting more address information reduces the probability of fraudulent accounts. This setting should match the address requirements as configured on !link in "Site Settings".', array(
      '!link' => variable_get('recurly_subdomain', '') ? l(t('your Recurly account'),
      recurly_hosted_url('configuration/edit')) : t('your Recurly account'),
    )),
  );
  $form['recurlyjs']['recurlyjs_enable_add_ons'] = array(
    '#title' => t('Enable Add-ons'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('recurlyjs_enable_add_ons', 1),
    '#description' => t('Add-ons are options that may be displayed in addition to the base subscription. If a subscription does not have add-ons, nothing additional will be displayed.'),
  );
  $form['recurlyjs']['recurlyjs_enable_coupons'] = array(
    '#title' => t('Enable coupons'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('recurlyjs_enable_coupons', 0),
    '#description' => t('Display the <em>Coupon Code</em> field on Recurly.js subscription forms.'),
  );
  $form['recurlyjs']['recurlyjs_hide_vat_number'] = array(
    '#title' => t('Hide VAT number input box from checkout page.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('recurlyjs_hide_vat_number', 0),
    '#description' => t('VAT numbers are mostly used in the EU and not often in the US.'),
  );
  $form['recurlyjs']['recurlyjs_accept_paypal'] = array(
    '#title' => t('Enable PayPal'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('recurlyjs_accept_paypal', FALSE),
    '#description' => t('Show an option to pay with PayPal. This requires a PayPal Business account with Referential Transactions enabled. <a href="https://docs.recurly.com/payment-gateways/paypal-payments">See the Recurly Paypal documentation</a>.'),
  );

  $jquery_requirement = FALSE;
  if (module_exists('jquery_update')) {
    $jquery_version = variable_get('jquery_update_jquery_version', '1.10');
    if (version_compare('1.7', $jquery_version) < 0) {
      $jquery_requirement = TRUE;
    }
  }

  if (!$jquery_requirement) {
    $form['recurlyjs']['recurlyjs_accept_paypal']['#disabled'] = TRUE;
    $form['recurlyjs']['recurlyjs_accept_paypal']['#value'] = FALSE;
    $form['recurlyjs']['recurlyjs_accept_paypal']['#description'] .= ' <strong>' . t('To enable this feature please install <a href="http://drupal.org/project/jquery_update">jQuery Update</a> and use jQuery 1.7 or higher.') . '</strong>';
  }
}

/**
 * Menu callback; Display the page for starting a subscription.
 */
function recurlyjs_subscribe_page($entity_type, $entity, $plan_code, $currency = NULL) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Ensure the account does not already have this exact same plan. Recurly
  // does not support a single account having multiple of the same plan.
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $local_account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id), TRUE);
  if ($local_account) {
    $current_subscriptions = recurly_account_get_subscriptions($local_account->account_code, 'active');

    // If the account is only allowed one subscription total, they shouldn't
    // ever see this signup page.
    if (variable_get('recurly_subscription_max', '1') === '1' && count($current_subscriptions) && empty($_POST)) {
      $current_subscription = reset($current_subscriptions);
      drupal_set_message(t('This account already has a @plan plan!', array('@plan' => $current_subscription->plan->name)));
      drupal_goto($entity_type . '/' . $id . '/subscription');
    }

    // Otherwise check if they already have one of this same plan.
    foreach ($current_subscriptions as $current_subscription) {
      if ($current_subscription->plan->plan_code === $plan_code && empty($_POST)) {
        drupal_set_message(t('This account already has a @plan plan!', array('@plan' => $current_subscription->plan->name)));
        drupal_goto($entity_type . '/' . $id . '/subscription/signup');
      }
    }
  }

  try {
    $plan = Recurly_Plan::get($plan_code);
  }
  catch (Recurly_NotFoundError $e) {
    return MENU_NOT_FOUND;
  }
  // Although this menu callback contains little else besides the subscription
  // form, it's a separate function because it's highly likely to need theming.
  $form = drupal_get_form('recurlyjs_subscribe_form', $entity_type, $entity, $plan_code, $plan, $currency);
  return theme('recurlyjs_subscribe_page', array('form' => $form));
}

/**
 * Form callback; Display the form for starting a subscription.
 *
 * @param string $entity_type
 *   The type of the entity who is subscribing.
 * @param object $entity
 *   The entity who is subscribing.
 * @param string $plan_code
 *   The code of the plan to which the entity is subscribing.
 * @param object $plan
 *   The plan to which the entity is subscribing.
 * @param string $currency
 *   Optional currency to use for subscription form.
 *
 * @return array
 *   A Form API array.
 */
function recurlyjs_subscribe_form($form, $form_state, $entity_type, $entity, $plan_code, $plan, $currency = NULL) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    $form['error'] = array(
      '#markup' => t('Could not initialize the Recurly client.'),
    );
    return $form;
  }

  $currency = is_null($currency) ? variable_get('recurly_default_currency', 'USD') : $currency;

  $form['#entity_type'] = $entity_type;
  $form['#entity'] = $entity;
  $form['#plan_code'] = $plan_code;
  $form['#plan'] = $plan;
  $form['#currency'] = $currency;

  $form['plan'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('recurlyjs-plan'),
    ),
  );

  // Plan name, cost, and frequency.
  foreach ($plan->unit_amount_in_cents as $unit_currency) {
    if ($unit_currency->currencyCode === $currency) {
      $unit_amount = recurly_format_currency($unit_currency->amount_in_cents, $unit_currency->currencyCode);
      break;
    }
  }
  $plan_name = check_plain($plan->__get('name'));
  $frequency = '<span class="recurlyjs-element recurlyjs-plan-frequency">(' . recurly_format_price_interval($unit_amount, $plan->__get('plan_interval_length'), $plan->__get('plan_interval_unit'), TRUE) . ')</span>';
  $form['plan']['plan_name'] = array(
    '#markup' => '<div class="recurlyjs-element recurlyjs-plan-name">' . $plan_name . ' ' . $frequency . '</div>',
  );

  $form['pricing'] = array(
    '#type' => '#container',
    '#attributes' => array(
      'class' => array('recurlyjs-pricing'),
    ),
  );
  // For the recurly.Pricing() module, we need to make sure we include an input
  // field with the plan name.
  $form['pricing'] = array();
  $form['pricing']['plan_code'] = array(
    '#type' => 'hidden',
    '#value' => $plan_code,
    '#attributes' => array(
      'data-recurly' => 'plan',
    ),
  );

  // Display order summary.
  // Setup fee. Hidden by default, populated by JS as needed.
  $form['pricing']['plan_setup'] = array(
    '#markup' => '<div class="recurlyjs-element recurlyjs-setup-fee recurlyjs-element__hidden">' . t('Setup fee:') . ' <span data-recurly="currency_symbol"></span><span data-recurly="setup_fee_now"></span></div>',
  );

  // Discount. Hidden by default, populated by JS as needed.
  $form['pricing']['plan_discount'] = array(
    '#markup' => '<div class="recurlyjs-element recurlyjs-discount recurlyjs-element__hidden">' . t('Discount:') . ' <span data-recurly="currency_symbol"></span><span data-recurly="discount_now"></span></div>',
  );

  // Sub total. Hidden by default, populated by JS as needed.
  $form['pricing']['plan_subtotal'] = array(
    '#markup' => '<div class="recurlyjs-element recurlyjs-subtotal recurlyjs-element recurlyjs-element__hidden">' . t('Subtotal:') . ' <span data-recurly="currency_symbol"></span><span data-recurly="subtotal_now"></span></div>',
  );

  // Taxes. Hidden by default, populated by JS as needed.
  $form['pricing']['plan_tax'] = array(
    '#markup' => '<div class="recurlyjs-element recurlyjs-tax recurlyjs-element recurlyjs-element__hidden">' . t('Taxes:') . ' <span data-recurly="currency_symbol"></span><span data-recurly="tax_now"></span></div>',
  );

  // Total cost.
  $form['pricing']['plan_total'] = array(
    '#markup' => '<div class="recurlyjs-element recurlyjs-total recurlyjs-element">' . t('Order total:') . ' <span data-recurly="currency_symbol"></span><span data-recurly="total_now"></span></div>',
  );

  if (variable_get('recurlyjs_enable_coupons')) {
    $form['coupon_code'] = array(
      '#type' => 'textfield',
      '#title' => t('Coupon Code'),
      '#description' => t('Recurly coupon code to be applied to subscription.'),
      '#element_validate' => array('_recurlyjs_element_validate_coupon_code'),
      '#attributes' => array(
        'data-recurly' => 'coupon',
      ),
    );
  }

  _recurlyjs_append_billing_fields($form);
  _recurlyjs_form_attach_js($form);

  if (variable_get('recurlyjs_hide_vat_number')) {
    $form['billing']['vat_number']['#access'] = FALSE;
  }
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Purchase'),
  );
  return $form;
}

/**
 * Submit handler for recurlyjs_subscribe_form().
 */
function recurlyjs_subscribe_form_submit($form, &$form_state) {
  $entity_type = $form['#entity_type'];
  $entity = $form['#entity'];
  $plan_code = $form['#plan_code'];
  $currency = $form['#currency'];
  $recurly_token = isset($form_state['values']['recurly-token']) ? $form_state['values']['recurly-token'] : NULL;
  $coupon_code = isset($form_state['values']['coupon_code']) ? $form_state['values']['coupon_code'] : NULL;

  list($entity_id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $recurly_account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $entity_id));

  if (!$recurly_account) {
    $recurly_account = new Recurly_Account();
    $recurly_account->first_name = check_plain($form_state['values']['first_name']);
    $recurly_account->last_name = check_plain($form_state['values']['last_name']);
    if ($entity_type == 'user') {
      $recurly_account->email = $entity->mail;
      $recurly_account->username = $entity->name;
    }

    // Account code is the only property required for Recurly account creation.
    // https://dev.recurly.com/docs/create-an-account.
    $recurly_account->account_code = $entity_type . '-' . $entity_id;

    // Allow other modules the chance to alter the new Recurly Account object
    // before it is saved.
    drupal_alter('recurlyjs_account', $recurly_account, $entity, $plan_code);
  }

  $subscription = new Recurly_Subscription();
  $subscription->account = $recurly_account;
  $subscription->plan_code = $plan_code;
  $subscription->currency = $currency;
  $subscription->coupon_code = $coupon_code;

  // Allow other modules the chance to alter the new Recurly Subscription object
  // before it is saved.
  drupal_alter('recurlyjs_subscription', $subscription, $entity, $plan_code);

  // Billing info is based on the token we retrieved from the Recurly JS API
  // and should only contain the token in this case. We add this after the above
  // alter hook to ensure it's not modified.
  $subscription->account->billing_info = new Recurly_BillingInfo();
  $subscription->account->billing_info->token_id = $recurly_token;

  try {
    // This saves all of the data assembled above in addition to creating a new
    // subscription record.
    $subscription->create();
  }
  catch (Recurly_ValidationError $e) {
    // There was an error validating information in the form. For example,
    // credit card was declined. We don't need to log these in Drupal, you can
    // find the errors logged within Recurly.
    drupal_set_message(t('<strong>Unable to create subscription:</strong><br/>@error', array('@error' => $e->getMessage())), 'error');
    $form_state['rebuild'] = TRUE;
    return;
  }
  catch (Recurly_Error $e) {
    // Catch any non-validation errors. This will be things like unable to
    // contact Recurly API, or lower level errors. Display a generic message to
    // the user letting them know there was an error and then log the detailed
    // version. There's probably nothing a user can do to correct these errors
    // so we don't need to display the details.
    watchdog('recurlyjs', 'Unable to create subscription: @error', array('@error' => $e->getMessage()));
    drupal_set_message(t('An error occured while trying to create your subscription. Please contact a site administrator.'));
    $form_state['rebuild'] = TRUE;
    return;
  }

  // Allow other modules to react to the new subscription being created.
  module_invoke_all('recurlyjs_new_subscription', $subscription, $entity);

  drupal_set_message(t('Account upgraded to @plan!', array('@plan' => $subscription->plan->name)));

  // Save the account locally immediately so that subscriber information may
  // be retrieved when the user is directed back to the /subscription tab.
  try {
    $account = $subscription->account->get();
    recurly_account_save($account, $entity_type, $entity_id);
  }
  catch (Recurly_Error $e) {
    watchdog('recurly', 'New subscriber account could not be retreived from Recurly. Received the following error: @error', array('@error' => $e->getMessage()));
  }

  $form_state['redirect'] = $entity_type . '/' . $entity_id . '/subscription';
}

/**
 * Form callback; Display the form for updating billing information.
 *
 * @param string $entity_type
 *   The type of the entity whose info is being updated.
 * @param object $entity
 *   The entity whose info is being updated.
 */
function recurlyjs_update_billing_form($form, $form_state, $entity_type, $entity) {
  // Includes iframe styling.
  drupal_add_css(drupal_get_path('module', 'recurlyjs') . '/css/recurlyjs.css');

  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    $form['error'] = array(
      '#markup' => t('Could not initialize the Recurly client.'),
    );
    return $form;
  }

  // See if we have a local mapping of entity ID to Recurly account code.
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $recurly_account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));

  try {
    $billing_info = Recurly_BillingInfo::get($recurly_account->account_code);
    $form['existing'] = array(
      '#theme' => 'recurly_credit_card_information',
      '#card_type' => check_plain($billing_info->card_type),
      '#first_name' => check_plain($billing_info->first_name),
      '#last_name' => check_plain($billing_info->last_name),
      '#year' => check_plain($billing_info->year),
      '#month' => check_plain($billing_info->month),
      '#mask_length' => strcasecmp($billing_info->card_type, 'American Express') === 0 ? 11 : 12,
      '#last_four' => check_plain($billing_info->last_four),
    );
  }
  catch (Recurly_NotFoundError $e) {
    watchdog('recurlyjs', 'Unable to retrieve billing information. Received the following error: @error', array('@error' => $e->getMessage()));
    drupal_set_message(t('Unable to retrieve billing information.'), 'error');
    return $form;
  }

  $form['#entity_type'] = $entity_type;
  $form['#entity'] = $entity;

  _recurlyjs_append_billing_fields($form);
  _recurlyjs_form_attach_js($form);

  $form['billing']['#title'] = t('New payment information');

  // Populate #default_value with existing billing info.
  $excluded_fields = array('month', 'year');
  foreach (element_children($form) as $form_element_name) {
    if (!in_array($form_element_name, $excluded_fields)) {
      $form[$form_element_name]['#default_value'] = $form_element_name != 'postal_code' ? $billing_info->$form_element_name : $billing_info->zip;
    }
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('recurlyjs_update_billing_submit'),
  );
  return $form;
}

/**
 * Submit handler for recurlyjs_update_billing_form().
 */
function recurlyjs_update_billing_submit($form, $form_state) {
  $entity_type = $form['#entity_type'];
  $entity = $form['#entity'];
  $recurly_token = isset($form_state['values']['recurly-token']) ? $form_state['values']['recurly-token'] : NULL;

  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $recurly_account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));

  if ($recurly_token && $recurly_account) {
    try {
      $billing_info = new Recurly_BillingInfo();
      $billing_info->account_code = $recurly_account->account_code;
      $billing_info->token_id = $recurly_token;
      $billing_info->update();
    }
    catch (Recurly_ValidationError $e) {
      // There was an error validating information in the form. For example,
      // credit card was declined. Let the user know. These errors are logged in
      // Recurly.
      drupal_set_message(t('<strong>Unable to update account:</strong><br/>@error', array('@error' => $e->getMessage())), 'error');
    }
    catch (Recurly_NotFoundError $e) {
      drupal_set_message(t('Could not find account or token is invalid or expired.'), 'error');
      $form_state['#rebuild'] = TRUE;
    }
    catch (Recurly_Error $e) {
      // Catch all other errors. Log the details, and display a message for the
      // user.
      watchdog('recurlyjs', 'Billing information update error: @error', array('@error' => $e->getMessage()));
      drupal_set_message(t('An error occured while trying to update your account. Please contact a site administrator.'));
      $form_state['#rebuild'] = TRUE;
    }
  }
}

/**
 * Configure Form API elements for Recurly billing forms.
 *
 * @param array &$form
 *   A Drupal form array.
 */
function _recurlyjs_append_billing_fields(array &$form) {
  $form['#prefix'] = '<div class="recurly-form-wrapper">';
  $form['#suffix'] = '</div>';
  $form['billing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment information'),
    '#attributes' => array(
      'class' => array('recurlyjs-billing-info'),
    ),
  );
  // recurly-element.js adds errors here upon failed validation.
  $form['errors'] = array(
    '#markup' => '<div id="recurly-form-errors"></div>',
    '#weight' => -300,
  );

  $form['billing']['name'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('recurlyjs-name-wrapper'),
    ),
  );
  $form['billing']['name']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#attributes' => array(
      'data-recurly' => 'first_name',
    ),
    '#prefix' => '<div class="recurlyjs-form-item__first_name">',
    '#suffix' => '</div>',
  );
  $form['billing']['name']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#attributes' => array(
      'data-recurly' => 'last_name',
    ),
    '#after_build' => array('_recurlyjs_form_remove_element_name'),
    '#prefix' => '<div class="recurlyjs-form-item__last_name">',
    '#suffix' => '</div>',
  );
  $form['billing']['cc_info'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('recurlyjs-cc-info'),
    ),
  );
  // Credit card fields are represented as <divs> in the DOM and Recurly.JS will
  // dynamically replace them with an input field inside of an iFrame. In order
  // to ensure these fields never contain data in Drupal's Form API we just add
  // them as static markup.
  $form['billing']['cc_info']['number'] = array(
    '#title' => t('Card Number'),
    '#markup' => '<label for="number">' . t('Card Number') . '</label><div data-recurly="number"></div>',
    '#prefix' => '<div class="form-item recurlyjs-form-item__number">',
    '#suffix' => '<span class="recurlyjs-icon-card recurlyjs-icon-card__inline recurlyjs-icon-card__unknown"></span></div>',
  );
  $form['billing']['cc_info']['cvv'] = array(
    '#title' => t('CVV'),
    '#markup' => '<label for="cvv">' . t('CVV') . '</label><div data-recurly="cvv"></div>',
    '#prefix' => '<div class="form-item recurlyjs-form-item__cvv">',
    '#suffix' => '</div>',
  );
  $form['billing']['cc_info']['month'] = array(
    '#title' => t('Month'),
    '#markup' => '<label for="month">' . t('Month') . '</label><div data-recurly="month"></div>',
    '#prefix' => '<div class="form-item recurlyjs-form-item__month">',
    '#suffix' => '</div>',
  );
  $form['billing']['cc_info']['year'] = array(
    '#title' => t('Year'),
    '#markup' => '<label for="year">' . t('Year') . '</label><div data-recurly="year"></div>',
    '#prefix' => '<div class="form-item recurlyjs-form-item__year">',
    '#suffix' => '</div>',
  );

  $address_requirement = variable_get('recurlyjs_address_requirement', 'full');
  $hide_vat_number = variable_get('recurlyjs_hide_vat_number', 0);

  if (in_array($address_requirement, array('zipstreet', 'full'))) {
    $form['billing']['address1'] = array(
      '#type' => 'textfield',
      '#title' => t('Address Line 1'),
      '#attributes' => array(
        'data-recurly' => 'address1',
      ),
      '#prefix' => '<div class="recurlyjs-form-item__address1">',
      '#suffix' => '</div>',
      '#after_build' => array('_recurlyjs_form_remove_element_name'),
    );
    $form['billing']['address2'] = array(
      '#type' => 'textfield',
      '#title' => t('Address Line 2'),
      '#attributes' => array(
        'data-recurly' => 'address2',
      ),
      '#prefix' => '<div class="recurlyjs-form-item__address2">',
      '#suffix' => '</div>',
      '#after_build' => array('_recurlyjs_form_remove_element_name'),
    );
  }

  $form['billing']['city_state_postal'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('recurlyjs-city-state-postal-wrapper'),
    ),
  );

  if ($address_requirement == 'full') {
    $form['billing']['city_state_postal']['city'] = array(
      '#type' => 'textfield',
      '#title' => t('City'),
      '#attributes' => array(
        'data-recurly' => 'city',
      ),
      '#prefix' => '<div class="recurlyjs-form-item__city">',
      '#suffix' => '</div>',
      '#after_build' => array('_recurlyjs_form_remove_element_name'),
    );
    $form['billing']['city_state_postal']['state'] = array(
      '#type' => 'textfield',
      '#title' => t('State'),
      '#attributes' => array(
        'data-recurly' => 'state',
      ),
      '#prefix' => '<div class="recurlyjs-form-item__state">',
      '#suffix' => '</div>',
      '#after_build' => array('_recurlyjs_form_remove_element_name'),
    );
  }

  if ($address_requirement != 'none') {
    $form['billing']['city_state_postal']['postal_code'] = array(
      '#type' => 'textfield',
      '#title' => t('Postal Code'),
      '#attributes' => array(
        'data-recurly' => 'postal_code',
      ),
      '#prefix' => '<div class="recurlyjs-form-item__postal_code">',
      '#suffix' => '</div>',
      '#after_build' => array('_recurlyjs_form_remove_element_name'),
    );
  }

  if ($address_requirement == 'full') {
    // country_get_list() requires includes/locale.inc.
    require_once DRUPAL_ROOT . '/includes/locale.inc';
    $form['billing']['country'] = array(
      '#type' => 'select',
      '#title' => t('Country'),
      '#attributes' => array(
        'data-recurly' => 'country',
      ),
      '#prefix' => '<div class="recurlyjs-form-item__country">',
      '#suffix' => '</div>',
      '#after_build' => array('_recurlyjs_form_remove_element_name'),
      '#options' => country_get_list(),
    );
  }

  if (!$hide_vat_number) {
    $form['billing']['vat_number'] = array(
      '#type' => 'textfield',
      '#title' => t('VAT Number'),
      '#attributes' => array(
        'data-recurly' => 'vat_number',
      ),
      '#prefix' => '<div class="recurlyjs-form-item__vat_number">',
      '#suffix' => '</div>',
      '#after_build' => array('_recurlyjs_form_remove_element_name'),
    );
  }

  $form['billing']['tax_code'] = array(
    '#type' => 'hidden',
    '#title' => t('digital'),
    '#attributes' => array(
      'data-recurly' => 'tax_code',
    ),
    '#prefix' => '<div class="recurlyjs-form-item__tax_code">',
    '#suffix' => '</div>',
    '#after_build' => array('_recurlyjs_form_remove_element_name'),
  );
  $form['recurly-token'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'data-recurly' => 'token',
    ),
  );
}

/**
 * Element after_build callback to remove the input #name attribute.
 *
 * @see https://docs.recurly.com/js/#build-a-card-form
 */
function _recurlyjs_form_remove_element_name($element, $form_state) {
  unset($element['#name']);
  return $element;
}

/**
 * Element validate callback.
 */
function _recurlyjs_element_validate_coupon_code($element, &$form_state, $form) {
  $coupon_code = isset($form_state['values']['coupon_code']) ? $form_state['values']['coupon_code'] : NULL;
  if (!$coupon_code) {
    return;
  }
  $currency = $form['#currency'];
  $plan_code = $form['#plan_code'];

  // Query Recurly to make sure this is a valid coupon code.
  try {
    $coupon = Recurly_Coupon::get($coupon_code);
  }
  catch (Recurly_NotFoundError $e) {
    form_error($element, t('The coupon code you have entered is not valid.'));
    return;
  }
  // Check that the coupon is available in the specified currency.
  if ($coupon && $coupon->discount_type !== 'percent') {
    if (!$coupon->discount_in_cents->offsetExists($currency)) {
      form_error($element, t('The coupon code you have entered is not valid in @currency.', array('@currency' => $currency)));
      return;
    }
  }
  // Check the the coupon is valid for the specified plan.
  if ($coupon && !_recurlyjs_coupon_valid_for_plan($coupon, $plan_code)) {
    form_error($element, t('The coupon code you have entered is not valid for the specified plan.'));
    return;
  }
}

/**
 * Validate Recurly coupon against a specified plan.
 *
 * @todo Move to recurly.module?
 *
 * @param Recurly_Coupon $recurly_coupon
 *   A Recurly coupon object.
 * @param string $plan_code
 *   A Recurly plan code.
 *
 * @return BOOL
 *   TRUE if the coupon is valid for the specified plan, else FALSE.
 */
function _recurlyjs_coupon_valid_for_plan(Recurly_Coupon $recurly_coupon, $plan_code) {
  return ($recurly_coupon->applies_to_all_plans || in_array($plan_code, $recurly_coupon->plan_codes));
}

/**
 * Attach Recurly JS library to Drupal form.
 *
 * @param array &$form
 *   A Drupal form array.
 */
function _recurlyjs_form_attach_js(array &$form) {
  $recurly_public_key = variable_get('recurly_public_key', '');
  // Add Recurly.js and the inline code to the page.
  $form['#attached']['js']['https://js.recurly.com/v4/recurly.js'] = array('type' => 'external');
  $form['#attached']['js'][] = drupal_get_path('module', 'recurlyjs') . '/recurly-element.js';
  $form['#attached']['js'][] = array(
    'data' => array('public_key' => $recurly_public_key),
    'type' => 'setting',
  );
}
