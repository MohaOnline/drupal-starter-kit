<?php

/**
 * @file
 * Hooks and functions for Commerce funds rules integration.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_funds_rules_event_info() {
  $items = array(
    'commerce_funds_funds_transfered' => array(
      'label' => t('Funds transfered'),
      'group' => t('Commerce funds'),
      'variables' => array(
        'issuer' => array(
          'type' => 'user',
          'label' => t('Issuer of the transaction'),
        ),
        'recipient' => array(
          'type' => 'user',
          'label' => t('Recipient of the transaction'),
        ),
        'transaction' => array(
          'type' => 'transaction',
          'label' => t('The transaction informations'),
        ),
      ),
    ),
    'commerce_funds_escrow_created' => array(
      'label' => t('Escrow payment created'),
      'group' => t('Commerce funds'),
      'variables' => array(
        'issuer' => array(
          'type' => 'user',
          'label' => t('Issuer of the transaction'),
        ),
        'recipient' => array(
          'type' => 'user',
          'label' => t('Recipient of the transaction'),
        ),
        'transaction' => array(
          'type' => 'transaction',
          'label' => t('The transaction informations'),
        ),
      ),
    ),
    'commerce_funds_escrow_cancelled' => array(
      'label' => t('Escrow payment cancelled'),
      'group' => t('Commerce funds'),
      'variables' => array(
        'issuer' => array(
          'type' => 'user',
          'label' => t('Issuer of the transaction'),
        ),
        'recipient' => array(
          'type' => 'user',
          'label' => t('Recipient of the transaction'),
        ),
        'transaction' => array(
          'type' => 'transaction',
          'label' => t('The transaction informations'),
        ),
      ),
    ),
    'commerce_funds_escrow_released' => array(
      'label' => t('Escrow payment released'),
      'group' => t('Commerce funds'),
      'variables' => array(
        'issuer' => array(
          'type' => 'user',
          'label' => t('Issuer of the transaction'),
        ),
        'recipient' => array(
          'type' => 'user',
          'label' => t('Recipient of the transaction'),
        ),
        'transaction' => array(
          'type' => 'transaction',
          'label' => t('The transaction informations'),
        ),
      ),
    ),
    'commerce_funds_withrawal_request_validated' => array(
      'label' => t('Withdrawal request accepted'),
      'group' => t('Commerce funds'),
      'variables' => array(
        'issuer' => array(
          'type' => 'user',
          'label' => t('Issuer of the request'),
        ),
        'request' => array(
          'type' => 'withdrawal_request',
          'label' => t('The withrawal request informations'),
        ),
      ),
    ),
    'commerce_funds_withrawal_request_declined' => array(
      'label' => t('Withdrawal request declined'),
      'group' => t('Commerce funds'),
      'variables' => array(
        'issuer' => array(
          'type' => 'user',
          'label' => t('Issuer of the request'),
        ),
        'request' => array(
          'type' => 'withdrawal_request',
          'label' => t('The withdrawal request informations'),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_funds_rules_action_info() {
  $actions = array();

  $actions['commerce_funds_update_balance'] = array(
    'label' => t('Update user account balance'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Deposit Order'),
      ),
    ),
    'group' => t('Commerce funds'),
    'callbacks' => array(
      'execute' => '_commerce_funds_rules_update_account_balance',
    ),
  );

  return $actions;
}

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_funds_default_rules_configuration() {

  $rules = array();

  $rule = rules_reaction_rule();
  $rule->label = t('Adds funds to balance when deposit is completed');
  $rule->tags = array('Commerce funds');
  $rule->active = TRUE;
  $rule->event('commerce_checkout_complete')
    ->condition('commerce_order_contains_product_type', array(
      'commerce_order:select' => 'commerce_order',
      'product_type' => array('value' => 'deposit'),
      'operator' => '=',
      'value' => '1',
    )
  )
    ->action(
      'commerce_funds_update_balance', array(
        'commerce-order:select' => 'commerce-order',
      )
    );

  $rules['commerce_funds_add_funds_to_balance'] = $rule;

  unset($rule);

  // A transfer has been made.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify recipient by email of a transfer.');
  $rule->tags = array('Commerce funds');
  $rule->active = TRUE;
  $rule->event('commerce_funds_funds_transfered')
    ->action('mail', array(
      'to' => '[recipient:mail]',
      'subject' => 'Transfer from [issuer:name]',
      'message' => '[issuer:name] has transfered [transaction:brut-amount] to your account with the following message : [transaction:notes]',
      'from' => '[site:mail]',
      'language' => LANGUAGE_NONE,
    )
  );

  $rules['commerce_funds_new_transfer_notification'] = $rule;

  // Escrow payment has been created.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify users by email of a new incoming escrow payment.');
  $rule->tags = array('Commerce funds');
  $rule->active = TRUE;
  $rule->event('commerce_funds_escrow_created')
    ->action('mail', array(
      'to' => '[issuer:mail]',
      'subject' => 'Escrow payment to [recipient:name]',
      'message' => 'Your escrow payment of [transaction:brut-amount] has been taken in count. [recipient:name] has been notified by email of your escrow.',
      'from' => '[site:mail]',
      'language' => LANGUAGE_NONE,
    )
  )
    ->action('mail', array(
      'to' => '[recipient:mail]',
      'subject' => 'Escrow payment from [issuer:name]',
      'message' => 'An escrow payment of [transaction:brut-amount] has been made to your account from [issuer:name].',
      'from' => '[site:mail]',
      'language' => LANGUAGE_NONE,
    )
  );

  $rules['commerce_funds_new_escrow_notification'] = $rule;

  unset($rule);

  // Escrow payment has been cancelled.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify user by email of the cancellation of an escrow payment.');
  $rule->tags = array('Commerce funds');
  $rule->active = TRUE;
  $rule->event('commerce_funds_escrow_cancelled')
    ->action('mail', array(
      'to' => '[issuer:mail]',
      'subject' => 'Escrow payment cancelled (to [recipient:name])',
      'message' => 'Your escrow payment of [transaction:brut-amount] to [recipient:name] has been cancelled.',
      'from' => '[site:mail]',
      'language' => LANGUAGE_NONE,
    )
  )
    ->action('mail', array(
      'to' => '[recipient:mail]',
      'subject' => 'Escrow payment cancelled (from [issuer:name])',
      'message' => 'The escrow payment of [transaction:brut-amount] from [issuer:name] has been cancelled.',
      'from' => '[site:mail]',
      'language' => LANGUAGE_NONE,
    )
  );

  $rules['commerce_funds_escrow_cancelled_notification'] = $rule;

  unset($rule);

  // Escrow payment has been released.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify users by email that an escrow payment has been released.');
  $rule->tags = array('Commerce funds');
  $rule->active = TRUE;
  $rule->event('commerce_funds_escrow_released')
    ->action('mail', array(
      'to' => '[issuer:mail]',
      'subject' => 'Escrow payment released (to [recipient:name])',
      'message' => 'Your escrow payment of [transaction:brut-amount] to [recipient:name] has been successfully released. [recipient:name] has been notified by email.',
      'from' => '[site:mail]',
      'language' => LANGUAGE_NONE,
    )
  )
    ->action('mail', array(
      'to' => '[recipient:mail]',
      'subject' => 'Escrow payment released (from [issuer:name])',
      'message' => 'The escrow payment of [transaction:brut-amount] from [issuer:name] has been released. Your account balance has been updated.',
      'from' => '[site:mail]',
      'language' => LANGUAGE_NONE,
    )
  );

  $rules['commerce_funds_escrow_released_notification'] = $rule;

  unset($rule);

  // A withdrawal request has been validated.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify user by email that a withdrawal request has been accepted.');
  $rule->tags = array('Commerce funds');
  $rule->active = TRUE;
  $rule->event('commerce_funds_withrawal_request_validated')
    ->action('mail', array(
      'to' => '[issuer:mail]',
      'subject' => 'Withdrawal request validated',
      'message' => 'Your withdrawal request of [request:brut-amount] issued on [request:date] has been validated and will be processed in due order.',
      'from' => '[site:mail]',
      'language' => LANGUAGE_NONE,
    )
  );

  $rules['commerce_funds_withrawal_request_validated_notification'] = $rule;

  unset($rule);

  // A withdrawal request has been declined.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify user by email that a withdrawal request has been declined.');
  $rule->tags = array('Commerce funds');
  $rule->active = TRUE;
  $rule->event('commerce_funds_withrawal_request_declined')
    ->action('mail', array(
      'to' => '[issuer:mail]',
      'subject' => 'Withdrawal request declined',
      'message' => 'Your withdrawal request of [request:brut-amount] issued on [request:date] has been declined.
      Reason (if any) : [request:reason].',
      'from' => '[site:mail]',
      'language' => LANGUAGE_NONE,
    )
  );

  $rules['commerce_funds_withrawal_request_declined_notification'] = $rule;

  return $rules;
}

/**
 * Update order owner balance account.
 *
 * @see commerce_funds_rules_action_info()
 */
function _commerce_funds_rules_update_account_balance($order) {

  $commissions = variable_get('commerce_funds_commissions', array());

  $payment_method = explode('|', $order->data['payment_method']);

  $commission_rate = array_key_exists($payment_method[0] . '_rate', $commissions) ? $commissions[$payment_method[0] . '_rate'] : 0;
  $commission_fixed = array_key_exists($payment_method[0] . '_fixed', $commissions) ? $commissions[$payment_method[0] . '_fixed'] : 0;

  $total_order = 0;
  $line_items = $order->commerce_line_items[LANGUAGE_NONE];
  // Loop through the line items so you find the deposit
  foreach ($line_items as $delta => $line_item_wrapper) {
    $line_item_id = $line_item_wrapper['line_item_id'];
    $line_item = commerce_line_item_load($line_item_id);
    if (isset($line_item->commerce_product) && $line_item->type == "product") {
      if (strpos($line_item->line_item_label, "deposit_", 0) !== false) {
        $commerce_total = intval($line_item->commerce_total[LANGUAGE_NONE][0]['amount']);
        $total_order += $commerce_total;
      }
    }
  }

  $deposit_after_commission_rate = $total_order * (1 - $commission_rate / 10000);
  $deposit_after_commission_fixed = $total_order - $commission_fixed;
  $deposit_after_commission = min(array($deposit_after_commission_rate, $deposit_after_commission_fixed));
  $commission = $total_order - $deposit_after_commission;

  $user_balance = new stdClass();
  $user_balance->uid = $order->uid;
  $user_balance->balance = $deposit_after_commission;

  $site_balance = new stdClass();
  $site_balance->uid = 1;
  $site_balance->balance = $commission;

  $transaction = new stdClass();
  $transaction->issuer = $order->uid;
  $transaction->recipient = $order->uid;
  $transaction->type = 'Deposit';
  $transaction->created = $order->created;
  $transaction->brut_amount = $total_order;
  $transaction->net_amount = $deposit_after_commission;
  $transaction->commission = $commission;
  $transaction->status = 'Completed';
  $transaction->notes = t('Deposit of @amount', array('@amount' => commerce_currency_format($transaction->net_amount, commerce_default_currency())));

  // Update account balance.
  $account_exist = db_query("SELECT * FROM commerce_funds_user_funds WHERE uid = :uid", array(':uid' => $order->uid))->fetchAssoc();

  if (!$account_exist) {
    drupal_write_record('commerce_funds_user_funds', $user_balance);
  }
  else {
    $user_balance->balance += $account_exist['balance'];
    drupal_write_record('commerce_funds_user_funds', $user_balance, 'uid');
  }

  // Update site balance.
  $site_balance = db_query("SELECT * FROM commerce_funds_user_funds WHERE uid = :uid", array(':uid' => 1))->fetchAssoc();
  $site_balance['balance'] += $commission;
  drupal_write_record('commerce_funds_user_funds', $site_balance, 'uid');

  // Write transaction.
  drupal_write_record('commerce_funds_transactions', $transaction);
}
