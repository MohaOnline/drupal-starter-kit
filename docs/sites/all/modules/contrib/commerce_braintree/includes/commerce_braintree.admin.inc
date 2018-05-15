<?php
/**
 * @file
 * Provides admin forms for commerce_braintree.
 */

/**
 * Form callback: allows the user to submit a transaction for settlement.
 */
function commerce_braintree_settle_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $amount = commerce_currency_format($transaction->amount, $transaction->currency_code);

  $default_amount = number_format(commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code), 2, '.', '');

  $form['settlement_message'] = array(
    '#markup' => t('Settling all or part of a transaction will finalize the charge on the customer\'s payment method.')
      . '<br /><strong>' . t('Notice: You can only settle a transaction one time.') . '</strong>' ,
  );

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Settlement amount'),
    '#description' => t('Enter the amount to settle for this transaction.'),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('Settle the payment transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Settle'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: validate the settlement values.
 */
function commerce_braintree_settle_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for settlement.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to settle.'));
  }

  // Ensure the amount is less than or equal to the captured amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot settle more than the original transaction amount.'));
  }
}

/**
 * Submit handler: submit the transaction for settlement.
 */
function commerce_braintree_settle_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  commerce_braintree_initialize($payment_method);

  // Attempt to invoke the Braintree API and process a transaction settlement.
  try {
    $result = Braintree_Transaction::submitForSettlement($transaction->remote_id, $amount);
  }
  catch (Exception $ex) {
    $result = NULL;
  }

  // Inform the user if there were any issues submitting the settlement.
  if (empty($result) || empty($result->success)) {
    $message = !empty($result->message) ? $result->message : t('No error provided from Braintree');
    drupal_set_message(t('The payment transaction could not be submitted for settlement. @message', array('@message' => $message)), 'error');
  }

  // Inform the user of a successful submission for settlement and update the payment transaction.
  if (!empty($result) && !empty($result->success)) {
    drupal_set_message(t('Transaction submitted for settlement successfully.'));
    // Update the payment transaction values to show the settlement.
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = $result->transaction->status;
    $transaction->message .= '<br />' . t('Submitted for settlement: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
    $transaction->amount = commerce_currency_decimal_to_amount($result->transaction->amount, $result->transaction->currencyIsoCode);
    commerce_payment_transaction_save($transaction);
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to void a transaction.
 */
function commerce_braintree_void_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $amount = commerce_currency_format($transaction->amount, $transaction->currency_code);
  $form['markup'] = array(
    '#markup' => '<strong>' . t('Are you sure that you want to void this transaction?') . '</strong>'
      . '<br /><br />'
      . t('Voiding a transaction will prevent the customer from being charged @amount and void the transaction record.', array('@amount' => $amount)),
  );

  $form = confirm_form($form,
    t('Are you sure that you want to void this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Void'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit handler: process the void request.
 */
function commerce_braintree_void_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  commerce_braintree_initialize($payment_method);

  // Attempt to invoke the Braintree API and process a void transaction on the payment.
  try {
    $result = Braintree_Transaction::void($transaction->remote_id);
  }
  catch (Exception $ex) {
    $result = NULL;
  }

  // Inform the user if there were any issues processing the void.
  if (empty($result) || empty($result->success)) {
    $message = !empty($result->message) ? $result->message : t('No error provided from Braintree');
    drupal_set_message(t('The payment transaction cannot be voided. @message', array('@message' => $message)), 'error');
  }

  // Inform the user of a successful void and update the payment transaction.
  if (!empty($result) && !empty($result->success)) {
    drupal_set_message(t('Void completed successfully.'));

    // Update the payment transaction values to show the void.
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->remote_status = 'voided';
    $transaction->message .= '<br />' . t('Voided: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
    commerce_payment_transaction_save($transaction);
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to issue a refund on a prior transaction.
 */
function commerce_braintree_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $default_amount = number_format(commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code), 2, '.', '');

  $form['refund_description'] = array(
    '#markup' => t('Refunding a transaction with return the refund amount to the customer\'s credit card.')
      . '<br />' . t('You can refund any amount greater than 0 and less than or equal to the original transaction.'),
  );

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => t('Enter the amount to be refunded back to the original credit card.'),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to refund?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Refund'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: check the refund amount before attempting refund request.
 */
function commerce_braintree_refund_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for refund.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the captured amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot refund more than the original transaction amount.'));
  }
}

/**
 * Submit handler: process a Braintree transaction refund.
 */
function commerce_braintree_refund_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = number_format($form_state['values']['amount'], 2, '.', '');
  $order = $form_state['order'];
  $payment_method = $form_state['payment_method'];

  commerce_braintree_initialize($payment_method);

  try {
    $result = Braintree_Transaction::refund($transaction->remote_id, $amount);
  }
  catch (Exception $ex) {
    $result = NULL;
  }

  // Inform the user if there were any issues processing the refund.
  if (empty($result) || empty($result->success)) {
    $message = !empty($result->message) ? $result->message : t('No error provided from Braintree');
    drupal_set_message(t('The payment transaction cannot be refunded at this time. @message', array('@message' => $message)), 'error');
  }

  // Inform the user of a successful void and update the payment transaction.
  if (!empty($result) && !empty($result->success)) {
    $refund_amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);
    drupal_set_message(t('Refund for @amount issued successfully', array('@amount' => commerce_currency_format($refund_amount, $transaction->currency_code))));

    // Create a new transaction to record the refund.
    $refund_transaction = commerce_payment_transaction_new($transaction->payment_method, $order->order_id);
    $refund_transaction->instance_id = $payment_method['instance_id'];
    $refund_transaction->remote_id = $result->transaction->id;
    $refund_transaction->amount = $refund_amount * -1;
    $refund_transaction->currency_code = $transaction->currency_code;
    $refund_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $refund_transaction->remote_status = $result->transaction->status;
    $refund_transaction->message = t('Credited to @remote_id.', array('@remote_id' => $transaction->remote_id));

    // Save the refund transaction.
    commerce_payment_transaction_save($refund_transaction);
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}
