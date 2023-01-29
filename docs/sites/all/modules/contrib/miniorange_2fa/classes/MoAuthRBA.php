<?php

class MoAuthRBA
{
   public static function mo2f_collect_attributes( $useremail, $rba_attributes ) {
    $url          = MoAuthConstants::getBaseUrl() . '/rest/rba/acs';
    $customerKey  = variable_get('mo_auth_customer_id', '');
    $apiKey = variable_get('mo_auth_customer_api_key', '');

     $fields = "{\"customerKey\":\"" . $customerKey . "\",\"userKey\":\"" . $useremail . "\",\"attributes\":" . $rba_attributes . "}";

    $response = MoAuthUtilities::callService($customerKey, $apiKey, $url, $fields,false);
    return $response;

  }


  public static function mo2f_evaluate_risk($useremail,$sessionUuid){
    $url = MoAuthConstants::getBaseUrl() . '/rest/rba/evaluate-risk';

    $customerKey  = variable_get('mo_auth_customer_id', '');
    /* The customer API Key provided to you */
    $apiKey = variable_get('mo_auth_customer_api_key', '');

    $appSecret = variable_get('mo_auth_customer_app_secret', '');
    $fields = array(
      'customerKey' => $customerKey,
      'appSecret' => $appSecret,
      'userKey' => $useremail,
      'sessionUuid' => $sessionUuid
    );
    $field_string = json_encode( $fields );
    $response = MoAuthUtilities::callService($customerKey, $apiKey, $url, $field_string,false);
    return $response;

  }


  public static function mo2f_register_rba_profile($useremail,$sessionUuid){
    $url = MoAuthConstants::getBaseUrl() . '/rest/rba/register-profile';

    $customerKey  = variable_get('mo_auth_customer_id', '');
    $apiKey = variable_get('mo_auth_customer_api_key', '');
    $fields = array(
      'customerKey' => $customerKey,
      'userKey' => $useremail,
      'sessionUuid' => $sessionUuid
    );
    $field_string = json_encode( $fields );
    $response = MoAuthUtilities::callService($customerKey, $apiKey, $url, $field_string,false);
    return $response;
  }
}
