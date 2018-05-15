<?php
/**
 * @file
 * Provides hook documentation for this module.
 */

/**
 * Allows other modules to alter the transaction error before it's
 * displayed to a user.
 *
 * @param $error string
 *   The error message that's displayed to the user.
 * @param $transaction
 *   The commerce payment transaction.
 * @param $response
 *   The response from Braintree API.
 */
function hook_commerce_braintree_transaction_error_alter(&$error, $transaction, $response) {
  $error = t('The billing information you entered is invalid.');
}

/**
 * Allows other modules to alter the transaction message.
 *
 * @param $message string
 *   The transaction message built by commerce_braintree.
 * @param $response object
 *   The commerce order object that built the sale.
 */
function hook_commerce_braintree_build_transaction_message_alter(&$message, $response) {
  $message = t('The transaction was successful');
}
