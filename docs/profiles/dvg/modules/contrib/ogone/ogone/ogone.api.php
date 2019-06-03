<?php

/**
 * @file
 * Hook documentation.
 */

/**
 * Alters the data that is sent to Ogone when redirecting payers.
 *
 * @param array $data
 * @param Payment $payment
 *
 * @return NULL
 */
function hook_ogone_redirect_data_alter(array &$data, Payment $payment) {
  $data['BGCOLOR'] = '#A1B2C3';
}

/**
 * Responds to Ogone payment feedback.
 *
 * @param array $data
 * @param Payment $payment
 *
 * @return NULL
 */
function hook_ogone_feedback(array $data, Payment $payment) {
  if ($data['AMOUNT'] < $payment->totalAmount(TRUE)) {
    drupal_set_message(t('Where is the money, Sonny?'));
  }
  else {
    drupal_set_message(t('Thank you!'));
  }
}
