<?php

/**
 * @file
 * Contains hook documentation.
 */

/**
 * Alters redirect data.
 *
 * @param Payment $payment
 *   The payment for which to alter the data.
 * @param array $data
 *   The data to alter. Keys and values correspond to Paypal PPS's documented
 *   redirect data.
 */
function hook_paypal_payment_pps_data(Payment $payment, array &$data) {
  // Enforce the interface language to be Ukrainian.
  $data['lc'] = 'UK';
}
