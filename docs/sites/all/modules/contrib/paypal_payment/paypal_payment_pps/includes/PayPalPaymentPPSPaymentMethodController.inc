<?php

/**
 * Contains \PayPalPaymentPPSPaymentMethodController.
 */

/**
 * A PayPal Payments Standard payment method.
 */
class PayPalPaymentPPSPaymentMethodController extends PaymentMethodController implements PayPalPaymentIPNPaymentMethodControllerInterface {

  /**
   * Automatic funds capture.
   */
  const CAPTURE_AUTOMATIC = 0;

  /**
   * Manual funds capture.
   */
  const CAPTURE_MANUAL = 1;

  /**
   * The production server URL.
   */
  const SERVER_URL = 'https://www.paypal.com/cgi-bin/webscr';

  /**
   * The sandbox server URL.
   */
  const SANDBOX_SERVER_URL = 'https://www.sandbox.paypal.com/cgi-bin/webscr';

  /**
   * {@inheritdoc}
   */
  public $controller_data_defaults = array(
    'email_address' => '',
    'server' => self::SERVER_URL,
    'capture' => self::CAPTURE_AUTOMATIC,
  );

  /**
   * {@inheritdoc}
   */
  public $payment_method_configuration_form_elements_callback = 'paypal_payment_pps_payment_method_configuration_form_elements';

  /**
   * Constructs a new instance.
   */
  public function __construct() {
    $currency_codes = array('AUD', 'BRL', 'CAD', 'CHF', 'CZK', 'DKK', 'EUR', 'GBP', 'HKD', 'HUF', 'ILS', 'JPY', 'MXN', 'MYR', 'NOK', 'NZD', 'PHP', 'PLN', 'SEK', 'SGD', 'THB', 'TRY', 'TWD', 'USD');
    $this->currencies = array_fill_keys($currency_codes, array());
    $this->title = 'PayPal Payments Standard';
  }

  /**
   * {@inheritdoc}
   */
  public function validate(Payment $payment, PaymentMethod $payment_method, $strict) {
  }

  /**
   * {@inheritdoc}
   */
  public function execute(Payment $payment) {
    entity_save('payment', $payment);
    $_SESSION['paypal_payment_pps_pid'] = $payment->pid;
    $url = 'paypal_payment_pps/redirect/' . $payment->pid;
    if (!empty($payment->contextObj)) {
      // Support for payment_context.
      $payment->contextObj->redirect($url);
    }
    else {
      drupal_goto($url);
    }
  }

  /**
   * {@inheritdoc}
   */
  public static function PayPalValidateIPNVariables(Payment $payment, array $ipn_variables) {
    // PayPal treats email addresses case-insensitively and returns them in
    // lowercase in $ipn_variables['business']. Therefore we must perform
    // a case-insensitive comparison.
    return strtolower($ipn_variables['business']) == strtolower($payment->method->controller_data['email_address']);
  }

  /**
   * {@inheritdoc}
   */
  public static function PayPalProcessIPN(Payment $payment, array $ipn_variables) {
  }

}
