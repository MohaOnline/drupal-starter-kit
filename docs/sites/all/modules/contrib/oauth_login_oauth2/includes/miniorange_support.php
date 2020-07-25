<?php
/**
 * @file
 * Contains miniOrange OAuth Client Support class.
 */

/**
 * @file
 * This class represents support information for customer.
 */
class MiniorangeOAuthClientSupport
{

    public $email;
    public $phone;
    public $query;
    public $plan;

    /**
     * Constructor.
     */
    public function __construct($email, $phone, $query, $plan = '')
    {
        $this->email = $email;
        $this->phone = $phone;
        $this->query = $query;
        $this->plan = $plan;
    }

    /**
     * Send support query.
     */
    public function sendSupportQuery()
    {
        $modules_info = system_get_info('module', 'oauth_login_oauth2');
        $modules_version = $modules_info['version'];

        if ($this->plan == 'demo') {
            $subject = "Drupal-7 OAuth Login Request For Demo | " .$modules_version ;
            $this->query = '[Drupal 7 OAuth Login Request For Demo] ' . $this->query;
            $fields = array(
                'company' => $_SERVER ['SERVER_NAME'],
                'email' => $this->email,
                'phone' => $this->phone,
                'ccEmail' => 'drupalsupport@xecurify.com',
                'query' => $this->query,
                'subject' => $subject,
            );
            $url = MiniorangeOAuthConstants::BASE_URL . '/moas/rest/customer/contact-us';
            $ch = curl_init($url);
            curl_setopt($ch, CURLOPT_HTTPHEADER, array(
                'Content-Type: application/json',
                'charset: UTF-8',
                'Authorization: Basic'
            ));
        } else {
            $this->query = '[Drupal 7 OAuth Login - Free | '.$modules_version.' ] ' . $this->query;
            $fields = array(
                'company' => $_SERVER ['SERVER_NAME'],
                'email' => $this->email,
                'phone' => $this->phone,
                'ccEmail' => 'drupalsupport@xecurify.com',
                'query' => $this->query,
                'subject' => "Drupal-7 OAuth Login Query - Free",
            );
            $url = MiniorangeOAuthConstants::BASE_URL . '/moas/rest/customer/contact-us';
            $ch = curl_init($url);
            curl_setopt($ch, CURLOPT_HTTPHEADER, array(
                'Content-Type: application/json',
                'charset: UTF-8',
                'Authorization: Basic'
            ));
        }
        $field_string = json_encode($fields);
        curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
        curl_setopt($ch, CURLOPT_ENCODING, "");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
        curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
        curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
        $content = curl_exec($ch);
        if (curl_errno($ch)) {
            $error = array(
                '%method' => 'sendSupportQuery',
                '%file' => 'miniorange_support.php',
                '%error' => curl_error($ch),
            );
            watchdog('oauth_login_oauth2', 'cURL Error at %method of %file: %error', $error);
            return FALSE;
        }
        curl_close($ch);
        return TRUE;
    }
}
