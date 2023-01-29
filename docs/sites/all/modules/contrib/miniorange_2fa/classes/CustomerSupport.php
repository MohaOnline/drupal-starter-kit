<?php
/**
 * @file
 * Contains miniOrange Support class.
 */

/**
 * This class represents support information for customer.
 */
class Miniorange2FASupport {
  public $email;
  public $phone;
  public $query;

  /**
   * Constructor.
   */
  public function __construct($email, $phone, $query) {
    $this->email = $email;
    $this->phone = $phone;
    $this->query = $query;
  }

  /**
   * Send support query.
   */
  public function sendSupportQuery() {
    $this->query = '[Drupal-7 2FA Module] ' . $this->query;
    $fields = array (
        'company' => $_SERVER['SERVER_NAME'],
        'email' => $this->email,
        'phone' => $this->phone,
        'ccEmail' => 'drupalsupport@xecurify.com',
        'query' => $this->query,
        'subject' => "Drupal-7 2FA Module Query",
    );
    $field_string = json_encode($fields);
    $url = MoAuthConstants::getBaseUrl().MoAuthConstants::$SUPPORT_QUERY;
    $customerKey  = variable_get('mo_auth_customer_id', '');
    $apiKey = variable_get('mo_auth_customer_api_key', '');
    $response = MoAuthUtilities::callService($customerKey, $apiKey, $url, $field_string,false);
    return $response === 'Query submitted.';

  }
}
