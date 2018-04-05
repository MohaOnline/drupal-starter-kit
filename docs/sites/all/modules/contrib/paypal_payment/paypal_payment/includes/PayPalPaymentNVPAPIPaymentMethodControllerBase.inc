<?php

/**
 * Contains \PayPalPaymentNVPAPIPaymentMethodControllerBase.
 */

/**
 * A base class for payment method controllers that talk to PayPal's NVP API.
 */
abstract class PayPalPaymentNVPAPIPaymentMethodControllerBase extends PaymentMethodController {

  /**
   * The production server.
   */
  const NVP_API_SERVER_PRODUCTION = 0;

  /**
   * The sandbox server.
   */
  const NVP_API_SERVER_SANDBOX = 1;

  /**
   * The production API server URL.
   */
  const NVP_API_URL_SERVER_PRODUCTION = NULL;

  /**
   * The sandbox API server URL.
   */
  const NVP_API_URL_SERVER_SANDBOX = NULL;

  /**
   * The PayPAL API version that is used.
   */
  const NVP_API_PAYPAL_VERSION = '86';

  /**
   * {@inheritdoc}
   */
  public $controller_data_defaults = array(
    'password' => '',
    'server' => self::NVP_API_SERVER_PRODUCTION,
    'signature' => '',
    'username' => '',
  );

  /**
   * Returns the API server URL.
   *
   * @throws InvalidArgumentException
   *
   * @param int $server
   *   One of the self::NVP_API_SERVER_* constants.
   *
   * @return string
   */
  public function NVPAPIServerURL($server) {
    $urls = array(
      $this::NVP_API_SERVER_PRODUCTION => $this::NVP_API_URL_SERVER_PRODUCTION,
      $this::NVP_API_SERVER_SANDBOX => $this::NVP_API_URL_SERVER_SANDBOX,
    );
    if (array_key_exists($server, $urls)) {
      return url($urls[$server], array(
        'external' => TRUE,
      ));
    }
    else {
      throw new InvalidArgumentException(t('Server type does not exist.'));
    }
  }

  /**
   * Parses an API response.
   *
   * @param string $response
   *
   * @return array
   */
  public function NVPAPIParseResponse($response) {
    $nvp = array();
    foreach (explode('&', $response) as $variable) {
      $fragments = explode('=', $variable);
      if (count($fragments) == 2) {
        $nvp[$fragments[0]] = urldecode($fragments[1]);
      }
    }

    return $nvp;
  }

  /**
   * Executes an API request.
   *
   * @param array $nvp_request
   *   NVP variables to POST.
   * @param Payment $payment
   *
   * @return array|false
   *   The NVP response data or FALSE in case of failure.
   */
  public function NVPAPIRequest(array $nvp_request, Payment $payment) {
    $nvp_request = array(
      'USER' => $payment->method->controller_data['username'],
      'PWD' => $payment->method->controller_data['password'],
      'SIGNATURE' => $payment->method->controller_data['signature'],
      'VERSION' => self::NVP_API_PAYPAL_VERSION,
    ) + $nvp_request;

    $post_data = array();
    foreach ($nvp_request as $name => $value) {
      $post_data[] = $name . '=' . urlencode($value);
    }
    $post_data = implode('&', $post_data);

    $response = chr_curl_http_request($this->NVPAPIServerURL($payment->method->controller_data['server']), array(
      'method' => 'POST',
      'data' => $post_data,
      'curl_opts' => [
        CURLOPT_SSL_VERIFYPEER => TRUE,
        CURLOPT_SSL_VERIFYHOST => 2,
      ],
    ));

    if (isset($response->error)) {
      watchdog('paypal_payment_ec', 'An API request failed with error @code: %error.', array(
        '@code' => $response->code,
        '%error' => $response->error,
      ), WATCHDOG_ERROR);
      return FALSE;
    }
    else {
      $nvp_response = $this->NVPAPIParseResponse($response->data);
      if ($nvp_response
      && isset($nvp_response['ACK']) && in_array($nvp_response['ACK'], array('Success', 'SuccessWithWarning'))) {
        return $nvp_response;
      }
    }
    return FALSE;
  }

}
