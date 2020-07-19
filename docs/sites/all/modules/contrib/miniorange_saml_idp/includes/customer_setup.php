<?php
/**
 * @file
 * Contains miniOrange Customer class.
 */

/**
 * @file
 * This class represents configuration for customer.
 */
class MiniorangeSAMLIdpCustomer {

  public $email;

  public $phone;

  public $customerKey;

  public $transactionId;

  public $password;

  public $otpToken;

  private $defaultCustomerId;

  private $defaultCustomerApiKey;

  /**
   * Constructor.
   */
  public function __construct($email, $phone, $password, $otp_token) {
    $this->email = $email;
    $this->phone = $phone;
    $this->password = $password;
    $this->otpToken = $otp_token;
    $this->defaultCustomerId = "16555";
    $this->defaultCustomerApiKey = "fFd2XcvTGDemZvbw1bcUesNJWEqKbbUq";
  }

  /**
   * Check if customer exists.
   */
  public function checkCustomer() {
    if (!Utilities::isCurlInstalled()) {
      return json_encode(array(
        "status" => 'CURL_ERROR',
        "statusMessage" => '<a href="http://php.net/manual/en/curl.installation.php">PHP cURL extension</a> is not installed or disabled.',
      ));
    }

    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/rest/customer/check-if-exists';
    $ch = curl_init($url);
    $email = $this->email;

    $fields = array(
      'email' => $email,
    );
    $field_string = json_encode($fields);

    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($ch, CURLOPT_ENCODING, "");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

    curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json', 'charset: UTF - 8',
      'Authorization: Basic',
    ));
    curl_setopt($ch, CURLOPT_POST, TRUE);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
    $content = curl_exec($ch);
    if (curl_errno($ch)) {
      $error = array(
        '%method' => 'checkCustomer',
        '%file' => 'customer_setup.php',
        '%error' => curl_error($ch),
      );
      watchdog('miniorange_saml_idp', 'Error at %method of %file: %error', $error);
    }
    curl_close($ch);

    return $content;
  }

  /**
   * Create Customer.
   */
  public function createCustomer() {
    if (!Utilities::isCurlInstalled()) {
      return json_encode(array(
        "statusCode" => 'ERROR',
        "statusMessage" => '. Please check your configuration.',
      ));
    }
    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/rest/customer/add';
    $ch = curl_init($url);

    $fields = array(
      'companyName' => $_SERVER['SERVER_NAME'],
      'areaOfInterest' => 'DRUPAL IDP Module',
      'email' => $this->email,
      'phone' => $this->phone,
      'password' => $this->password,
    );
    $field_string = json_encode($fields);

    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($ch, CURLOPT_ENCODING, "");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

    curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json',
      'charset: UTF - 8',
      'Authorization: Basic',
    ));
    curl_setopt($ch, CURLOPT_POST, TRUE);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
    $content = curl_exec($ch);

    if (curl_errno($ch)) {
      $error = array(
        '%method' => 'createCustomer',
        '%file' => 'customer_setup.php',
        '%error' => curl_error($ch),
      );
      watchdog('miniorange_saml_idp', 'Error at %method of %file: %error', $error);
    }
    curl_close($ch);
    return $content;
  }

  /**
   * Get Customer Keys.
   */
  public function getCustomerKeys() {
    if (!Utilities::isCurlInstalled()) {
      return json_encode(array(
        "apiKey" => 'CURL_ERROR',
        "token" => '<a href="http://php.net/manual/en/curl.installation.php">PHP cURL extension</a> is not installed or disabled.',
      ));
    }

    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/rest/customer/key';
    $ch = curl_init($url);
    $email = $this->email;
    $password = $this->password;

    $fields = array(
      'email' => $email,
      'password' => $password,
    );
    $field_string = json_encode($fields);

    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($ch, CURLOPT_ENCODING, "");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

    curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json',
      'charset: UTF - 8',
      'Authorization: Basic',
    ));
    curl_setopt($ch, CURLOPT_POST, TRUE);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
    $content = curl_exec($ch);
    if (curl_errno($ch)) {
      $error = array(
        '%method' => 'getCustomerKeys',
        '%file' => 'customer_setup.php',
        '%error' => curl_error($ch),
      );
      watchdog('miniorange_saml_idp', 'Error at %method of %file: %error', $error);
    }
    curl_close($ch);

    return $content;
  }

  /**
   * Send OTP.
   */
  public function sendOtp() {
    if (!Utilities::isCurlInstalled()) {
      return json_encode(array(
        "status" => 'CURL_ERROR',
        "statusMessage" => '<a href="http://php.net/manual/en/curl.installation.php">PHP cURL extension</a> is not installed or disabled.',
      ));
    }
    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/api/auth/challenge';
    $ch = curl_init($url);
    $customer_key = $this->defaultCustomerId;
    $api_key = $this->defaultCustomerApiKey;

    $username = variable_get('miniorange_saml_idp_customer_admin_email', NULL);

    /* Current time in milliseconds since midnight, January 1, 1970 UTC. */
    $currentTimeInMillis = round(microtime(TRUE) * 1000);

    /* Creating the Hash using SHA-512 algorithm */
    $string_to_hash = $customer_key . number_format($currentTimeInMillis, 0, '', '' ) . $api_key;
    $hash_value = hash("sha512", $string_to_hash);

    $customer_key_header = "Customer-Key: " . $customer_key;
    $timestamp_header = "Timestamp: " . number_format($currentTimeInMillis, 0, '', '' );
    $authorization_header = "Authorization: " . $hash_value;

    $fields = array(
      'customerKey' => $customer_key,
      'email' => $username,
      'authType' => 'EMAIL',
    );
    $field_string = json_encode($fields);

    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($ch, CURLOPT_ENCODING, "");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

    curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: application/json", $customer_key_header,
      $timestamp_header, $authorization_header,
    ));
    curl_setopt($ch, CURLOPT_POST, TRUE);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
    $content = curl_exec($ch);

    if (curl_errno($ch)) {
      $error = array(
        '%method' => 'sendOtp',
        '%file' => 'customer_setup.php',
        '%error' => curl_error($ch),
      );
      watchdog('miniorange_saml_idp', 'Error at %method of %file: %error', $error);
    }
    curl_close($ch);
    return $content;
  }

  /**
   * Validate OTP.
   */
  public function validateOtp($transaction_id) {
    if (!Utilities::isCurlInstalled()) {
      return json_encode(array(
        "status" => 'CURL_ERROR',
        "statusMessage" => '<a href="http://php.net/manual/en/curl.installation.php">PHP cURL extension</a> is not installed or disabled.',
      ));
    }

    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/api/auth/validate';
    $ch = curl_init($url);

    $customer_key = $this->defaultCustomerId;
    $api_key = $this->defaultCustomerApiKey;

    $currentTimeInMillis = round(microtime(TRUE) * 1000);

    $string_to_hash = $customer_key . number_format($currentTimeInMillis, 0, '', '' ) . $api_key;
    $hash_value = hash("sha512", $string_to_hash);

    $customer_key_header = "Customer-Key: " . $customer_key;
    $timestamp_header = "Timestamp: " . number_format($currentTimeInMillis, 0, '', '' );
    $authorization_header = "Authorization: " . $hash_value;

    $fields = array(
      'txId' => $transaction_id,
      'token' => $this->otpToken,
    );

    $field_string = json_encode($fields);

    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($ch, CURLOPT_ENCODING, "");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

    curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: application/json", $customer_key_header,
      $timestamp_header, $authorization_header,
    ));
    curl_setopt($ch, CURLOPT_POST, TRUE);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
    $content = curl_exec($ch);

    if (curl_errno($ch)) {
      $error = array(
        '%method' => 'validateOtp',
        '%file' => 'customer_setup.php',
        '%error' => curl_error($ch),
      );
      watchdog('miniorange_saml_idp', 'Error at %method of %file: %error', $error);
    }
    curl_close($ch);
    return $content;
  }
}