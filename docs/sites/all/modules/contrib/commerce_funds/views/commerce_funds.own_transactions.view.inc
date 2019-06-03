<?php

/**
 * @file
 * Views callbacks for "own transactions" views.
 *
 * Provides users all transactions & escrow payments.
 * Commerce funds module.
 */

$view = new view();
$view->name = 'commerce_funds_own_transactions';
$view->description = 'Display incoming and outgoing transactions for users.';
$view->tag = 'Commerce funds';
$view->base_table = 'commerce_funds_transactions';
$view->human_name = 'User transactions';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Your transactions';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view own transactions';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
 'transaction_id' => 'transaction_id',
 'type' => 'type',
 'brut_amount' => 'brut_amount',
 'commission' => 'commission',
 'net_amount' => 'net_amount',
 'created' => 'created',
 'name' => 'name',
 'notes' => 'notes',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
 'transaction_id' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 0,
 ),
 'type' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 0,
 ),
 'brut_amount' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 0,
 ),
 'commission' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 1,
 ),
 'net_amount' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 1,
 ),
 'created' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 0,
 ),
 'name' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 0,
 ),
 'notes' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 0,
 ),
);
$handler->display->display_options['style_options']['empty_table'] = TRUE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Transactions issued';
$handler->display->display_options['header']['area']['empty'] = TRUE;
$handler->display->display_options['header']['area']['content'] = '<h2>Incoming Escrows</h2>';
$handler->display->display_options['header']['area']['format'] = 'full_html';
/* Relationship: Commerce funds: Recipient of the transaction. */
$handler->display->display_options['relationships']['recipient']['id'] = 'recipient';
$handler->display->display_options['relationships']['recipient']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['relationships']['recipient']['field'] = 'recipient';
/* Field: Commerce funds: Transaction ID */
$handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
$handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
$handler->display->display_options['fields']['transaction_id']['label'] = '';
$handler->display->display_options['fields']['transaction_id']['exclude'] = TRUE;
$handler->display->display_options['fields']['transaction_id']['element_label_colon'] = FALSE;
/* Field: Commerce funds: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['type']['field'] = 'type';
/* Field: Commerce funds: Brut amount */
$handler->display->display_options['fields']['brut_amount']['id'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['brut_amount']['field'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['label'] = 'Amount';
/* Field: Commerce funds: Commission */
$handler->display->display_options['fields']['commission']['id'] = 'commission';
$handler->display->display_options['fields']['commission']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['commission']['field'] = 'commission';
/* Field: Commerce funds: Net amount */
$handler->display->display_options['fields']['net_amount']['id'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['net_amount']['field'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['label'] = 'Total';
/* Field: Commerce funds: Transaction date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Date';
$handler->display->display_options['fields']['created']['date_format'] = 'medium';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'recipient';
$handler->display->display_options['fields']['name']['label'] = 'Recipient';
$handler->display->display_options['fields']['name']['empty'] = 'Balance';
/* Field: Commerce funds: Transaction status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = 'Status';
/* Field: Commerce funds: Note */
$handler->display->display_options['fields']['notes']['id'] = 'notes';
$handler->display->display_options['fields']['notes']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['notes']['field'] = 'notes';
$handler->display->display_options['fields']['notes']['label'] = 'Details';
/* Sort criterion: Commerce funds: Transaction date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Contextual filter: Commerce funds: Issuer uid */
$handler->display->display_options['arguments']['issuer']['id'] = 'issuer';
$handler->display->display_options['arguments']['issuer']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['arguments']['issuer']['field'] = 'issuer';
$handler->display->display_options['arguments']['issuer']['default_action'] = 'default';
$handler->display->display_options['arguments']['issuer']['default_argument_type'] = 'current_user';
$handler->display->display_options['arguments']['issuer']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['issuer']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['issuer']['summary_options']['items_per_page'] = '25';

/* Display: All issued transactions (Page) */
$handler = $view->new_display('page', 'All issued transactions (Page)', 'issued_transactions');
$handler->display->display_options['display_description'] = 'All transactions issued by the user.';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Transaction issued';
$handler->display->display_options['header']['area']['content'] = '<h2>Transactions issued</h2>';
$handler->display->display_options['header']['area']['format'] = 'full_html';
$handler->display->display_options['defaults']['empty'] = FALSE;
$handler->display->display_options['path'] = 'user/funds/transactions';

/* Display: All received transactions (Attachment) */
$handler = $view->new_display('attachment', 'All received transactions (Attachment)', 'received_transactions');
$handler->display->display_options['display_description'] = 'All transactions received by the user.';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Transactions received';
$handler->display->display_options['header']['area']['empty'] = TRUE;
$handler->display->display_options['header']['area']['content'] = '<h2>Transactions received</h2>';
$handler->display->display_options['header']['area']['format'] = 'full_html';
$handler->display->display_options['defaults']['empty'] = FALSE;
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Commerce funds: Recipient of the transaction. */
$handler->display->display_options['relationships']['recipient']['id'] = 'recipient';
$handler->display->display_options['relationships']['recipient']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['relationships']['recipient']['field'] = 'recipient';
/* Relationship: Commerce funds: Issuer of the transaction. */
$handler->display->display_options['relationships']['issuer']['id'] = 'issuer';
$handler->display->display_options['relationships']['issuer']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['relationships']['issuer']['field'] = 'issuer';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Commerce funds: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['type']['field'] = 'type';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'recipient';
$handler->display->display_options['fields']['name']['label'] = 'From';
/* Field: Commerce funds: Brut amount */
$handler->display->display_options['fields']['brut_amount']['id'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['brut_amount']['field'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['label'] = 'Amount';
/* Field: Commerce funds: Transaction date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Date';
$handler->display->display_options['fields']['created']['date_format'] = 'medium';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: Commerce funds: Note */
$handler->display->display_options['fields']['notes']['id'] = 'notes';
$handler->display->display_options['fields']['notes']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['notes']['field'] = 'notes';
$handler->display->display_options['fields']['notes']['label'] = 'Details';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Commerce funds: Recipient uid */
$handler->display->display_options['arguments']['recipient']['id'] = 'recipient';
$handler->display->display_options['arguments']['recipient']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['arguments']['recipient']['field'] = 'recipient';
$handler->display->display_options['arguments']['recipient']['default_action'] = 'default';
$handler->display->display_options['arguments']['recipient']['default_argument_type'] = 'current_user';
$handler->display->display_options['arguments']['recipient']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['recipient']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['recipient']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['displays'] = array(
 'issued_transactions' => 'issued_transactions',
 'default' => 0,
 'incoming_escrow_payments' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_arguments'] = FALSE;

/* Display: Incoming escrow payments (Page) */
$handler = $view->new_display('page', 'Incoming escrow payments (Page)', 'incoming_escrow_payments');
$handler->display->display_options['display_description'] = 'User incoming escrow payments.';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Incoming escrows';
$handler->display->display_options['header']['area']['empty'] = TRUE;
$handler->display->display_options['header']['area']['content'] = '<h2>Incoming Escrows</h2>';
$handler->display->display_options['header']['area']['format'] = 'full_html';
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Commerce funds: Issuer of the transaction. */
$handler->display->display_options['relationships']['issuer']['id'] = 'issuer';
$handler->display->display_options['relationships']['issuer']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['relationships']['issuer']['field'] = 'issuer';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Commerce funds: Transaction ID */
$handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
$handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
$handler->display->display_options['fields']['transaction_id']['label'] = '';
$handler->display->display_options['fields']['transaction_id']['exclude'] = TRUE;
$handler->display->display_options['fields']['transaction_id']['element_label_colon'] = FALSE;
/* Field: Commerce funds: Transaction date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Date';
$handler->display->display_options['fields']['created']['date_format'] = 'medium';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: Commerce funds: Brut amount */
$handler->display->display_options['fields']['brut_amount']['id'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['brut_amount']['field'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['label'] = 'Amount';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'issuer';
$handler->display->display_options['fields']['name']['label'] = 'From';
/* Field: Commerce funds: Note */
$handler->display->display_options['fields']['notes']['id'] = 'notes';
$handler->display->display_options['fields']['notes']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['notes']['field'] = 'notes';
$handler->display->display_options['fields']['notes']['label'] = 'Details';
/* Field: Commerce funds: Transaction status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = 'Status';
/* Field: Commerce funds: Escrow payment operations */
$handler->display->display_options['fields']['operations']['id'] = 'operations';
$handler->display->display_options['fields']['operations']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['operations']['field'] = 'operations';
$handler->display->display_options['fields']['operations']['label'] = 'Operations';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Commerce funds: Recipient uid */
$handler->display->display_options['arguments']['recipient']['id'] = 'recipient';
$handler->display->display_options['arguments']['recipient']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['arguments']['recipient']['field'] = 'recipient';
$handler->display->display_options['arguments']['recipient']['default_action'] = 'default';
$handler->display->display_options['arguments']['recipient']['default_argument_type'] = 'current_user';
$handler->display->display_options['arguments']['recipient']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['recipient']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['recipient']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Commerce funds: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = 'Escrow payment';
$handler->display->display_options['path'] = 'user/funds/escrow-payments';

/* Display: Outgoing escrow payments */
$handler = $view->new_display('attachment', 'Outgoing escrow payments', 'outgoing_escrow_payments');
$handler->display->display_options['display_description'] = 'User outgoing escrow payments.';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Outgoing escrows';
$handler->display->display_options['header']['area']['empty'] = TRUE;
$handler->display->display_options['header']['area']['content'] = '<h2>Outgoing Escrows</h2>';
$handler->display->display_options['header']['area']['format'] = 'full_html';
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Commerce funds: Recipient of the transaction. */
$handler->display->display_options['relationships']['recipient']['id'] = 'recipient';
$handler->display->display_options['relationships']['recipient']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['relationships']['recipient']['field'] = 'recipient';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Commerce funds: Transaction ID */
$handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
$handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
$handler->display->display_options['fields']['transaction_id']['label'] = '';
$handler->display->display_options['fields']['transaction_id']['exclude'] = TRUE;
$handler->display->display_options['fields']['transaction_id']['element_label_colon'] = FALSE;
/* Field: Commerce funds: Transaction date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Date';
$handler->display->display_options['fields']['created']['date_format'] = 'medium';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: Commerce funds: Brut amount */
$handler->display->display_options['fields']['brut_amount']['id'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['brut_amount']['field'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['label'] = 'Amount';
/* Field: Commerce funds: Commission */
$handler->display->display_options['fields']['commission']['id'] = 'commission';
$handler->display->display_options['fields']['commission']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['commission']['field'] = 'commission';
/* Field: Commerce funds: Net amount */
$handler->display->display_options['fields']['net_amount']['id'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['net_amount']['field'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['label'] = 'Total';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'recipient';
$handler->display->display_options['fields']['name']['label'] = 'Payment to';
/* Field: Commerce funds: Note */
$handler->display->display_options['fields']['notes']['id'] = 'notes';
$handler->display->display_options['fields']['notes']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['notes']['field'] = 'notes';
$handler->display->display_options['fields']['notes']['label'] = 'Details';
/* Field: Commerce funds: Transaction status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = 'Status';
/* Field: Commerce funds: Escrow payment operations */
$handler->display->display_options['fields']['operations']['id'] = 'operations';
$handler->display->display_options['fields']['operations']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['operations']['field'] = 'operations';
$handler->display->display_options['fields']['operations']['label'] = 'Operations';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Commerce funds: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = 'Escrow payment';
$handler->display->display_options['displays'] = array(
 'incoming_escrow_payments' => 'incoming_escrow_payments',
 'default' => 0,
 'issued_transactions' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_arguments'] = FALSE;
