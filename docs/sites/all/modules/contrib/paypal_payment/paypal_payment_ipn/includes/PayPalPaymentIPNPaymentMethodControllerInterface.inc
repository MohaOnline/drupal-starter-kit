<?php

/**
 * Contains \PayPalPaymentIPNPaymentMethodControllerInterface.
 */

/**
 * Provides IPN for payment methods.
 */
interface PayPalPaymentIPNPaymentMethodControllerInterface {

  /**
   * Validates IPN message variables for a payment.
   *
   * @param Payment $payment
   * @param array $ipn_variables
   *
   * @return bool
   *   Whether the IPN variables are valid.
   */
  public static function PayPalValidateIPNVariables(Payment $payment, array $ipn_variables);

  /**
   * Processes an IPN for a payment.
   *
   * @param Payment $payment
   * @param array $ipn_variables
   *
   * @return NULL
   */
  public static function PayPalProcessIPN(Payment $payment, array $ipn_variables);

}
