<?php

/**
 * Contains \PayPalPaymentIPNTestPaymentMethodController.
 */

/**
 * A dummy class that simply implements PayPalPaymentIPNPaymentMethodControllerInterface.
 */
class PayPalPaymentIPNTestPaymentMethodController extends PaymentMethodController implements PayPalPaymentIPNPaymentMethodControllerInterface {

  /**
   * Constructs a new instance.
   */
  public function __construct() {
    $this->title = __CLASS__;
  }

  /**
   * {@inheritdoc}
   */
  public static function PayPalValidateIPNVariables(Payment $payment, array $ipn_variables) {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public static function PayPalProcessIPN(Payment $payment, array $ipn_variables) {
  }

}
