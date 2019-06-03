<?php

/**
 * @file
 * Commerce funds pages.
 */

 /**
  * Form constructor for the admin fees configuration.
  *
  * @param array $enabled_methods
  *   The enabled methods for transactions.
  *
  * @see commerce_funds_configure_fees_validate()
  * @see commerce_funds_configure_fees_submit()
  *
  * @ingroup forms
  */
function commerce_funds_configure_fees($form, &$form_state, array $enabled_methods) {

  $form['#enabledmethods'] = $enabled_methods;

  $commissions = variable_get('commerce_funds_commissions', array());

  $form['commerce_funds'] = array(
    '#type' => 'fieldset',
    '#title' => t('Commerce funds'),
    '#collapsible' => FALSE,
  );

  $form['commerce_funds']['transfer_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Transfer Fee (%)'),
    '#description' => t('Commission rate taken on Transfers'),
    '#default_value' => array_key_exists('transfer_rate', $commissions) ? $commissions['transfer_rate'] / 100 : '0',
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
  );

  $form['commerce_funds']['transfer_fixed'] = array(
    '#type' => 'textfield',
    '#title' => t('Fixed Transfer Fee (@currency)', array('@currency' => commerce_default_currency())),
    '#description' => t('Fixed fee taken (or minimum applied if a rate is defined) on Transfers.'),
    '#default_value' => array_key_exists('transfer_fixed', $commissions) ? $commissions['transfer_fixed'] / 100 : '0',
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
  );

  $form['commerce_funds']['escrow_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Escrow Fee (%)'),
    '#description' => t('Commission rate taken on Escrows'),
    '#default_value' => array_key_exists('escrow_rate', $commissions) ? $commissions['escrow_rate'] / 100 : '0',
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
  );

  $form['commerce_funds']['escrow_fixed'] = array(
    '#type' => 'textfield',
    '#title' => t('Fixed Escrow Fee (@currency)', array('@currency' => commerce_default_currency())),
    '#description' => t('Fixed fee taken (or minimum applied if a rate is defined) on Escrows.'),
    '#default_value' => array_key_exists('escrow_fixed', $commissions) ? $commissions['escrow_fixed'] / 100 : '0',
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
  );

  $form['deposit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Deposit Fees'),
    '#collapsible' => FALSE,
  );

  if ($enabled_methods) {
    $enabled_methods_num = count($enabled_methods);
    foreach ($enabled_methods as $method) {
      $method_id = $method['method_id'];
      $form['deposit'][$method_id . '_rate'] = array(
        '#type' => 'textfield',
        '#title' => t('@method Fee (%)', array('@method' => $method['title'])),
        '#description' => t('Fee taken for Deposits made using @method', array('@method' => $method['title'])),
        '#default_value' => array_key_exists($method_id . '_rate', $commissions) ? $commissions[$method_id . '_rate'] / 100 : '0',
        '#size' => 5,
        '#maxlength' => 5,
        '#required' => TRUE,
      );

      $form['deposit'][$method_id . '_fixed'] = array(
        '#type' => 'textfield',
        '#title' => t('Fixed @method Fee (@currency)',
          array(
            '@method' => $method['title'],
            '@currency' => commerce_default_currency(),
          )
        ),
        '#description' => t('Fixed fee taken (or minimum applied if a rate is defined) for Deposits made using @method', array('@method' => $method['title'])),
        '#default_value' => array_key_exists($method_id . '_fixed', $commissions) ? $commissions[$method_id . '_fixed'] / 100 : '0',
        '#size' => 5,
        '#maxlength' => 5,
        '#required' => TRUE,
      );
    }
    if ($method_id == "free_order" && $enabled_methods_num <= 1) {
      $form['deposit']['no_methods'] = array(
        '#markup' => t('No payment method have been enabled for deposits.'),
      );
    }
  }
  else {
    $form['deposit']['no_methods'] = array(
      '#markup' => t('No payment method have been enabled for deposits.'),
    );
  }

  $form['withdraw'] = array(
    '#type' => 'fieldset',
    '#title' => t('Withdrawal Fees'),
    '#collapsible' => FALSE,
  );

  $enabled_methods = _commerce_funds_get_enabled_withdrawal_methods();

  if ($enabled_methods) {
    foreach ($enabled_methods as $key => $method) {
      $method_id = $key;
      $form['withdraw'][$method_id . '_rate'] = array(
        '#type' => 'textfield',
        '#title' => t('@method Fee (%)', array('@method' => $method)),
        '#description' => t('Fee taken for Withdrawals made using @method', array('@method' => $method)),
        '#default_value' => array_key_exists($method_id . '_rate', $commissions) ? $commissions[$method_id . '_rate'] / 100 : 0,
        '#size' => 5,
        '#maxlength' => 5,
        '#required' => TRUE,
      );

      $form['withdraw'][$method_id . '_fixed'] = array(
        '#type' => 'textfield',
        '#title' => t('Fixed @method Fee (@currency)',
          array(
            '@method' => $method,
            '@currency' => commerce_default_currency(),
          )
        ),
        '#description' => t('Fixed fee taken (or minimum applied if a rate is defined) for Withdrawals made using @method', array('@method' => $method)),
        '#default_value' => array_key_exists($method_id . '_fixed', $commissions) ? $commissions[$method_id . '_fixed'] / 100 : 0,
        '#size' => 5,
        '#maxlength' => 5,
        '#required' => TRUE,
      );
    }
  }
  else {
    $form['withdraw']['no_methods'] = array(
      '#markup' => t('No withdrawal method have been enabled.'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form constructor for deposit funds.
 *
 * @see commerce_funds_deposit_funds_validate()
 * @see commerce_funds_deposit_funds_submit()
 *
 * @ingroup forms
 */
function commerce_funds_deposit_funds($form, &$form_state) {

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Deposit Amount (@currency)',
      array('@currency' => commerce_default_currency())
    ),
    '#description' => t('Please enter the amount you wish to deposit in @currency',
      array('@currency' => commerce_default_currency())
    ),
    '#default_value' => '',
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Form constructor for admin withrawal methods configuration.
 *
 * @see commerce_funds_configure_withdraw_methods_submit()
 *
 * @ingroup forms
 */
function commerce_funds_configure_withdraw_methods($form, &$form_state) {

  $methods = array(
    'paypal' => 'Paypal',
    'skrill' => 'Skrill',
    'bank_account' => 'Bank Account',
    'check' => 'Check',
  );

  $values = variable_get('commerce_funds_withdrawal_methods', array());

  $form['methods'] = array(
    '#type' => 'checkboxes',
    '#options' => $methods,
    '#default_value' => $values,
    '#title' => t('Choose Payment methods allowed for withdrawals'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configurations'),
  );

  return $form;
}

/**
 * Form constructor for paypal configuration.
 *
 * @see commerce_funds_manage_withdrawal_method_paypal_submit()
 *
 * @ingroup forms
 */
function commerce_funds_manage_withdrawal_method_paypal($form, &$form_state) {

  $enabled_methods = variable_get('commerce_funds_withdrawal_methods', array());

  if (!$enabled_methods['paypal']) {
    return FALSE;
  }

  global $user;

  $form['paypal_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Paypal Email'),
    '#description' => t('Withdrawals using Paypal will be sent to this email'),
    '#default_value' => $user->data && array_key_exists('paypal', $user->data) ? $user->data['paypal']['paypal_email'] : '',
    '#size' => 40,
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form constructor for skrill configuration.
 *
 * @see commerce_funds_manage_withdrawal_method_skrill_submit()
 *
 * @ingroup forms
 */
function commerce_funds_manage_withdrawal_method_skrill($form, &$form_state) {

  $enabled_methods = variable_get('commerce_funds_withdrawal_methods', array());

  if (!$enabled_methods['skrill']) {
    return FALSE;
  }

  global $user;

  $form['skrill_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Skrill Email'),
    '#description' => t('Withdrawals using Skrill will be sent to this email'),
    '#default_value' => $user->data && array_key_exists('skrill', $user->data) ? $user->data['skrill']['skrill_email'] : '',
    '#size' => 40,
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form constructor for bank account configuration.
 *
 * @see commerce_funds_manage_withdrawal_method_bank_account_submit()
 *
 * @ingroup forms
 */
function commerce_funds_manage_withdrawal_method_bank_account($form, &$form_state) {

  include_once DRUPAL_ROOT . '/includes/locale.inc';

  $enabled_methods = variable_get('commerce_funds_withdrawal_methods', array());

  if (!$enabled_methods['bank_account']) {
    return FALSE;
  }

  global $user;

  $form['account_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of Account Holder'),
    '#description' => '',
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['account_name'] : '',
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['account_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Number / IBAN'),
    '#description' => '',
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['account_number'] : '',
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['bank_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank Name'),
    '#description' => '',
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['bank_name'] : '',
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['bank_country'] = array(
    '#type' => 'select',
    '#title' => t('Bank Country'),
    '#options' => country_get_list(),
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['bank_country'] : '',
    '#description' => '',
    '#required' => TRUE,
  );

  $form['swift_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Swift Code'),
    '#description' => '',
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['swift_code'] : '',
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['bank_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank Address'),
    '#description' => '',
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['bank_address'] : '',
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['bank_address2'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank Address 2'),
    '#description' => '',
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['bank_address2'] : '',
    '#size' => 40,
    '#maxlength' => 128,
  );

  $form['bank_city'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank City'),
    '#description' => '',
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['bank_city'] : '',
    '#size' => 20,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['bank_province'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank Province'),
    '#description' => '',
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['bank_province'] : '',
    '#size' => 20,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['bank_postalcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank Postal Code'),
    '#description' => '',
    '#default_value' => $user->data && array_key_exists('bank_account', $user->data) ? $user->data['bank_account']['bank_postalcode'] : '',
    '#size' => 20,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form constructor for check configuration.
 *
 * @see commerce_funds_manage_withdrawal_method_check_submit()
 *
 * @ingroup forms
 */
function commerce_funds_manage_withdrawal_method_check($form, &$form_state) {

  $enabled_methods = variable_get('commerce_funds_withdrawal_methods', array());

  if (!$enabled_methods['check']) {
    return FALSE;
  }

  global $user;

  $form['check_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Full Name'),
    '#description' => t('Full Name to write the Check to'),
    '#default_value' => $user->data && array_key_exists('check', $user->data) ? $user->data['check']['check_name'] : '',
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['check_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#description' => t('Detailed address to send the check to'),
    '#default_value' => $user->data && array_key_exists('check', $user->data) ? $user->data['check']['check_address'] : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['check_address2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 2'),
    '#description' => t('Detailed address to send the check to'),
    '#default_value' => $user->data && array_key_exists('check', $user->data) ? $user->data['check']['check_address2'] : '',
    '#size' => 60,
    '#maxlength' => 128,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form constructor for withdrawing funds.
 *
 * @param array $methods
 *   Enabled and configured methods.
 *
 * @see commerce_funds_withdraw_funds_validate()
 * @see commerce_funds_withdraw_funds_submit()
 *
 * @ingroup forms
 */
function commerce_funds_withdraw_funds($form, &$form_state, array $methods) {

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount to withdraw (@currency)',
     array('@currency' => commerce_default_currency())),
    '#description' => t('Please enter the amount you wish to withdraw in @currency',
     array('@currency' => commerce_default_currency())),
    '#default_value' => '',
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['methods'] = array(
    '#type' => 'radios',
    '#options' => $methods,
    '#title' => t('Choose a Withdrawal method'),
    '#default_value' => '',
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Request'),
  );

  return $form;
}

/**
 * Form constructor for transfering funds.
 *
 * @param string $commission_description
 *   Message to display the commission applied.
 *
 * @see commerce_funds_transfer_funds_validate()
 * @see commerce_funds_transfer_funds_submit()
 *
 * @ingroup forms
 */
function commerce_funds_transfer_funds($form, &$form_state, $commission_description) {

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount to transfer (@currency)',
     array(
       '@currency' => commerce_default_currency(),
     )),
    '#description' => $commission_description,
    '#default_value' => '',
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['username'] = array(
    '#id' => 'uc-funds-transfer-to',
    '#type' => 'textfield',
    '#title' => t('Transfer To'),
    '#description' => t('Please enter the username of the user you want to transfer the funds to'),
    '#default_value' => '',
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => t('Eventually add a message to the recipient.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Transfer Funds'),
  );

  return $form;
}

/**
 * Form constructor for making an escrow payment.
 *
 * @param string $commission_description
 *   Message to display the commission applied.
 *
 * @see commerce_funds_escrow_payment_validate()
 * @see commerce_funds_escrow_payment_submit()
 *
 * @ingroup forms
 */
function commerce_funds_escrow_payment($form, &$form_state, $commission_description) {

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount to put in Escrow (@currency)',
     array('@currency' => commerce_default_currency())),
    '#description' => $commission_description,
    '#default_value' => '',
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['username'] = array(
    '#id' => 'uc-funds-escrow-to',
    '#type' => 'textfield',
    '#title' => t('Allocated To'),
    '#description' => t('Please enter the username of the user you want to allocate the escrow to'),
    '#default_value' => '',
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => t('Eventually add a message to the recipient.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Allocate Funds'),
  );

  return $form;
}

/**
 * Form constructor for approving a withdrawal request.
 *
 * @param int $request_id
 *   The id of the request.
 *
 * @see commerce_funds_approve_withdrawal_request_submit()
 *
 * @ingroup forms
 */
function commerce_funds_approve_withdrawal_request($form, &$form_state, $request_id) {

  $form['#request_id'] = $request_id;

  $request = entity_load_single('commerce_funds_withdrawal_request', $request_id);

  if ($request->status == 'Pending approval') {
    $user = user_load($request->issuer);
    $path = 'admin/commerce/funds/view-withdraw-requests';
    $message = t('Are you sure you want to approve the withdrawal request made by @username ?', array('@username' => $user->name));
    return confirm_form($form, t('Approve Request?'), $path, $message, t('Yes'));
  }
  else {
    drupal_not_found();
    drupal_exit();
  }
}

/**
 * Form constructor for declining a withdrawal request.
 *
 * @param int $request_id
 *   The id of the request.
 *
 * @see commerce_funds_decline_withdrawal_request_submit()
 *
 * @ingroup forms
 */
function commerce_funds_decline_withdrawal_request($form, &$form_state, $request_id) {

  $form['#request_id'] = $request_id;

  $form['reason'] = array(
    '#type' => 'textarea',
    '#title' => t('Reason for Decline'),
    '#description' => '',
  );

  $request = entity_load_single('commerce_funds_withdrawal_request', $request_id);

  if ($request->status == 'Pending approval') {
    $user = user_load($request->issuer);
    $path = 'admin/commerce/funds/view-withdraw-requests';
    $message = t('Are you sure you want to decline the withdrawal request made by @username ?', array('@username' => $user->name));
    return confirm_form($form, t('Decline Request?'), $path, $message, t('Yes'));
  }
  else {
    drupal_not_found();
    drupal_exit();
  }
}
