<?php

/**
 * @file
 * Views callbacks for "All transactions" views.
 *
 * Commerce funds module.
 */

  $view = new view();
$view->name = 'commerce_funds_all_transactions';
$view->description = 'Display all transactions from all users.';
$view->tag = 'Commerce funds';
$view->base_table = 'commerce_funds_transactions';
$view->human_name = 'All transactions';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'All transactions';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer transactions';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['style_plugin'] = 'table';
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'No transaction found';
$handler->display->display_options['empty']['area']['empty'] = TRUE;
$handler->display->display_options['empty']['area']['content'] = '<p>No transaction found</p>';
$handler->display->display_options['empty']['area']['format'] = 'full_html';
/* Relationship: Commerce funds: Recipient of the transaction. */
$handler->display->display_options['relationships']['recipient']['id'] = 'recipient';
$handler->display->display_options['relationships']['recipient']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['relationships']['recipient']['field'] = 'recipient';
/* Relationship: Commerce funds: Issuer of the transaction. */
$handler->display->display_options['relationships']['issuer']['id'] = 'issuer';
$handler->display->display_options['relationships']['issuer']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['relationships']['issuer']['field'] = 'issuer';
/* Field: Commerce funds: Transaction ID */
$handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
$handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
/* Field: Commerce funds: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['type']['field'] = 'type';
/* Field: Commerce funds: Transaction date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Date';
$handler->display->display_options['fields']['created']['date_format'] = 'medium';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: User: Name */
$handler->display->display_options['fields']['name_1']['id'] = 'name_1';
$handler->display->display_options['fields']['name_1']['table'] = 'users';
$handler->display->display_options['fields']['name_1']['field'] = 'name';
$handler->display->display_options['fields']['name_1']['relationship'] = 'issuer';
$handler->display->display_options['fields']['name_1']['label'] = 'From';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'recipient';
$handler->display->display_options['fields']['name']['label'] = 'To';
/* Field: Commerce funds: Note */
$handler->display->display_options['fields']['notes']['id'] = 'notes';
$handler->display->display_options['fields']['notes']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['notes']['field'] = 'notes';
/* Field: Commerce funds: Transaction status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = 'Status';
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
/* Sort criterion: Commerce funds: Transaction date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Filter criterion: Commerce funds: Transaction date */
$handler->display->display_options['filters']['created']['id'] = 'created';
$handler->display->display_options['filters']['created']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['filters']['created']['field'] = 'created';
$handler->display->display_options['filters']['created']['group'] = 1;
$handler->display->display_options['filters']['created']['exposed'] = TRUE;
$handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
$handler->display->display_options['filters']['created']['expose']['label'] = 'Date';
$handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
$handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
$handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Commerce funds: Transaction status */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['exposed'] = TRUE;
$handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
$handler->display->display_options['filters']['status']['expose']['label'] = 'Transaction status';
$handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
$handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
$handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
$handler->display->display_options['filters']['status']['is_grouped'] = TRUE;
$handler->display->display_options['filters']['status']['group_info']['label'] = 'Status';
$handler->display->display_options['filters']['status']['group_info']['identifier'] = 'status';
$handler->display->display_options['filters']['status']['group_info']['group_items'] = array(
  1 => array(
    'title' => 'Pending',
    'operator' => '=',
    'value' => 'Pending',
  ),
  2 => array(
    'title' => 'Completed',
    'operator' => '=',
    'value' => 'Completed',
  ),
  3 => array(
    'title' => 'Cancelled',
    'operator' => '=',
    'value' => 'Cancelled',
  ),
);
/* Filter criterion: Commerce funds: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'commerce_funds_transactions';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['group'] = 1;
$handler->display->display_options['filters']['type']['exposed'] = TRUE;
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
$handler->display->display_options['filters']['type']['is_grouped'] = TRUE;
$handler->display->display_options['filters']['type']['group_info']['label'] = 'Type';
$handler->display->display_options['filters']['type']['group_info']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['group_info']['group_items'] = array(
  1 => array(
    'title' => 'Deposit',
    'operator' => '=',
    'value' => 'Deposit',
  ),
  2 => array(
    'title' => 'Transfer',
    'operator' => '=',
    'value' => 'Transfer',
  ),
  3 => array(
    'title' => 'Escrow payment',
    'operator' => '=',
    'value' => 'Escrow Payment',
  ),
);
/* Filter criterion: User: Name (raw) */
$handler->display->display_options['filters']['name']['id'] = 'name';
$handler->display->display_options['filters']['name']['table'] = 'users';
$handler->display->display_options['filters']['name']['field'] = 'name';
$handler->display->display_options['filters']['name']['relationship'] = 'uid';
$handler->display->display_options['filters']['name']['group'] = 1;
$handler->display->display_options['filters']['name']['exposed'] = TRUE;
$handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['label'] = 'Issued by';
$handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
$handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);

/* Display: All transactions (Page) */
$handler = $view->new_display('page', 'All transactions (Page)', 'page');
$handler->display->display_options['display_description'] = 'Display all transactions from all users.';
$handler->display->display_options['path'] = 'admin/commerce/funds/view-transactions';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'View Transactions';
$handler->display->display_options['menu']['description'] = 'View all users transactions';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'management';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
