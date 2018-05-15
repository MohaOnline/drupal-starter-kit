<?php

/**
 * @file
 * Provides API methods exposed by this module.
 */

/**
 * Alter sale data before transaction sent to Braintree.
 *
 * @param array $sale_data
 *   The array that is passed to Braintree_Transaction::sale().
 * @param mixed $order
 *   The commerce order object that built the sale.
 *
 * @see https://developers.braintreepayments.com/javascript+php/sdk/server/transaction-processing/create
 */
function commerce_braintree_hostedfields_sale_data_alter(array &$sale_data, $order) {
  // Change the sale amount to $100.00.
  $sale_data['amount'] = '100.00';
}

/**
 * Implements hook_commerce_braintree_hostedfields_js_alter().
 *
 * @param $js_settings
 *   An array of settings by reference that will be passed to
 *   the javascript API for Braintree.
 * @param $payment_method
 *    The Drupal commerce payment method settings for context.
 */
function commerce_braintree_commerce_braintree_hostedfields_js_alter(&$js_settings, $payment_method) {
  // Change the color of the input text to pink.
  // See https://developers.braintreepayments.com/reference/client-reference/javascript/v2/hosted-fields#options
  $js_settings['hostedFields']['styles'] = array(
    'input' => array(
      'color' => 'blue',
    ),
  );
}
