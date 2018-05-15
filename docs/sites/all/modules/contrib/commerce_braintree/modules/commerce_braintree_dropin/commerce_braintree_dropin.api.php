<?php

/**
 * @file
 * Provies API methods exposed by this module.
 */

/**
 * Allows other modules to alter the sale data before it is submitted to
 * Braintree
 *
 * @param $sale_data
 *   The array that is passed to Braintree_Transaction::sale().
 *   @see https://developers.braintreepayments.com/javascript+php/sdk/server/transaction-processing/create
 * @param $order
 *   The commerce order object that built the sale.
 */
function hook_commerce_braintree_dropin_sale_data_alter(&$sale_data, $order) {
  // Change the sale amount to $100.00.
  $sale_data['amount'] = '100.00';
}

/**
 * Implements hook_commerce_braintree_dropin_js_alter().
 *
 * @param $js_settings
 *   An array of settings by reference that will be passed to
 *   the javascript API for Braintree.
 * @param $payment_method
 *    The Drupal commerce payment method settings for context.
 */
function commerce_braintree_commerce_braintree_dropin_js_alter(&$js_settings, $payment_method) {
  // No example.
}
