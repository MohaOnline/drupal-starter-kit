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

    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/rest/customer/check-if-exists';

    $fields = array(
      'email' => $this->email,
    );
   return $this->callService($url,$fields);
  }

  /**
   * Create Customer.
   */
  public function createCustomer() {

    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/rest/customer/add';

    $fields = array(
      'companyName' => $_SERVER['SERVER_NAME'],
      'areaOfInterest' => 'DRUPAL IDP Module',
      'email' => $this->email,
      'phone' => $this->phone,
      'password' => $this->password,
    );
    return $this->callService($url,$fields);
  }

  /**
   * Get Customer Keys.
   */
  public function getCustomerKeys() {

    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/rest/customer/key';
    $fields = array(
      'email' => $this->email,
      'password' => $this->password,
    );

    return $this->callService($url,$fields);
  }

  /**
   * Send OTP.
   */
  public function sendOtp() {

    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/api/auth/challenge';

    $customer_key = $this->defaultCustomerId;
    $username = variable_get('miniorange_saml_idp_customer_admin_email', NULL);

    $fields = array(
      'customerKey' => $customer_key,
      'email' => $username,
      'authType' => 'EMAIL',
    );
    return $this->callService($url,$fields,TRUE);
  }

  /**
   * Validate OTP.
   */
  public function validateOtp($transaction_id) {

    $url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/api/auth/validate';

    $fields = array(
      'txId' => $transaction_id,
      'token' => $this->otpToken,
    );


    return $this->callService($url,$fields,TRUE);
  }

  function callService($url,$fields,$addExtendedHeader=FALSE,$logError=TRUE){
    if (!Utilities::isCurlInstalled()) {
      return json_encode(array(
        "statusCode" => 'ERROR',
        "statusMessage" => 'cURL is not enabled on your site. Please enable the cURL module.',
      ));
    }
    $fieldString = is_string($fields)?$fields:json_encode($fields);

    $header = $this->getHeader($addExtendedHeader);

    $response = drupal_http_request($url,[
      'data' => $fieldString,
      'method'=>'POST',
      'allow_redirects' => TRUE,
      'http_errors' => FALSE,
      'decode_content'  => true,
      'verify' => FALSE,
      'headers' =>$header
    ]);

    if(isset($response->error) && $response->code != 200 && $logError)
    {
        $error = array(
          '%apiName' => explode("moas",$url)[1],
          '%error' => $response->error,
        );
        watchdog('miniorange_saml_idp','Error at %apiName of  %error', $error);
    }
    return $response->data;

  }
  function getHeader($addExtendedHeader=FALSE){

    $header = array(
      'Content-Type'=>'application/json', 'charset'=>'UTF - 8',
      'Authorization'=> 'Basic',
    );

    if($addExtendedHeader){
      /* Current time in milliseconds since midnight, January 1, 1970 UTC. */
      $current_time_in_millis = $this->getTimeStamp();


      /* Creating the Hash using SHA-512 algorithm */
      list($customerId,$apiKey) = $this->getCustomerDetails();

      $string_to_hash = $customerId .$current_time_in_millis . $apiKey;
      $hashValue = hash("sha512", $string_to_hash);
      $timestamp_header = number_format($current_time_in_millis, 0, '', '' );
      $header=array_merge($header,array("Customer-Key"=>$customerId,
        "Timestamp"=>$timestamp_header, "Authorization"=>$hashValue));

    }
    return $header;
  }
  public  function getTimeStamp()
  {
    $url = MiniorangeSAMLIdpConstants::BASE_URL.'/moas/rest/mobile/get-timestamp';
    $fields = array();
    $currentTimeInMillis = $this->callService($url,$fields);
    if (empty($currentTimeInMillis)) {
      $currentTimeInMillis = round(microtime(true) * 1000);
      $currentTimeInMillis = number_format($currentTimeInMillis, 0, '', '');
    }
    return $currentTimeInMillis;
  }


  public function  getCustomerDetails(){
    $customerKey = variable_get('miniorange_saml_idp_customer_id');
    $apikey = variable_get('miniorange_saml_idp_customer_api_key');

    if(!is_null($customerKey) && !is_null($apikey) && !empty($customerKey) && !empty($apikey)){
      return array($customerKey,$apikey);
    }
    else
      return array($this->defaultCustomerId,$this->defaultCustomerApiKey);


  }




}
