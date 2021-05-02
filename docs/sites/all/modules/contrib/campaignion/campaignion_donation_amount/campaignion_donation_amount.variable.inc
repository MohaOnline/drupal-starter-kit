<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_donation_amount_variable_info($options) {
  $v['campaignion_donation_amount_currency_symbol'] = [
    'type' => 'string',
    'title' => t('Currency symbol', [], $options),
    'description' => t('Currency symbol used for donation amounts.', [], $options),
    'default' => '€',
    'localize' => TRUE,
  ];
  return $v;
}
