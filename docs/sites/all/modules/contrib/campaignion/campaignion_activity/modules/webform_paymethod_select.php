<?php

use \Drupal\campaignion_activity\WebformPayment;
use \Drupal\webform_paymethod_select\WebformPaymentContext;

/**
 * Implements hook_campaignion_action_info().
 */
function webform_paymethod_select_campaignion_action_info() {
  $info['webform_payment'] = 'Drupal\campaignion_activity\WebformPayment';
}

/**
 * Implements hook_payment_status_change().
 */
function campaignion_activity_payment_status_change(Payment $payment, PaymentStatusItem $previous_status_item) {
  $statusChangedToSuccess = $payment->getStatus()->status == PAYMENT_STATUS_SUCCESS && $previous_status_item->status != PAYMENT_STATUS_SUCCESS;
  $hasContextObj = $payment->contextObj instanceof WebformPaymentContext;
  if (!$statusChangedToSuccess || !$hasContextObj)
    return;

  if (!($activity = WebformPayment::byPayment($payment))) {
    try {
      $activity = WebformPayment::fromPayment($payment);
      $activity->save();
    } catch (Exception $e) {
      watchdog('campaignion_activity', 'Error when trying to log webform_payment activity: !message -- !trace', array('!message' => $e->getMessage(), '!trace' => $e->getTraceAsString()), WATCHDOG_WARNING);
    }
  }
}
