<?php

/**
 * @file
 * Views callbacks for "All withdrawal requests" views.
 *
 * Commerce funds module.
 */

 $view = new view();
$view->name = 'all_withdrawal_requests';
$view->description = 'Display all withdrawal requests from all users.';
$view->tag = 'Commerce funds';
$view->base_table = 'commerce_funds_withdrawal_requests';
$view->human_name = 'All withdrawal requests';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'All withdrawal requests';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer withdraw requests';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
/* Relationship: Commerce funds: Requester of the withdrawal. */
$handler->display->display_options['relationships']['issuer']['id'] = 'issuer';
$handler->display->display_options['relationships']['issuer']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['relationships']['issuer']['field'] = 'issuer';
/* Field: Commerce funds: Request ID */
$handler->display->display_options['fields']['request_id']['id'] = 'request_id';
$handler->display->display_options['fields']['request_id']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['request_id']['field'] = 'request_id';
/* Field: Commerce funds: Withdrawal transaction date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Date';
$handler->display->display_options['fields']['created']['date_format'] = 'medium';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: Commerce funds: Withdrawal requested */
$handler->display->display_options['fields']['brut_amount']['id'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['brut_amount']['field'] = 'brut_amount';
$handler->display->display_options['fields']['brut_amount']['label'] = 'Requested';
/* Field: Commerce funds: Withdrawal commission */
$handler->display->display_options['fields']['commission']['id'] = 'commission';
$handler->display->display_options['fields']['commission']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['commission']['field'] = 'commission';
$handler->display->display_options['fields']['commission']['label'] = 'Commission';
/* Field: Commerce funds: Withdrawal total amount */
$handler->display->display_options['fields']['net_amount']['id'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['net_amount']['field'] = 'net_amount';
$handler->display->display_options['fields']['net_amount']['label'] = 'Paid amount';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'issuer';
/* Field: Commerce funds: User balance */
$handler->display->display_options['fields']['balance']['id'] = 'balance';
$handler->display->display_options['fields']['balance']['table'] = 'commerce_funds_user_funds';
$handler->display->display_options['fields']['balance']['field'] = 'balance';
$handler->display->display_options['fields']['balance']['relationship'] = 'issuer';
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
/* Field: Commerce funds: Withdrawal operations */
$handler->display->display_options['fields']['operations']['id'] = 'operations';
$handler->display->display_options['fields']['operations']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['fields']['operations']['field'] = 'operations';
$handler->display->display_options['fields']['operations']['label'] = 'Operations';
/* Sort criterion: Commerce funds: Withdrawal transaction date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Filter criterion: User: Name (raw) */
$handler->display->display_options['filters']['name']['id'] = 'name';
$handler->display->display_options['filters']['name']['table'] = 'users';
$handler->display->display_options['filters']['name']['field'] = 'name';
$handler->display->display_options['filters']['name']['relationship'] = 'issuer';
$handler->display->display_options['filters']['name']['group'] = 1;
$handler->display->display_options['filters']['name']['exposed'] = TRUE;
$handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['label'] = 'Username';
$handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
$handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
$handler->display->display_options['filters']['name']['group_info']['label'] = 'Name (raw)';
$handler->display->display_options['filters']['name']['group_info']['identifier'] = 'name';
$handler->display->display_options['filters']['name']['group_info']['group_items'] = array(
  1 => array(
    'title' => 'Paypal',
    'operator' => '=',
    'value' => 'paypal',
  ),
  2 => array(
    'title' => 'Skrill',
    'operator' => '=',
    'value' => 'skrill',
  ),
  3 => array(
    'title' => 'Bank account',
    'operator' => '=',
    'value' => 'bank_account',
  ),
  4 => array(
    'title' => 'Check',
    'operator' => '=',
    'value' => 'check',
  ),
);
/* Filter criterion: Commerce funds: Withdrawal transaction date */
$handler->display->display_options['filters']['created']['id'] = 'created';
$handler->display->display_options['filters']['created']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['filters']['created']['field'] = 'created';
$handler->display->display_options['filters']['created']['group'] = 1;
$handler->display->display_options['filters']['created']['exposed'] = TRUE;
$handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
$handler->display->display_options['filters']['created']['expose']['label'] = 'Withdrawal transaction date';
$handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
$handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
$handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Commerce funds: Withdrawal method */
$handler->display->display_options['filters']['method']['id'] = 'method';
$handler->display->display_options['filters']['method']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['filters']['method']['field'] = 'method';
$handler->display->display_options['filters']['method']['group'] = 1;
$handler->display->display_options['filters']['method']['exposed'] = TRUE;
$handler->display->display_options['filters']['method']['expose']['operator_id'] = 'method_op';
$handler->display->display_options['filters']['method']['expose']['label'] = 'Withdrawal method';
$handler->display->display_options['filters']['method']['expose']['operator'] = 'method_op';
$handler->display->display_options['filters']['method']['expose']['identifier'] = 'method';
$handler->display->display_options['filters']['method']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
$handler->display->display_options['filters']['method']['is_grouped'] = TRUE;
$handler->display->display_options['filters']['method']['group_info']['label'] = 'Method';
$handler->display->display_options['filters']['method']['group_info']['identifier'] = 'method';
$handler->display->display_options['filters']['method']['group_info']['group_items'] = array(
  1 => array(
    'title' => 'Paypal',
    'operator' => '=',
    'value' => 'paypal',
  ),
  2 => array(
    'title' => 'Skrill',
    'operator' => '=',
    'value' => 'skrill',
  ),
  3 => array(
    'title' => 'Bank account',
    'operator' => '=',
    'value' => 'bank_account',
  ),
  4 => array(
    'title' => 'Check',
    'operator' => '=',
    'value' => 'check',
  ),
);
/* Filter criterion: Commerce funds: Withdrawal status */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'commerce_funds_withdrawal_requests';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['exposed'] = TRUE;
$handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
$handler->display->display_options['filters']['status']['expose']['label'] = 'Withdrawal status';
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
    'title' => 'Pending approval',
    'operator' => '=',
    'value' => 'Pending approval',
  ),
  2 => array(
    'title' => 'Approved',
    'operator' => '=',
    'value' => 'Approved',
  ),
  3 => array(
    'title' => 'Declined',
    'operator' => '=',
    'value' => 'Declined',
  ),
);

/* Display: All withdrawal requests (Page) */
$handler = $view->new_display('page', 'All withdrawal requests (Page)', 'all_withdrawal_requests');
$handler->display->display_options['display_description'] = 'Display all withdrawal requests from all users.';
$handler->display->display_options['path'] = 'admin/commerce/funds/view-withdraw-requests';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Withdrawals requests';
$handler->display->display_options['menu']['description'] = 'View all users withdrawal requests';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'management';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
