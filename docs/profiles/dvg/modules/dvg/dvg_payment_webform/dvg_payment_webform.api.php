<?php

/**
 * @file
 * Hooks provided by DvG Payment Webform.
 */

/**
 * Act on payment completion.
 * Note: Could be run concurrently by separate instances.
 * As both the user, as well as the payment provider will try to complete the payment at about the same time.
 *
 * @param string $status
 *   Status of the payment, either PAYMENT_STATUS_SUCCESS or PAYMENT_STATUS_PENDING.
 * @param stdClass $node
 *   Drupal node object of a webform.
 * @param stdClass $submission
 *   Webform submission for the payment.
 */
function hook_dvg_payment_completed($status, $node, $submission) {
  if ($status === PAYMENT_STATUS_SUCCESS) {
    webform_submission_send_mail($node, $submission);
  }
}

/**
 * Alter the payment amount before it is send to the payment provider.
 * E.g. to alter the amount depending on user input in the form.
 *
 * This hook is also called when showing the payment summary.
 *
 * @param int $payment_amount
 *   The payment amount.
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function hook_dvg_payment_webform_amount_alter(&$payment_amount, $form, $form_state) {
  // Multiply the payment amount by 3.
  $multiplier = 3;
  $payment_amount = $multiplier * $payment_amount;
}
