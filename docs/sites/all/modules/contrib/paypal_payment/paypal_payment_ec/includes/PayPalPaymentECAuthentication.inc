<?php

/**
 * Contains \PayPalPaymentECAuthentication.
 */

/**
 * PayPal Express Checkout authentication.
 */
class PayPalPaymentECAuthentication {

  /**
   * The date and time at which the token was generated.
   *
   * @var integer
   *   A UNIX timestamp.
   */
  public $created;

  /**
   * The ID of the PayPal payer.
   *
   * @var string
   */
  public $payerID = '';

  /**
   * The PID of the Payment this is for.
   *
   * @var integer
   */
  public $pid = 0;

  /**
   * The token.
   *
   * @var string
   */
  public $token;

  /**
   * Constructs a new class instance.
   *
   * @param string $token
   *   See self::$token.
   * @param int $created
   *   See self::$created.
   * @param int $pid
   *   See self::$pid.
   */
  public function __construct($token = NULL, $created = NULL, $pid = 0) {
    $this->created = $created;
    $this->pid = $pid;
    $this->token = $token;
  }

}
