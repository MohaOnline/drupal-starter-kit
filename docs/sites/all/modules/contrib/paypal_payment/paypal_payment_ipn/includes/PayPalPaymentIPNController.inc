<?php

/**
 * Contains \PayPalPaymentIPNController.
 */

/**
 * Provides IPN functionality.
 *
 * @see PayPalPaymentIPN
 */
abstract class PayPalPaymentIPNController {

  /**
   * The PayPal IPN server URL.
   */
  const PAYPAL_IPN_SERVER_URL = 'https://www.paypal.com/cgi-bin/webscr';

  /**
   * The PayPal IPN sandbox server URL.
   */
  const PAYPAL_IPN_SANDBOX_SERVER_URL = 'https://www.sandbox.paypal.com/cgi-bin/webscr';

  /**
   * Loads an IPN.
   *
   * @param int $psiid
   *   The payment status item ID of the IPN to load.
   *
   * @return PayPalPaymentIPN|false
   */
  public static function load($psiid) {
    $ipn_data = db_select('paypal_payment_ipn', 'mpi')
      ->fields('mpi')
      ->condition('psiid', $psiid)
      ->execute()
      ->fetchAssoc();

    if ($ipn_data) {
      return new PayPalPaymentIPN($ipn_data);
    }
    return FALSE;
  }

  /**
   * Saves an IPN.
   *
   * @param PayPalPaymentIPN $ipn
   *
   * @return int
   *   The db_merge() return value.
   */
  public static function save(PayPalPaymentIPN $ipn) {
    $fields = array_intersect_key(get_object_vars($ipn), get_class_vars(get_class($ipn)));

    $merge_status = db_merge('paypal_payment_ipn')
      ->key(array('psiid' => $ipn->psiid))
      ->fields($fields)
      ->execute();

    return $merge_status;
  }

  /**
   * Deletes an IPN.
   *
   * @param int $psiid
   *   The payment status item ID of the IPN to load.
   */
  public static function delete($psiid) {
    db_delete('paypal_payment_ipn')
      ->condition('psiid', $psiid)
      ->execute();
  }

  /**
   * Returns the IPN URL.
   */
  public static function URL() {
    return url(PAYPAL_IPN_LISTENER_PATH, array(
      'absolute' => TRUE,
    ));
  }

  /**
   * Acknowledges an IPN.
   *
   * @param array $ipn_variables
   *   IPN message variables in the order they were received in from PayPal.
   *
   * @return bool
   *   Whether the IPN was valid and successfully acknowledged.
   */
  public static function acknowledge(array $ipn_variables) {
    // Prepare the request data.
    $ipn_variables['cmd'] = '_notify-validate';
    $data = [];
    foreach ($ipn_variables as $variable => $value) {
      $data[] = $variable . '=' . rawurlencode($value);
    }
    $data = implode('&', $data);

    // Execute the request.
    $url = empty($ipn_variables['test_ipn']) ? self::PAYPAL_IPN_SERVER_URL : self::PAYPAL_IPN_SANDBOX_SERVER_URL;
    // Use url() so we can alter the request using hook_url_outbound_alter().
    $url = url($url, array(
      'external' => TRUE,
    ));
    $response = chr_curl_http_request($url, array(
      'method' => 'POST',
      'data' => $data,
      'curl_opts' => [
        CURLOPT_SSL_VERIFYPEER => TRUE,
        CURLOPT_SSL_VERIFYHOST => 2,
      ],
    ));
    // Process the response.
    if (isset($response->error)) {
      watchdog('paypal_payment_ipn', 'An IPN acknowledgement failed with error @code: %error.', array(
        '@code' => $response->code,
        '%error' => $response->error,
      ), WATCHDOG_ERROR);
      return FALSE;
    }
    elseif ($response->data == 'VERIFIED') {
      return TRUE;
    }
    else {
      watchdog('paypal_payment_ipn', 'PayPal did not verify an IPN acknowledgement.', array(), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  /**
   * Validates the contents of IPN variables.
   *
   * @param array $ipn_variables
   *
   * @return boolean
   */
  public static function validate(array $ipn_variables) {
    // Only process IPNs for which we have a payment.
    if (isset($ipn_variables['invoice']) && ($pid = self::PID($ipn_variables['invoice']))) {
      if ($payment = entity_load_single('payment', $pid)) {
        // Allow payment method controllers to completely take over validation.
        if ($payment->method->controller instanceof PayPalPaymentIPNPaymentMethodControllerInterface) {
          return $payment->method->controller->PayPalValidateIPNVariables($payment, $ipn_variables);
        }
        else {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Processes an IPN.
   *
   * @param array $ipn_variables
   *
   * @return \PaymentStatusItem
   *   The new payment status item.
   */
  public static function process(array $ipn_variables) {
    $pid = self::PID($ipn_variables['invoice']);
    $payment = entity_load_single('payment', $pid);
    if ($payment->method->controller instanceof PayPalPaymentIPNPaymentMethodControllerInterface) {
      $payment->method->controller->PayPalProcessIPN($payment, $ipn_variables);
    }
    $new_status = new PaymentStatusItem(self::convertStatus($ipn_variables));
    $new_status->ipn = new PayPalPaymentIPN($ipn_variables);
    $payment->setStatus($new_status);
    entity_save('payment', $payment);
    return $new_status;
  }

  /**
   * Hashes a Payment PID.
   *
   * @param integer $pid
   *
   * @return string
   */
  public static function hashPID($pid) {
    return hash('sha256', $pid . drupal_get_hash_salt());
  }

  /**
   * Creates a PayPal invoice ID from a Payment PID.
   *
   * @see PayPalPaymentController::PID()
   *
   * @param integer $pid
   *
   * @return string
   */
  public static function invoiceID($pid) {
    return 'drupal_paypal_payment_ipn-' . self::hashPID($pid) . '-' . $pid;
  }

  /**
   * Extracts a Payment PID from a PayPa invoice ID.
   *
   * @see PayPalPaymentController::invoiceID()
   *
   * @param string $invoice_id
   *
   * @return integer|false
   *   The PID, or FALSE if the invoice ID did not contain a valid PID.
   */
  public static function PID($invoice_id) {
    $fragments = explode('-', $invoice_id);
    if (count($fragments) == 3) {
      list(, $hash, $pid) = $fragments;
      return $hash == self::hashPID($pid) ? (int) $pid : FALSE;
    }
    return FALSE;
  }

  /**
   * Returns a map of PayPal statuses to Payment statuses.
   *
   * @return array
   *   Keys are PayPal statuses, values are Payment statuses.
   */
  public static function statusMap() {
    return array(
      'Canceled_Reversal' => PAYPAL_PAYMENT_STATUS_CANCELLED_REVERSAL,
      'Completed' => PAYMENT_STATUS_SUCCESS,
      'Created' => PAYMENT_STATUS_NEW,
      'Denied' => PAYPAL_PAYMENT_STATUS_DENIED,
      'Expired' => PAYMENT_STATUS_EXPIRED,
      'Failed' => PAYMENT_STATUS_FAILED,
      'Pending' => PAYMENT_STATUS_PENDING,
      'Refunded' => PAYPAL_PAYMENT_STATUS_REFUNDED,
      'Reversed' => PAYPAL_PAYMENT_STATUS_REVERSED,
      // @todo How do Processed and Completed relate to each other?
      'Processed' => PAYMENT_STATUS_SUCCESS,
      'Voided' => PAYMENT_STATUS_AUTHORIZATION_FAILED,
    );
  }

  /**
   * Returns a map of PayPal "pending" statuses to Payment statuses.
   *
   * @return array
   *   Keys are PayPal pending reasons, values are Payment statuses.
   */
  public static function pendingStatusMap() {
    return array(
      'address' => PAYPAL_PAYMENT_STATUS_ADDRESS,
      'authorization' => PAYPAL_PAYMENT_STATUS_WAITING_FOR_CAPTURE,
      'echeck' => PAYPAL_PAYMENT_STATUS_WAITING_FOR_CLEARANCE,
      'intl' => PAYPAL_PAYMENT_STATUS_INTL,
      'multi-currency' => PAYPAL_PAYMENT_STATUS_MULTI_CURRENCY,
      'order' => PAYPAL_PAYMENT_STATUS_WAITING_FOR_CAPTURE,
      'paymentreview' => PAYPAL_PAYMENT_STATUS_REVIEW,
      'unilateral' => PAYPAL_PAYMENT_STATUS_UNILATERAL,
      'upgrade' => PAYPAL_PAYMENT_STATUS_UPGRADE,
      'verify' => PAYPAL_PAYMENT_STATUS_VERIFY,
      'other' => PAYMENT_STATUS_PENDING,
    );
  }

  /**
   * Converts a PayPal status to a Payment status.
   *
   * @param array $ipn_variables
   *
   * @return string
   */
  public static function convertStatus(array $ipn_variables) {
    if (isset($ipn_variables['payment_status'])) {
      $paypal_status = $ipn_variables['payment_status'];
      if ($paypal_status == 'Pending') {
        if (isset($ipn_variables['pending_reason'])) {
          $status_map = self::pendingStatusMap();
          $pending_reason = $ipn_variables['pending_reason'];
          return isset($status_map[$pending_reason]) ? $status_map[$pending_reason] : PAYMENT_STATUS_PENDING;
        }
        return PAYMENT_STATUS_PENDING;
      }
      else {
        $status_map = self::statusMap();
        return isset($status_map[$paypal_status]) ? $status_map[$paypal_status] : PAYMENT_STATUS_UNKNOWN;
      } 
    }
    return PAYMENT_STATUS_UNKNOWN;
  }

}
