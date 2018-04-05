<?php

/**
 * Contains \PayPalPaymentIPN.
 */

/**
 * A PayPal IPN.
 *
 * @see PayPalPaymentIPNController
 * @see paypal_payment_schema()
 */
class PayPalPaymentIPN {

  /**
   * The PID of the Payment this IPN is for.
   *
   * @see PayPalPaymentIPNController::invoiceID()
   * @see PayPalPaymentIPNController::PID()
   *
   * @var string
   */
  public $pid = 0;

  /**
   * The PayPal transaction ID.
   *
   * @var string
   */
  public $txn_id = '';

  /**
   * The payment status item ID.
   *
   * @var string
   */
  public $psiid = 0;

  /**
   * Constructs a new instance.
   */
  public function __construct(array $properties = array()) {
    foreach ($properties as $property => $value) {
      $this->$property = $value;
    }
  }

}
