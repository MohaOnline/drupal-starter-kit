<?php

/**
 * Contains \PayPalPaymentECPaymentMethodController.
 */

/**
 * A PayPal Express Checkout payment method.
 */
class PayPalPaymentECPaymentMethodController extends PayPalPaymentNVPAPIPaymentMethodControllerBase implements PayPalPaymentIPNPaymentMethodControllerInterface {

  /**
   * Automatic funds capture.
   */
  const CAPTURE_AUTOMATIC = 'Sale';

  /**
   * Manual funds capture.
   */
  const CAPTURE_MANUAL = 'Authorization';

  /**
   * The production API server URL.
   */
  const NVP_API_URL_SERVER_PRODUCTION = 'https://api-3t.paypal.com/nvp';

  /**
   * The sandbox API server URL.
   */
  const NVP_API_URL_SERVER_SANDBOX = 'https://api-3t.sandbox.paypal.com/nvp';

  /**
   * The production checkout URL.
   */
  const URL_CHECKOUT_PRODUCTION = 'https://www.paypal.com/cgi-bin/webscr';

  /**
   * The sandbox checkout URL.
   */
  const URL_CHECKOUT_SANDBOX = 'https://www.sandbox.paypal.com/cgi-bin/webscr';

  /**
   * The token lifetime, e.g. for how many seconds they are valid.
   */
  const PAYPAL_TOKEN_LIFETIME = 10800;

  /**
   * {@inheritdoc}
   */
  public $controller_data_defaults = array(
    'capture' => self::CAPTURE_AUTOMATIC,
    'email_address' => '',
    'password' => '',
    'server' => self::NVP_API_SERVER_PRODUCTION,
    'signature' => '',
    'username' => '',
  );

  /**
   * {@inheritdoc}
   */
  public $payment_method_configuration_form_elements_callback = 'paypal_payment_ec_payment_method_configuration_form_elements';

  /**
   * Constructs a new instance.
   */
  public function __construct() {
    $currency_codes = array('AUD', 'CAD', 'CHF', 'CZK', 'DKK', 'EUR', 'GBP', 'HKD', 'HUF', 'JPY', 'NOK', 'NZD', 'PLN', 'SEK', 'SGD', 'USD');
    $this->currencies = array_fill_keys($currency_codes, array());
    $this->title = 'PayPal Express Checkout';
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
    // Prepare the PayPal checkout token.
    $authentication = NULL;
    if ($payment->pid) {
      $authentication = $this->loadAuthentication($payment->pid);
    }
    if (!$authentication) {
      entity_save('payment', $payment);
      $authentication = $this->setExpressCheckout($payment);
      if ($authentication) {
        $this->saveAuthentication($authentication);
      }
    }

    // Start checkout.
    if ($authentication) {
      drupal_goto($this->checkoutURL($payment->method->controller_data['server'], $authentication->token));
    }
    else {
      $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));
    }
  }

  /**
   * Returns the redirect URL.
   *
   * @throws InvalidArgumentException
   *
   * @param string $server
   *   One of the self::NVP_API_SERVER_* constants.
   * @param string $token
   *   The PayPal checkout token received from setExpressCheckout.
   *
   * @return string
   */
  public function checkoutURL($server, $token) {
    $urls = array(
      $this::NVP_API_SERVER_PRODUCTION => $this::URL_CHECKOUT_PRODUCTION,
      $this::NVP_API_SERVER_SANDBOX => $this::URL_CHECKOUT_SANDBOX,
    );

    if (isset($urls[$server])) {
      $url = url($urls[$server], array(
        'external' => TRUE,
      ));
    }
    else {
      throw new InvalidArgumentException(t('Server type does not exist.'));
    }
    $options = array(
      'query' => array(
        'cmd' => '_express-checkout',
        'token' => $token,
        // This module requires that users confirm their payments when they are
        // at the Paypal site, and not when they have returned to Drupal.
        'useraction' => 'commit',
      ),
    );

    return url($url, $options + array(
      'external' => TRUE,
    ));
  }

  /**
   * Sets up a PayPal payment.
   *
   * @param Payment $payment
   *
   * @return PayPalPaymentECAuthentication|false
   *   A PayPal checkout token or FALSE in case of failure.
   */
  public function setExpressCheckout(Payment $payment) {
    global $user;

    $return_url = url('paypal_payment_ec/return', array(
      'absolute' => TRUE,
    ));
    $nvp_request = array(
      'METHOD' => 'SetExpressCheckout',
      'RETURNURL' => $return_url,
      'CANCELURL' => $return_url,
      'LOCALECODE' => strtoupper(variable_get('site_default_country', 'US')),
      'EMAIL' => (!empty($user->mail)) ? $user->mail : '',
    ) + $this->paymentNVP($payment);

    $nvp_response = $this->NVPAPIRequest($nvp_request, $payment);
    if (isset($nvp_response['TOKEN']) && isset($nvp_response['TIMESTAMP'])) {
      $created = DateTime::createFromFormat(DateTime::ISO8601, $nvp_response['TIMESTAMP'])->getTimestamp();
      return new PayPalPaymentECAuthentication($nvp_response['TOKEN'], $created, $payment->pid);
    }
    return FALSE;
  }

  /**
   * Gets checkout information.
   *
   * @see self::setExpressCheckout()
   *
   * @param Payment $payment
   * @param PayPalPaymentECAuthentication $authentication
   *
   * @return bool
   *   Whether the request was successful.
   */
  public function getExpressCheckoutDetails(Payment $payment, PayPalPaymentECAuthentication $authentication) {
    $nvp_request = array(
      'METHOD' => 'GetExpressCheckoutDetails',
      'TOKEN' => $authentication->token,
    );
    $nvp_response = $this->NVPAPIRequest($nvp_request, $payment);
    if (isset($nvp_response['PAYERID'])) {
      $authentication->payerID = $nvp_response['PAYERID'];
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Executes a checkout.
   *
   * @param Payment $payment
   * @param PayPalPaymentECAuthentication $authentication
   *
   * @return bool
   *   Whether the request was successful.
   */
  public function doExpressCheckoutPayment(Payment $payment, $authentication) {
    $nvp_request = array(
      'METHOD' => 'doExpressCheckoutPayment',
      'PAYERID' => $authentication->payerID,
      'TOKEN' => $authentication->token,
    ) + $this->paymentNVP($payment);
    $nvp_response = $this->NVPAPIRequest($nvp_request, $payment);
    if (isset($nvp_response['PAYMENTINFO_0_PAYMENTSTATUS'])) {
      $payment_status = $this->convertStatus($nvp_response);
      $payment->setStatus(new PaymentStatusItem($payment_status));
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Returns NPV variables for a Payment.
   *
   * @param Payment $payment
   *
   * @return array
   */
  public function paymentNVP(Payment $payment) {
    $currency = currency_load($payment->currency_code);
    $amount = $currency->roundAmount($payment->totalAmount(TRUE));
    return array(
      'PAYMENTREQUEST_0_CURRENCYCODE' => $payment->currency_code,
      'PAYMENTREQUEST_0_AMT' => $amount,
      'PAYMENTREQUEST_0_PAYMENTACTION' => $payment->method->controller_data['capture'],
      'PAYMENTREQUEST_0_DESC' => $payment->description,
      'PAYMENTREQUEST_0_INVNUM' => PayPalPaymentIPNController::invoiceID($payment->pid),
      'PAYMENTREQUEST_0_NOTIFYURL' => PayPalPaymentIPNController::URL(),
    );
  }

  /**
   * Saves authentication information.
   *
   * @param PayPalPaymentECAuthentication $authentication
   *
   * @return integer
   *   MergeQuery::STATUS_INSERT or MergeQuery::STATUS_NEW.
   */
  public function saveAuthentication(PayPalPaymentECAuthentication $authentication) {
    $merge_status = db_merge('paypal_payment_ec_payment')
      ->key(array(
        'pid' => $authentication->pid,
      ))
      ->fields(array(
        'created' => $authentication->created,
        'payerID' => $authentication->payerID,
        'pid' => $authentication->pid,
        'token' => $authentication->token,
      ))
      ->execute();

    return $merge_status;
  }

  /**
   * Loads authentication information.
   *
   * @param mixed $value
   *   The value $property must have when loading the authentication
   *   information.
   * @param string $property
   *   The property to select by. Must be either "pid" or "token".
   *
   * @return string|false
   *   The token, or FALSE in case of failure.
   */
  public function loadAuthentication($value, $property = 'pid') {
    /** @var DatabaseStatementPrefetch $statement */
    $statement = db_select('paypal_payment_ec_payment', 'mpet')
      ->fields('mpet')
      ->condition($property, $value)
      ->execute();
    $statement->setFetchMode(PDO::FETCH_CLASS | PDO::FETCH_PROPS_LATE, 'PayPalPaymentECAuthentication');
    $authentication = $statement->fetch();
    if ($authentication && $authentication->created + self::PAYPAL_TOKEN_LIFETIME > REQUEST_TIME) {
      return $authentication;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Deletes authentication information.
   *
   * @param integer $pid
   *
   * @return null
   */
  public function deleteAuthentication($pid) {
    db_delete('paypal_payment_ec_payment')
      ->condition('pid', $pid)
      ->execute();
  }

  /**
   * Returns a map of PayPal statuses to Payment statuses.
   *
   * @return array
   *   Keys are PayPal statuses, values are Payment statuses.
   */
  public function statusMap() {
    return array(
      'Canceled-Reversal' => PAYPAL_PAYMENT_STATUS_CANCELLED_REVERSAL,
      'In-Progress' => PAYMENT_STATUS_PENDING,
      'Partially-Refunded' => PAYPAL_PAYMENT_STATUS_PARTIALLY_REFUNDED,
      'Completed-Funds-Held' => PAYPAL_PAYMENT_STATUS_COMPLETED_FUNDS_HELD,
    ) + PayPalPaymentIPNController::statusMap();
  }

  /**
   * Converts a PayPal status to a Payment status.
   *
   * @param array $nvp_response
   *   A doExpressCheckout response.
   *
   * @return string
   */
  public function convertStatus(array $nvp_response) {
    if (isset($nvp_response['PAYMENTINFO_0_PAYMENTSTATUS'])) {
      $paypal_status = $nvp_response['PAYMENTINFO_0_PAYMENTSTATUS'];
      if ($paypal_status == 'Pending' && isset($nvp_response['PAYMENTINFO_0_PENDINGREASON'])) {
        $status_map = PayPalPaymentIPNController::pendingStatusMap();
        $pending_reason = $nvp_response['PAYMENTINFO_0_PENDINGREASON'];
        return isset($status_map[$pending_reason]) ? $status_map[$pending_reason] : PAYMENT_STATUS_PENDING;
      }
      else {
        $status_map = $this->statusMap();
        return isset($status_map[$paypal_status]) ? $status_map[$paypal_status] : PAYMENT_STATUS_UNKNOWN;
      } 
    }
    return PAYMENT_STATUS_UNKNOWN;
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
