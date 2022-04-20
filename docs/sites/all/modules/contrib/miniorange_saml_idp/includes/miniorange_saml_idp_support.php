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
	  $customer = new MiniorangeSAMLIdpCustomer(NULL,NULL,NULL,NULL);
	  $response = $customer->callService($url,$fields);
    return $response=== 'Query submitted.';
	}
}
