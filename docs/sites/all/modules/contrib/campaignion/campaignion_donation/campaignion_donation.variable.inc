<?php

/**
 * @file
 * Hook implementation for the variable module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_donation_variable_info() {
  $info['campaignion_donation_manual_direct_debit_one_off'] = [
    'title' => t('Manual direct debit donation one-off allowed'),
    'description' => t('Whether manual direct debit is available for one-off payments.'),
    'type' => 'boolean',
    'default' => FALSE,
  ];
  return $info;
}
