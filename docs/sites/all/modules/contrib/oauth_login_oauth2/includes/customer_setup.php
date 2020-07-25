<?php
/**
 * @file
 * Contains miniOrange Customer class.
 */

/**
 * @file
 * This class represents configuration for customer.
 */
class MiniorangeOAuthCustomer {

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

    $url = MiniorangeOAuthConstants::BASE_URL . '/moas/rest/customer/check-if-exists';
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
      watchdog('oauth_login_oauth2', 'Error at %method of %file: %error', $error);
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
    $url = MiniorangeOAuthConstants::BASE_URL . '/moas/rest/customer/add';
    $ch = curl_init($url);

    $fields = array(
      'companyName' => $_SERVER['SERVER_NAME'],
      'areaOfInterest' => 'Drupal 7 OAuth Client Module ',
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
      watchdog('oauth_login_oauth2', 'Error at %method of %file: %error', $error);
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

    $url = MiniorangeOAuthConstants::BASE_URL . '/moas/rest/customer/key';
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
      watchdog('oauth_login_oauth2', 'Error at %method of %file: %error', $error);
    }
    curl_close($ch);

    return $content;
  }

  /**
   * Send OTP.
   */
  public function sendOtp() {
    $currentTimeInMillis = '';
    if (!Utilities::isCurlInstalled()) {
      return json_encode(array(
        "status" => 'CURL_ERROR',
        "statusMessage" => '<a href="http://php.net/manual/en/curl.installation.php">PHP cURL extension</a> is not installed or disabled.',
      ));
    }
  
    $url = MiniorangeOAuthConstants::BASE_URL . '/moas/api/auth/challenge';
    $ch = curl_init($url);
    $customer_key = $this->defaultCustomerId;
    $api_key = $this->defaultCustomerApiKey;

    $username = variable_get('miniorange_oauth_client_customer_admin_email',NULL);
   
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
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json', $customer_key_header,
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
      watchdog('oauth_login_oauth2', 'Error at %method of %file: %error', $error);
    }
    curl_close($ch);
    
    return $content;
  }

  /**
   * Validate OTP.
   */
  public function validateOtp($transaction_id) {
    $currentTimeInMillis = '';
    if (!Utilities::isCurlInstalled()) {
      return json_encode(array(
        "status" => 'CURL_ERROR',
        "statusMessage" => '<a href="http://php.net/manual/en/curl.installation.php">PHP cURL extension</a> is not installed or disabled.',
      ));
    }

    $url = MiniorangeOAuthConstants::BASE_URL . '/moas/api/auth/validate';
    $ch = curl_init($url);

    $customer_key = $this->defaultCustomerId;
    $api_key = $this->defaultCustomerApiKey;

    $currentTimeInMillis =  round(microtime(TRUE) * 1000);

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
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json', $customer_key_header,
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
      watchdog('oauth_login_oauth2', 'Error at %method of %file: %error', $error);
    }
    curl_close($ch);
    return $content;
  }
  
  function verifyLicense($code) {
		$url = MiniorangeOAuthConstants::BASE_URL . '/moas/api/backupcode/verify';
		$ch = curl_init ( $url );
		
		/* The customer Key provided to you */
		$customerKey = variable_get( 'miniorange_oauth_client_customer_id' );
		
		/* The customer API Key provided to you */
		$apiKey = variable_get( 'miniorange_oauth_client_customer_api_key' );
		
		Global $base_url;
		
		/* Current time in milliseconds since midnight, January 1, 1970 UTC. */
		$currentTimeInMillis = round(microtime(TRUE) * 1000);
		
		/* Creating the Hash using SHA-512 algorithm */
		$stringToHash = $customerKey . number_format($currentTimeInMillis, 0, '', '' ) . $apiKey;
		$hashValue = hash ( "sha512", $stringToHash );
		
		$customerKeyHeader = "Customer-Key: " . $customerKey;
		$timestampHeader = "Timestamp: " . number_format($currentTimeInMillis, 0, '', '' );
		$authorizationHeader = "Authorization: " . $hashValue;
		
		$fields = '';
		
		// *check for otp over sms/email
		$fields = array (
				'code' => $code ,
				'customerKey' => $customerKey,
				'additionalFields' => array(
					'field1' =>  $base_url
					
				)
				
		);
		
		$field_string = json_encode ( $fields );
		
		curl_setopt ( $ch, CURLOPT_FOLLOWLOCATION, true );
		curl_setopt ( $ch, CURLOPT_ENCODING, "" );
		curl_setopt ( $ch, CURLOPT_RETURNTRANSFER, true );
		curl_setopt ( $ch, CURLOPT_AUTOREFERER, true );
		curl_setopt ( $ch, CURLOPT_SSL_VERIFYPEER, false ); // required for https urls
		
		curl_setopt ( $ch, CURLOPT_MAXREDIRS, 10 );
		curl_setopt ( $ch, CURLOPT_HTTPHEADER, array (
				'Content-Type: application/json',
				$customerKeyHeader,
				$timestampHeader,
				$authorizationHeader 
		) );
		curl_setopt ( $ch, CURLOPT_POST, true );
		curl_setopt ( $ch, CURLOPT_POSTFIELDS, $field_string );
		curl_setopt ( $ch, CURLOPT_CONNECTTIMEOUT, 5 );
		curl_setopt ( $ch, CURLOPT_TIMEOUT, 20 );
		$content = curl_exec ( $ch );
		
		if (curl_errno ( $ch )) {
			echo 'Request Error:' . curl_error ( $ch );
			exit ();
		}
		
		curl_close ( $ch );
		return $content;
	}

	function updateStatus() {
		
		$url = MiniorangeOAuthConstants::BASE_URL . '/moas/api/backupcode/updatestatus';
		
		$ch = curl_init ( $url );
		$customerKey = variable_get( 'miniorange_oauth_client_customer_id' );
		$apiKey = variable_get( 'miniorange_oauth_client_customer_api_key' );
		$currentTimeInMillis = round(microtime(TRUE) * 1000);
		$stringToHash = $customerKey . number_format($currentTimeInMillis, 0, '', '' ) . $apiKey;
		$hashValue = hash ( "sha512", $stringToHash );
		$customerKeyHeader = "Customer-Key: " . $customerKey;
		$timestampHeader = "Timestamp: " . number_format($currentTimeInMillis, 0, '', '' );
		$authorizationHeader = "Authorization: " . $hashValue;
		$key = variable_get('miniorange_oauth_client_customer_admin_token');
		$code = AESEncryption::decrypt_data(variable_get('miniorange_oauth_client_license_key'),$key);
		$fields = array ( 'code' => $code , 'customerKey' => $customerKey);
		$field_string = json_encode ( $fields );
		curl_setopt ( $ch, CURLOPT_FOLLOWLOCATION, true );
		curl_setopt ( $ch, CURLOPT_ENCODING, "" );
		curl_setopt ( $ch, CURLOPT_RETURNTRANSFER, true );
		curl_setopt ( $ch, CURLOPT_AUTOREFERER, true );
		curl_setopt ( $ch, CURLOPT_SSL_VERIFYPEER, false ); // required for https urls
		curl_setopt ( $ch, CURLOPT_MAXREDIRS, 10 );
		curl_setopt ( $ch, CURLOPT_HTTPHEADER, array (
				'Content-Type: application/json',
				$customerKeyHeader,
				$timestampHeader,
				$authorizationHeader 
		) );
		curl_setopt ( $ch, CURLOPT_POST, true );
		curl_setopt ( $ch, CURLOPT_POSTFIELDS, $field_string );
		curl_setopt ( $ch, CURLOPT_CONNECTTIMEOUT, 5 );
		curl_setopt ( $ch, CURLOPT_TIMEOUT, 20 );
		$content = curl_exec ( $ch );
		if (curl_errno ( $ch )) {
			echo 'Request Error:' . curl_error ( $ch );
			exit ();
		}
		curl_close ( $ch );
		return $content;
	}
}