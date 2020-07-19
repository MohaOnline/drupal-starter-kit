<?php
/**
 * @file
 * Contains miniOrange Support class.
 */

/**
 * @file
 * This class represents support information for customer.
 */
class MiniorangeSAMLIdpSupport {

	public $email;
	public $phone;
	public $query;
	public $plan;

	/**
     * Constructor.
     */
	public function __construct($email, $phone, $query, $plan = '' ) {
      $this->email = $email;
      $this->phone = $phone;
      $this->query = $query;
	  $this->plan  = $plan;
	}

	/**
	 * Send support query.
	 */
	public function sendSupportQuery() {
	  $version = system_get_info('module','miniorange_saml_idp')['version'];
	  $drupalCoreVersion = VERSION;

		if ($this->plan == 'demo') {
            $subject = "Demo request for Drupal-". $drupalCoreVersion ." IdP premimum Module";
            $this->query = 'Use case description - ' . $this->query;

            $customerKey = variable_get('miniorange_saml_idp_customer_id');
            $apikey = variable_get('miniorange_saml_idp_customer_api_key');
            $content = '<div > [Drupal-'. $drupalCoreVersion .' IdP premium demo ' . $version . ']' . $this->query . '</div>';
			$fields = array (
				'company' => $_SERVER ['SERVER_NAME'],
				'email' => $this->email,
				'ccEmail' => 'drupalsupport@xecurify.com',
				'phone' => $this->phone,
				'query' => $content,
				'subject' => $subject
			);
        }
		else{
			$this->query = '[Drupal-'. $drupalCoreVersion .' SAML IDP Free ' . $version . '] ' . $this->query;
			$fields = array (
				'company' => $_SERVER ['SERVER_NAME'],
				'email' => $this->email,
				'ccEmail' => 'drupalsupport@xecurify.com',
				'phone' => $this->phone,
				'query' => $this->query,
				'subject' => 'Drupal-'. $drupalCoreVersion .' SAML IDP Free Query',
			);
	    }
		$url = MiniorangeSAMLIdpConstants::BASE_URL . '/moas/rest/customer/contact-us';
		$ch = curl_init ( $url );
		curl_setopt($ch, CURLOPT_HTTPHEADER, array(
                'Content-Type: application/json',
                'charset: UTF-8',
                'Authorization: Basic'
        ));
		$field_string = json_encode ($fields);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
		curl_setopt($ch, CURLOPT_ENCODING, "");
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
		curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_setopt($ch, CURLOPT_MAXREDIRS, 10);

		curl_setopt($ch, CURLOPT_POST, TRUE);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
		$content = curl_exec($ch);
		if (curl_errno($ch)) {
        $error = array(
          '%method' => 'sendSupportQuery',
          '%file' => 'miniorange_saml_idp_support.php',
          '%error' => curl_error($ch),
        );
        watchdog('miniorange_saml_idp', 'cURL Error at %method of %file: %error', $error);
        return FALSE;
      }
      curl_close ($ch);
      return TRUE;
	}
}