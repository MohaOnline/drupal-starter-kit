<?php

/**
 * @file
 * Views callbacks for Commerce funds module.
 */

/**
 * Implements hook_views_data().
 */
function commerce_funds_views_data() {

  $data['commerce_funds_transactions'] = array(
    'table' => array(
      'group' => t('Commerce funds'),
      'base' => array(
         // Define this as a base table.
        'field' => 'transaction_id',
        'title' => t('Transactions'),
        'help' => t('Transactions from commerce funds.'),
        'weight' => -10,
      ),
      'join' => array(
        'users' => array(
          // The primary key in the referenced table.
          'left_field' => 'uid',
          // The foreign key in this table.
          'field' => array('uid', 'recipient'),
        ),
      ),
    ),
    // Describe table fields to views.
    'transaction_id' => array(
      'title' => t('Transaction ID'),
      'help' => t('The unique transaction ID.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    ),
    'issuer' => array(
      'title' => t('Issuer uid'),
      'help' => t('The uid of the issuer of the transaction.'),
      'relationship' => array(
        'base' => 'users',
        'base field' => 'uid',
        'handler' => 'views_handler_relationship',
        'label' => t('The user who issued the transaction.'),
        'title' => t('Issuer of the transaction.'),
        'help' => t('Display user informations of the issuer of the transaction.'),
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    ),
    'recipient' => array(
      'title' => t('Recipient uid'),
      'help' => t('The uid of the recipient of the transaction.'),
      'relationship' => array(
        'base' => 'users',
        'base field' => 'uid',
        'handler' => 'views_handler_relationship',
        'label' => t('The user who received the transaction.'),
        'title' => t('Recipient of the transaction.'),
        'help' => t('Display user informations of the recipient of the transaction.'),
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    ),
    'created' => array(
      'title' => t('Transaction date'),
      'help' => t('Unix timestamp of the transaction.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'brut_amount' => array(
      'title' => t('Brut amount'),
      'help' => t('The transfered amount.'),
      'field' => array(
        'handler' => 'CommerceFundsHandlerFieldMoneyAmount',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'commission' => array(
      'title' => t('Commission'),
      'help' => t('Commission applied to the transaction.'),
      'field' => array(
        'handler' => 'CommerceFundsHandlerFieldMoneyAmount',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'net_amount' => array(
      'title' => t('Net amount'),
      'help' => t('Total amount of the transaction.'),
      'field' => array(
        'handler' => 'CommerceFundsHandlerFieldMoneyAmount',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'type' => array(
      'title' => t('Type'),
      'help' => t('The type of the transaction.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    'status' => array(
      'title' => t('Transaction status'),
      'help' => t('Status of the transaction.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    'notes' => array(
      'title' => t('Note'),
      'help' => t('Annotation left by the issuer.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    'operations' => array(
      'title' => t('Escrow payment operations'),
      'help' => t('Available operations for the escrow payment'),
      'field' => array(
        'handler' => 'CommerceFundsHandlerFieldEscrowOperations',
        'no group by' => TRUE,
      ),
    ),
  );

  $data['commerce_funds_withdrawal_requests'] = array(
    'table' => array(
      'group' => t('Commerce funds'),
      'base' => array(
         // Define this as a base table.
        'field' => 'request_id',
        'title' => t('Withdrawal requests'),
        'help' => t('Withdrawal requests from commerce funds.'),
        'weight' => -10,
      ),
      'join' => array(
        'users' => array(
          // The primary key in the referenced table.
          'left_field' => 'uid',
          // The foreign key in this table.
          'field' => 'uid',
        ),
      ),
    ),
    // Describe table fields to views.
    'request_id' => array(
      'title' => t('Request ID'),
      'help' => t('The unique request ID.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    ),
    'issuer' => array(
      'title' => t('Withdrawal requester uid'),
      'help' => t('The uid of the requester of withdrawal.'),
      'relationship' => array(
        'base' => 'users',
        'base field' => 'uid',
        'handler' => 'views_handler_relationship',
        'label' => t('The user who request a withdrawal.'),
        'title' => t('Requester of the withdrawal.'),
        'help' => t('Display user informations of the requester of the withdrawal.'),
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    ),
    'created' => array(
      'title' => t('Withdrawal transaction date'),
      'help' => t('Unix timestamp of the transaction.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'method' => array(
      'title' => t('Withdrawal method'),
      'help' => t('The withdrawal method asked by the user.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    'brut_amount' => array(
      'title' => t('Withdrawal requested'),
      'help' => t('The withdrawal requested amount.'),
      'field' => array(
        'handler' => 'CommerceFundsHandlerFieldMoneyAmount',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'net_amount' => array(
      'title' => t('Withdrawal total amount'),
      'help' => t('The cost of the withdrawal request.'),
      'field' => array(
        'handler' => 'CommerceFundsHandlerFieldMoneyAmount',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'commission' => array(
      'title' => t('Withdrawal commission'),
      'help' => t('Commission applied to the withdraw.'),
      'field' => array(
        'handler' => 'CommerceFundsHandlerFieldMoneyAmount',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'status' => array(
      'title' => t('Withdrawal status'),
      'help' => t('Status of the withdrawal.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    'reason' => array(
      'title' => t('Withdrawal declined reason'),
      'help' => t('Annotation left by admin for declining request.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    'operations' => array(
      'title' => t('Withdrawal operations'),
      'help' => t('Available operations for the withdrawal'),
      'field' => array(
        'handler' => 'CommerceFundsHandlerFieldWithdrawalOperations',
        'no group by' => TRUE,
      ),
    ),
  );

  $data['commerce_funds_user_funds'] = array(
    'table' => array(
      'group' => t('Commerce funds'),
      'base' => array(
         // Define this as a base table.
        'field' => 'uid',
        'title' => t('User balances'),
        'help' => t('The balances of the users.'),
        'weight' => -10,
      ),
      'join' => array(
        'users' => array(
          // The primary key in the referenced table.
          'left_field' => 'uid',
          // The foreign key in this table.
          'field' => 'uid',
        ),
      ),
    ),
    'balance' => array(
      'title' => t('User balance'),
      'help' => t('The balance of the user.'),
      'field' => array(
        'handler' => 'CommerceFundsHandlerFieldMoneyAmount',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_default_views().
 */
function commerce_funds_views_default_views() {
  $views = array();
  $path = drupal_get_path('module', 'commerce_funds') . '/views/*.view.inc';

  foreach (glob($path) as $file) {
    require_once $file;
    $views[$view->name] = $view;
    unset($view);
  }

  return $views;
}
