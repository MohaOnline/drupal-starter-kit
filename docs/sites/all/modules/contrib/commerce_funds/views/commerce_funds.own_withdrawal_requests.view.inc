<?php

/**
 * @file
 * Views callbacks for "Own withdrawal requests" views.
 *
 * Provides users own withdrawal requests.
 * Commerce funds module.
 */

$view = new view();
$view->name = 'commerce_funds_withdrawals';
$view->description = 'Display user withdrawal requests.';
$view->tag = 'Commerce funds';
$view->base_table = 'commerce_funds_withdrawal_requests';
$view->human_name = 'User withdrawal requests';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Your withdrawal request';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'withdraw funds';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
 'created' => 'created',
 'brut_amount' => 'brut_amount',
 'commission' => 'commission',
 'amount' => 'amount',
 'method' => 'method',
 'status' => 'status',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
 'created' => array(
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
   'empty_column' => 1,
 ),
 'commission' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 1,
 ),
 'amount' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 0,
 ),
 'method' => array(
   'sortable' => 0,
   'default_sort_order' => 'asc',
   'align' => '',
   'separator' => '',
   'empty_column' => 0,
 ),
 'status' => array(
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
$handler->display->display_options['header']['area']['label'] = 'Pending withdrawals';
$handler->display->display_options['header']['area']['empty'] = TRUE;
$handler->display->display_options['header']['area']['content'] = '<h2>Pending withdrawals</h2>';
$handler->display->display_options['header']['area']['format'] = 'full_html';
/* Field: Commerce funds: Withdrawal transaction date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Time';
$handler->display->display_options['fields']['created']['date_format'] = 'medium';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: Commerce funds: Withdrawal requested */
$handler->display->display_options['fields']['brut_amount']['id'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['brut_amount']['field'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['label'] = 'Request';
/* Field: Commerce funds: Withdrawal commission */
$handler->display->display_options['fields']['commission']['id'] = 'commission';
$handler->display->display_options['fields']['commission']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['commission']['field'] = 'commission';
$handler->display->display_options['fields']['commission']['label'] = 'Commission';
/* Field: Commerce funds: Withdrawal total amount */
$handler->display->display_options['fields']['net_amount']['id'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['net_amount']['field'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['label'] = 'Due';
/* Field: Commerce funds: Withdrawal method */
$handler->display->display_options['fields']['method']['id'] = 'method';
$handler->display->display_options['fields']['method']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['method']['field'] = 'method';
$handler->display->display_options['fields']['method']['label'] = 'Method';
/* Field: Commerce funds: Withdrawal status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = 'Status';
/* Contextual filter: Commerce funds: Withdrawal requester uid */
$handler->display->display_options['arguments']['issuer']['id'] = 'issuer';
$handler->display->display_options['arguments']['issuer']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['arguments']['issuer']['field'] = 'issuer';
$handler->display->display_options['arguments']['issuer']['default_action'] = 'default';
$handler->display->display_options['arguments']['issuer']['default_argument_type'] = 'current_user';
$handler->display->display_options['arguments']['issuer']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['issuer']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['issuer']['summary_options']['items_per_page'] = '25';
/* Filter criterion: Commerce funds: Withdrawal status */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 'Pending approval';

/* Display: Pending withdrawal requests (Page) */
$handler = $view->new_display('page', 'Pending withdrawal requests (Page)', 'page');
$handler->display->display_options['display_description'] = 'Display user pending withdrawal requests.';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Pending withdrawals';
$handler->display->display_options['header']['area']['empty'] = TRUE;
$handler->display->display_options['header']['area']['content'] = '<h2>Pending requests</h2>';
$handler->display->display_options['header']['area']['format'] = 'full_html';
$handler->display->display_options['path'] = 'user/funds/withdrawals';

/* Display: Previous withdrawal requests (Attachment) */
$handler = $view->new_display('attachment', 'Previous withdrawal requests (Attachment)', 'attachment_1');
$handler->display->display_options['display_description'] = 'Display previous user withdrawal requests.';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['label'] = 'Previous requests';
$handler->display->display_options['header']['area']['empty'] = TRUE;
$handler->display->display_options['header']['area']['content'] = '<h2>Previous requests</h2>';
$handler->display->display_options['header']['area']['format'] = 'full_html';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Commerce funds: Withdrawal transaction date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Time';
$handler->display->display_options['fields']['created']['date_format'] = 'medium';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: Commerce funds: Withdrawal requested */
$handler->display->display_options['fields']['brut_amount']['id'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['brut_amount']['field'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['label'] = 'Request';
/* Field: Commerce funds: Withdrawal commission */
$handler->display->display_options['fields']['commission']['id'] = 'commission';
$handler->display->display_options['fields']['commission']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['commission']['field'] = 'commission';
$handler->display->display_options['fields']['commission']['label'] = 'Commission';
/* Field: Commerce funds: Withdrawal total amount */
$handler->display->display_options['fields']['net_amount']['id'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['net_amount']['field'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['label'] = 'Paid';
/* Field: Commerce funds: Withdrawal method */
$handler->display->display_options['fields']['method']['id'] = 'method';
$handler->display->display_options['fields']['method']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['method']['field'] = 'method';
$handler->display->display_options['fields']['method']['label'] = 'Method';
/* Field: Commerce funds: Withdrawal status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = 'Status';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Commerce funds: Withdrawal status */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['operator'] = '!=';
$handler->display->display_options['filters']['status']['value'] = 'Pending approval';
$handler->display->display_options['displays'] = array(
 'page' => 'page',
 'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
