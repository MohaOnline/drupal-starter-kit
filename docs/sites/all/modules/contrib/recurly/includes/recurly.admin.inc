<?php

/**
 * @file
 * Recurly settings forms and administration page callbacks.
 */

/**
 * Returns the site-wide Recurly settings form.
 */
function recurly_settings_form($form, &$form_state) {
  // Recommend setting some subscription plans if not enabled.
  $plan_options = array_filter(variable_get('recurly_subscription_plans', array()));
  if (empty($plan_options) && variable_get('recurly_private_api_key', '') && variable_get('recurly_pages', '1')) {
    drupal_set_message(t('Recurly built-in pages are enabled, but no plans have yet been enabled. Enable plans on the <a href="!url">Subscription Plans page</a>.', array('!url' => url('admin/config/services/recurly/subscription-plans'))), 'warning', FALSE);
  }

  // Add form elements to collect default account information.
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default account settings'),
    '#description' => t('Configure this information based on the "API Credentials" section within the Recurly administration interface.'),
    '#collapsible' => TRUE,
  );
  $form['account']['recurly_private_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private API Key'),
    '#default_value' => variable_get('recurly_private_api_key', ''),
  );
  $form['account']['recurly_public_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Public Key'),
    '#description' => t('Enter this if needed for Recurly.js. Note that this version of the Recurly module only supports Recurly.js v4, which uses the "public key" and not the "transparent post key" used by Recurly.js v2.'),
    '#default_value' => variable_get('recurly_public_key', ''),
  );
  $form['account']['recurly_subdomain'] = array(
    '#type' => 'textfield',
    '#title' => t('Subdomain'),
    '#description' => t("The subdomain of your account."),
    '#default_value' => variable_get('recurly_subdomain', ''),
  );
  $currencies = array_keys(recurly_currency_list());
  $form['account']['recurly_default_currency'] = array(
    '#type' => 'select',
    '#title' => t('Default currency'),
    '#description' => t('Select the 3-character currency code for the currency you would like to use by default.You can find a list of supported currencies in your <a href="!url">Recurly account currencies page</a>.', array('!url' => recurly_hosted_url('configuration/currencies'))),
    '#default_value' => variable_get('recurly_default_currency', 'USD'),
    '#options' => array_combine($currencies, $currencies),
  );

  // Add form elements to configure default push notification settings.
  $form['push'] = array(
    '#type' => 'fieldset',
    '#title' => t('Push notification settings'),
    '#description' => t('If you have supplied an HTTP authentication username and password in your Push Notifications settings at Recurly, your web server must be configured to validate these credentials at your listener URL.'),
    '#collapsible' => TRUE,
  );
  $form['push']['recurly_listener_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Listener URL key'),
    '#description' => t('Customizing the listener URL gives you protection against fraudulent push notifications.') . '<br />' . t('Based on your current key, you should set @url as your Push Notification URL at Recurly.',
      array(
        '@url' => url('recurly/listener/' . variable_get('recurly_listener_key', ''), array('absolute' => TRUE)),
      )),
    '#default_value' => variable_get('recurly_listener_key', ''),
    '#required' => TRUE,
    '#size' => 32,
    '#field_prefix' => url('recurly/listener/', array('absolute' => TRUE)),
  );
  $form['push']['recurly_push_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log authenticated incoming push notifications. (Primarily used for debugging purposes.)'),
    '#default_value' => variable_get('recurly_push_logging', FALSE),
  );

  $entity_types = entity_get_info();
  $entity_options = array();
  foreach ($entity_types as $entity_name => $entity_info) {
    $entity_options[$entity_name] = $entity_info['label'];
    $first_bundle_name = key($entity_info['bundles']);
    // Don't generate a list of bundles if this entity does not have types.
    if ($entity_info['bundles'] > 1 && $first_bundle_name !== $entity_name) {
      foreach ($entity_info['bundles'] as $bundle_name => $bundle_info) {
        $entity_type_options[$entity_name][$bundle_name] = $bundle_info['label'];
      }
    }
  }

  // If any of the below options change we need to rebuild the menu system.
  // Keep a record of their current values.
  $recurly_entity_type = variable_get('recurly_entity_type', 'user');
  $form_state['pages_previous_values'] = array(
    'recurly_entity_type' => $recurly_entity_type,
    'recurly_bundle_' . $recurly_entity_type => variable_get('recurly_bundle_' . $recurly_entity_type),
    'recurly_pages' => variable_get('recurly_pages', '1'),
    'recurly_coupon_page' => variable_get('recurly_coupon_page', '1'),
    'recurly_subscription_plans' => variable_get('recurly_subscription_plans', array()),
    'recurly_subscription_max' => variable_get('recurly_subscription_max', '1'),
  );

  $form['sync'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recurly account syncing'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($recurly_entity_type),
    '#description' => t("Each time a particular object type (User, Node, Group, etc.) is updated, you may have information sent to Recurly to keep the contact information kept up-to-date. It is extremely important to maintain updated contact information in Recurly, as when an account enters the dunning process, the e-mail account in Recurly is the primary contact address."),
  );
  $form['sync']['recurly_entity_type'] = array(
    '#title' => t('Send Recurly account updates for each'),
    '#type' => 'select',
    '#options' => array('' => 'Nothing (disabled)') + $entity_options,
    '#default_value' => $recurly_entity_type,
  );
  if (!empty($entity_type_options)) {
    foreach ($entity_type_options as $entity_name => $bundles) {
      $form['sync']['recurly_bundles']['recurly_bundle_' . $entity_name] = array(
        '#title' => t('Specifically the following @entity type', array('@entity' => $entity_types[$entity_name]['label'])),
        '#type' => 'select',
        '#options' => $bundles,
        '#default_value' => variable_get('recurly_bundle_' . $entity_name),
        '#states' => array(
          'visible' => array(
            'select[name="recurly_entity_type"]' => array('value' => $entity_name),
          ),
        ),
      );
    }
  }

  $mapping = recurly_token_mapping();
  $form['sync']['recurly_token_mapping'] = array(
    '#title' => t('Token mappings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#parents' => array('recurly_token_mapping'),
    '#description' => t('Each Recurly account field is displayed below, specify a token that will be used to update the Recurly account each time the object (node or user) is updated. The Recurly "username" field is automatically populated with the object name (for users) or title (for nodes).'),
  );
  $form['sync']['recurly_token_mapping']['email'] = array(
    '#title' => t('Email'),
    '#type' => 'textfield',
    '#default_value' => $mapping['email'],
    '#description' => t('i.e. [user:mail] or [node:author:mail]'),
  );
  $form['sync']['recurly_token_mapping']['username'] = array(
    '#title' => t('Username'),
    '#type' => 'textfield',
    '#default_value' => $mapping['username'],
    '#description' => t('i.e. [user:name] or [node:title]'),
  );
  $form['sync']['recurly_token_mapping']['first_name'] = array(
    '#title' => t('First name'),
    '#type' => 'textfield',
    '#default_value' => $mapping['first_name'],
  );
  $form['sync']['recurly_token_mapping']['last_name'] = array(
    '#title' => t('Last name'),
    '#type' => 'textfield',
    '#default_value' => $mapping['last_name'],
  );
  $form['sync']['recurly_token_mapping']['company_name'] = array(
    '#title' => t('Company'),
    '#type' => 'textfield',
    '#default_value' => $mapping['company_name'],
  );
  $form['sync']['recurly_token_mapping']['address1'] = array(
    '#title' => t('Address line 1'),
    '#type' => 'textfield',
    '#default_value' => $mapping['address1'],
  );
  $form['sync']['recurly_token_mapping']['address2'] = array(
    '#title' => t('Address line 2'),
    '#type' => 'textfield',
    '#default_value' => $mapping['address2'],
  );
  $form['sync']['recurly_token_mapping']['city'] = array(
    '#title' => t('City'),
    '#type' => 'textfield',
    '#default_value' => $mapping['city'],
  );
  $form['sync']['recurly_token_mapping']['state'] = array(
    '#title' => t('State'),
    '#type' => 'textfield',
    '#default_value' => $mapping['state'],
    '#description' => t('Values sent to Recurly must be two-letter abbreviations.'),
  );
  $form['sync']['recurly_token_mapping']['zip'] = array(
    '#title' => t('Zip code'),
    '#type' => 'textfield',
    '#default_value' => $mapping['zip'],
  );
  // country_get_list() requires includes/locale.inc
  require_once DRUPAL_ROOT . '/includes/locale.inc';
  $form['sync']['recurly_token_mapping']['country'] = array(
    '#title' => t('Country'),
    '#type' => 'select',
    '#default_value' => $mapping['country'],
    '#options' => country_get_list(),
  );
  $form['sync']['recurly_token_mapping']['phone'] = array(
    '#title' => t('Phone number'),
    '#type' => 'textfield',
    '#default_value' => $mapping['phone'],
  );
  $form['sync']['recurly_token_mapping']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array_keys($entity_options),
    '#global_types' => FALSE,
  );
  $form['pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Built-in subscription/invoice pages'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('recurly_pages', '1'),
    '#description' => t('The Recurly module provides built-in pages for letting users view their own recent invoices on the site instead of needing to go to the Recurly site. If a companion module is enabled such as the Recurly Hosted Pages or Recurly.js module (both included with this project), appropriate links to update billing information or subscribe will also be displayed on these pages.'),
    '#states' => array(
      'visible' => array(
        'select[name="recurly_entity_type"]' => array('!value' => ''),
      ),
    ),
  );
  $form['pages']['recurly_pages'] = array(
    '#title' => t('Enable built-in pages'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('recurly_pages', '1'),
    '#description' => t('Hosted pages will be enabled on the same object type as the "Account Syncing" option above.'),
  );

  // All the settings below are dependent upon the pages option being enabled.
  $pages_enabled = array(
    'visible' => array(
      'input[name=recurly_pages]' => array('checked' => TRUE),
    ),
  );
  $form['pages']['recurly_coupon_page'] = array(
    '#title' => t('Enable coupon redemption page'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('recurly_coupon_page', '1'),
    '#description' => t('Show the "Redeem coupon" tab underneath the subscription page.'),
    '#states' => $pages_enabled,
  );
  $form['pages']['recurly_subscription_display'] = array(
    '#title' => t('List subscriptions'),
    '#type' => 'radios',
    '#options' => array(
      'live' => t('Live subscriptions (active, trials, canceled, and past due)'),
      'all' => t('All (includes expired subscriptions)'),
    ),
    '#description' => t('Users may subscribe or switch between any of the enabled plans by visiting the Subscription tab.'),
    '#default_value' => variable_get('recurly_subscription_display', 'live'),
    '#states' => $pages_enabled,
  );
  $form['pages']['recurly_subscription_max'] = array(
    '#title' => t('Multiple plans'),
    '#type' => 'radios',
    // Allows the number of plans to be some arbitrary amount in the future.
    '#options' => array(
      '1' => t('Single-plan mode'),
      '0' => t('Multiple-plan mode'),
    ),
    '#description' => t('Single-plan mode allows users are only one subscription at a time, preventing them from having multiple plans active at the same time. If users are allowed to sign up for more than one subscription, use Multiple-plan mode.'),
    '#access' => count($plan_options),
    '#default_value' => variable_get('recurly_subscription_max', '1'),
    '#states' => $pages_enabled,
  );
  $form['pages']['recurly_subscription_upgrade_timeframe'] = array(
    '#title' => t('Upgrade plan behavior'),
    '#type' => 'radios',
    '#options' => array(
      'now' => t('Upgrade immediately (pro-rating billing period usage)'),
      'renewal' => t('On next renewal'),
    ),
    '#access' => count($plan_options) > 1,
    '#description' => t('Affects users who are able to change their own plan (if more than one is enabled). Overriddable when changing plans as users with "Administer Recurly" permission.') . ' ' . t('An upgrade is considered moving to any plan that costs more than the current plan (regardless of billing cycle).'),
    '#default_value' => variable_get('recurly_subscription_upgrade_timeframe', 'now'),
    '#states' => $pages_enabled,
  );
  $form['pages']['recurly_subscription_downgrade_timeframe'] = array(
    '#title' => t('Downgrade plan behavior'),
    '#type' => 'radios',
    '#options' => array(
      'now' => t('Downgrade immediately (pro-rating billing period usage)'),
      'renewal' => t('On next renewal'),
    ),
    '#access' => count($plan_options) > 1,
    '#description' => t('Affects users who are able to change their own plan (if more than one is enabled). Overriddable when changing plans as users with "Administer Recurly" permission.') . ' ' . t('An downgrade is considered moving to any plan that costs less than the current plan (regardless of billing cycle).'),
    '#default_value' => variable_get('recurly_subscription_downgrade_timeframe', 'renewal'),
    '#states' => $pages_enabled,
  );
  $form['pages']['recurly_subscription_cancel_behavior'] = array(
    '#title' => t('Cancel plan behavior'),
    '#type' => 'radios',
    '#options' => array(
      'cancel' => t('Cancel at renewal (leave active until end of period)'),
      'terminate_prorated' => t('Terminate immediately (prorated refund)'),
      'terminate_full' => t('Terminate immediately (full refund)'),
    ),
    '#description' => t('Affects users canceling their own subscription plans. Overriddable when canceling plans as users with "Administer Recurly" permission. Note that this behavior is also used when content associated with a Recurly account is deleted, or when users associated with an account are canceled.'),
    '#enabled' => count($plan_options) > 1,
    '#default_value' => variable_get('recurly_subscription_cancel_behavior', 'cancel'),
    '#states' => $pages_enabled,
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'recurly_settings_form_submit';
  return $form;
}

/**
 * Trims user-supplied API text values.
 */
function recurly_settings_form_validate($form, &$form_state) {
  $keys = array(
    'recurly_private_api_key',
    'recurly_public_key',
    'recurly_subdomain',
    'recurly_listener_key',
  );
  foreach ($keys as $key) {
    $form_state['values'][$key] = trim($form_state['values'][$key]);
  }

  // Check that the API key is valid.
  if ($form_state['values']['recurly_private_api_key']) {
    try {
      $settings = array(
        'api_key' => $form_state['values']['recurly_private_api_key'],
        'public_key' => $form_state['values']['recurly_public_key'],
        'subdomain' => $form_state['values']['recurly_subdomain'],
      );
      recurly_client_initialize($settings, TRUE);
      $plans = recurly_subscription_plans();
    }
    catch (Recurly_UnauthorizedError $e) {
      form_set_error('recurly_private_api_key', t('Your API Key is not authorized to connect to Recurly.'));
    }
  }
}

/**
 * Submit handler for recurly_settings_form().
 */
function recurly_settings_form_submit($form, &$form_state) {
  // Rebuild the menu system if any of the built-in page options change.
  foreach ($form_state['pages_previous_values'] as $variable_name => $previous_value) {
    if (isset($form_state['values'][$variable_name]) && $form_state['values'][$variable_name] !== $previous_value) {
      menu_rebuild();
    }
  }
}

/**
 * Displays a list of subscription plans as defined in your Recurly account.
 */
function recurly_subscription_plans_overview() {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  try {
    $plans = recurly_subscription_plans();
  }
  catch (Recurly_Error $e) {
    return t('No plans could be retrieved from Recurly. Recurly reported the following error: "@error"', array('@error' => $e->getMessage()));
  }

  return drupal_get_form('recurly_subscription_plans_form', $plans);
}

/**
 * Form handler; Provide a form for ordering and enabling subscription plans.
 */
function recurly_subscription_plans_form($form, &$form_state, $plans) {
  $form['weights']['#tree'] = TRUE;

  $plan_options = array();
  $count = 0;
  foreach ($plans as $plan) {
    $plan_options[$plan->plan_code] = $plan->name;
    $form['#plans'][$plan->plan_code] = array(
      'plan' => $plan,
      'unit_amounts' => array(),
      'setup_amounts' => array(),
    );

    // TODO: Remove reset() calls once Recurly_CurrencyList implements Iterator.
    // See https://github.com/recurly/recurly-client-php/issues/37
    $unit_amounts = in_array('IteratorAggregate', class_implements($plan->unit_amount_in_cents)) ? $plan->unit_amount_in_cents : reset($plan->unit_amount_in_cents);
    $setup_fees = in_array('IteratorAggregate', class_implements($plan->setup_fee_in_cents)) ? $plan->setup_fee_in_cents : reset($plan->setup_fee_in_cents);
    foreach ($unit_amounts as $unit_amount) {
      $form['#plans'][$plan->plan_code]['unit_amounts'][$unit_amount->currencyCode] = t('@unit_price every @interval_length @interval_unit',
        array(
          '@unit_price' => recurly_format_currency($unit_amount->amount_in_cents, $unit_amount->currencyCode),
          '@interval_length' => $plan->plan_interval_length,
          '@interval_unit' => $plan->plan_interval_unit,
        ));
    }
    foreach ($setup_fees as $setup_fee) {
      $form['#plans'][$plan->plan_code]['setup_amounts'][$unit_amount->currencyCode] = recurly_format_currency($setup_fee->amount_in_cents, $setup_fee->currencyCode);
    }
    $form['weights'][$plan->plan_code] = array(
      '#type' => 'hidden',
      '#default_value' => $count,
      '#attributes' => array('class' => array('weight')),
    );
    $count++;
  }

  // Order our plans based on any existing value.
  $existing_plans = variable_get('recurly_subscription_plans', array());
  $plan_list = array();
  foreach ($existing_plans as $plan_code => $enabled) {
    if (isset($form['#plans'][$plan_code])) {
      $plan_list[$plan_code] = $form['#plans'][$plan_code];
    }
  }
  // Then add any new plans to the end.
  $plan_list += is_array($form['#plans']) ? $form['#plans'] : array();
  $form['#plans'] = $plan_list;

  foreach ($form['#plans'] as $plan_id => $details) {
    $operations = array();

    // Add an edit link if available for the current user.
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => recurly_subscription_plan_edit_url($details['plan']),
    );

    // Add a purchase link if Hosted Payment Pages are enabled.
    if (module_exists('recurly_hosted')) {
      $operations['purchase'] = array(
        'title' => t('purchase'),
        'href' => recurly_hosted_subscription_plan_purchase_url($details['plan']->plan_code),
      );
    }

    $form['#plans'][$plan_id]['operations'] = array(
      '#theme' => 'links',
      '#links' => $operations,
      '#attributes' => array(
        'class' => array('links', 'inline'),
      ),
    );
  }

  $form['recurly_subscription_plans'] = array(
    '#type' => 'checkboxes',
    '#options' => $plan_options,
    '#default_value' => array_intersect_key($existing_plans, $plan_options),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update plans'),
  );

  return $form;
}

/**
 * Submit handler for recurly_subscription_plans_form().
 */
function recurly_subscription_plans_form_submit($form, &$form_state) {
  // Order our variable based on the form order.
  $recurly_subscription_plans = array();
  foreach ($form_state['input']['weights'] as $plan_code => $weight) {
    if (isset($form_state['values']['recurly_subscription_plans'][$plan_code])) {
      $recurly_subscription_plans[$plan_code] = $form_state['values']['recurly_subscription_plans'][$plan_code];
    }
  }

  // Note that we don't actually need to care able the "weight" field values,
  // since the order of POST is actually changed based on the field position.
  variable_set('recurly_subscription_plans', $recurly_subscription_plans);

  drupal_set_message(t('Status and order of subscription plans updated!'));
}

/**
 * Output the themed version of the recurly_subscription_plans_form().
 */
function theme_recurly_subscription_plans_form($variables) {
  $form = $variables['form'];

  // Format the plan data into a table for display.
  $header = array(
    array('data' => t('Subscription plan'), 'colspan' => 2),
    t('Weight'),
    t('Price'),
    t('Setup fee'),
    t('Trial'),
    t('Operations'),
  );
  $rows = array();

  foreach ($form['#plans'] as $plan_code => $details) {
    $plan = $details['plan'];

    $description = '';

    // Prepare the description string if one is given for the plan.
    if (!empty($plan->description)) {
      $description = '<div class="description">' . nl2br(check_plain($plan->description)) . '</div>';
    }

    $form['recurly_subscription_plans'][$plan_code]['#title_display'] = 'none';

    $row = array();
    $row[] = array('class' => array('checkbox'), 'data' => drupal_render($form['recurly_subscription_plans'][$plan_code]));
    $row[] = check_plain($plan->name) . ' <small>(' . check_plain($plan_code) . ')</small>' . $description;
    $row[] = drupal_render($form['weights'][$plan_code]);
    $row[] = implode('<br />', $details['unit_amounts']);
    $row[] = implode('<br />', $details['setup_amounts']);
    $row[] = $plan->trial_interval_length ? t('@trial_length @trial_unit', array('@trial_length' => $plan->trial_interval_length, '@trial_unit' => $plan->trial_interval_unit)) : t('No trial');
    $row[] = drupal_render($details['operations']);
    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No subscription plans found. You can start by creating one in <a href="!url">your Recurly account</a>.', array('!url' => variable_get('recurly_subdomain', '') ? recurly_hosted_url('plans') : 'http://app.recurly.com')),
        'colspan' => 7,
      ),
    );
  }

  drupal_add_tabledrag('recurly-subscription-plans', 'order', 'sibling', 'weight');

  // Add a few settings to hide tabledrag annoyances.
  $js = "Drupal.theme.prototype.tableDragChangedMarker = function () { return ''; };
Drupal.theme.prototype.tableDragChangedWarning = function () { return ''; };";
  drupal_add_js($js, 'inline');

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'recurly-subscription-plans'),
    ));
  $output .= drupal_render_children($form);
  return $output;
}
