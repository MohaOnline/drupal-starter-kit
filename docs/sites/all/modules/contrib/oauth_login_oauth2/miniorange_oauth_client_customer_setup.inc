
<?php
require_once 'includes/customer_setup.php';
/**
 * Customer setup form().
 */

function miniorange_oauth_client_customer_setup($form, &$form_state)
 {
     drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
     drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
     drupal_add_js( drupal_get_path('module', 'oauth_login_oauth2') . '/js/slide_support_button.js');

     $current_status = variable_get('miniorange_oauth_client_status', '');

    if ($current_status == 'VALIDATE_OTP') {

        $form['mo_header_style'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

        $form['mo_top_div'] = array(
            '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
        );

        $form['miniorange_oauth_client_customer_otp_token'] = array(
          '#type' => 'textfield',
          '#title' => t('OTP'),
          '#attributes' => array('style' => 'width:30%;'),
        );

        $form['miniorange_oauth_client_customer_validate_otp_button'] = array(
          '#type' => 'submit',
          '#value' => t('Validate OTP'),
          '#submit' => array('miniorange_oauth_client_validate_otp_submit'),
          '#id' => 'button_config',
        );

        $form['miniorange_oauth_client_customer_setup_resendotp'] = array(
          '#type' => 'submit',
          '#value' => t('Resend OTP'),
          '#submit' => array('miniorange_oauth_client_resend_otp'),
          '#id' => 'button_config',
        );

        $form['miniorange_oauth_client_customer_setup_back'] = array(
          '#type' => 'submit',
          '#value' => t('Back'),
          '#submit' => array('miniorange_oauth_client_back'),
          '#id' => 'button_config',
        );

        $form['mo_header_style_end'] = array('#markup' => '</div>');
        Utilities::spConfigGuide($form, $form_state);
        Utilities::AddSupportButton($form, $form_state);

        return $form;
    }
    elseif ($current_status == 'PLUGIN_CONFIGURATION') {

        // Show customer configuration here.
        $form['mo_header_style'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

        $form['mo_top_div'] = array(
            '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
        );


        $form['markup_top'] = array(
          '#markup' => '<div style="display:block;margin-top:10px;text-align: center;font-size: 15px;color:rgba(0, 128, 0, 0.80);background-color:rgba(0, 255, 0, 0.15);padding:5px;">
                Thank you for registering with miniOrange</div>'
          . '<br><br><h4>Your Profile: </h4>',
        );

        $header = array(
          'email' => array('data' => t('Customer Email')),
          'customerid' => array('data' => t('Customer ID')),
          'token' => array('data' => t('Token Key')),
          'apikey' => array('data' => t('API Key')),
        );

        $options = array();

        $options[0] = array(
          'email' => variable_get('miniorange_oauth_client_customer_admin_email', ''),
          'customerid' => variable_get('miniorange_oauth_client_customer_id', ''),
          'token' => variable_get('miniorange_oauth_client_customer_admin_token', ''),
          'apikey' => variable_get('miniorange_oauth_client_customer_api_key', ''),
        );

        $form['fieldset']['customerinfo'] = array(
          '#theme' => 'table',
          '#header' => $header,
          '#rows' => $options,
        );

        $form['mo_header_style_end'] = array('#markup' => '<br><br><br><br><br></div>');

        Utilities::spConfigGuide($form, $form_state);
        Utilities::AddSupportButton($form, $form_state);

        return $form;
    }

    $form['mo_header_style'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

    $form['mo_top_div'] = array(
        '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
    );

    $form['markup_1'] = array(
        '#markup' => '<h3>Register/Login with miniOrange</h3>',
        '#suffix' => '<hr><br>'
    );

     $form['markup_2'] = array(
         '#markup' => '<div class="mo_oauth_highlight_background_note_export"><p><h3>Why Should I register?</h3></p>
           <b>You will be needing a miniOrange account to upgrade to the Standard, Premium or Enterprise versions of the modules.</b><br>
             If you are facing any issues with registering from module, you can click <a href="https://www.miniorange.com/businessfreetrial" target="_blank"> here</a> to register.<br>
             We do not store any information except the email that you will use to register with us.<br>             
             </div>',
     );

    $form['miniorange_oauth_client_customer_setup_username'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#prefix' => '<br>',
        '#attributes' => array('style' => 'width:50%;', 'placeholder' => 'Enter your email'),
    );

    $form['miniorange_oauth_client_customer_setup_phone'] = array(
        '#type' => 'textfield',
        '#title' => t('Phone'),
        '#description' => '<b>NOTE:</b> We will only call if you need support.',
        '#attributes' => array('style' => 'width:50%;'),
    );

    $form['miniorange_oauth_client_customer_setup_password'] = array(
        '#type' => 'password_confirm',
    );

    $form['miniorange_oauth_client_customer_setup_button'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#id' => 'button_config',
        '#submit' => array('miniorange_oauth_client_customer_setup_submit'),
    );

     $form['markup_divEnd'] = array(
         '#markup' => '</div>'
     );
     Utilities::spConfigGuide($form, $form_state);
     Utilities::AddSupportButton($form, $form_state);


     return $form;
 }

/**
 * Validate OTP.
 */
function miniorange_oauth_client_validate_otp_submit(&$form, $form_state) {

  $otp_token = trim($form['miniorange_oauth_client_customer_otp_token']['#value']);
  $username = variable_get('miniorange_oauth_client_customer_admin_email', NULL);
  $phone = variable_get('miniorange_oauth_client_customer_admin_phone', NULL);
  $tx_id = variable_get('miniorange_oauth_client_tx_id', NULL);
  $customer_config = new MiniorangeOAuthCustomer($username, $phone, NULL, $otp_token);

  // Validate OTP.
  $validate_otp_response = json_decode($customer_config->validateOtp($tx_id));
  if ($validate_otp_response->status == 'SUCCESS') {
    // OTP Validated. Show Configuration page.
    $current_status = 'PLUGIN_CONFIGURATION';
    variable_set('miniorange_oauth_client_status', $current_status);
    variable_del('miniorange_oauth_client_tx_id');

    // OTP Validated. Create customer.
    $password = variable_get('miniorange_oauth_client_customer_admin_password', '');
    $customer_config = new MiniorangeOAuthCustomer($username, $phone, $password, NULL);
    $create_customer_response = json_decode($customer_config->createCustomer());
    if ($create_customer_response->status == 'SUCCESS') {
      // Customer created.
       $current_status = 'PLUGIN_CONFIGURATION';
      variable_set('miniorange_oauth_client_status', $current_status);
      variable_set('miniorange_oauth_client_customer_admin_email', $username);
      variable_set('miniorange_oauth_client_customer_admin_phone', $phone);
      variable_set('miniorange_oauth_client_customer_admin_token', $create_customer_response->token);
      variable_set('miniorange_oauth_client_customer_id', $create_customer_response->id);
      variable_set('miniorange_oauth_client_customer_api_key', $create_customer_response->apiKey);

      drupal_set_message(t('Customer account created.'));
    }
    else if(trim($create_customer_response->message) == 'Email is not enterprise email.' || ($create_customer_response->status) == 'INVALID_EMAIL_QUICK_EMAIL')
    {
            drupal_set_message(t('There was an error creating an account for you. You may have entered an invalid Email-Id
            <strong>(We discourage the use of disposable emails) </strong>
            <br>Please try again with a valid email.'), 'error');
            variable_set('miniorange_oauth_client_status', '');
           return;
    }
    else {
      drupal_set_message(t('Error creating customer'), 'error');
      return;
    }
  }
  else {
    drupal_set_message(t('Error validating OTP'), 'error');
    return;
  }

}

/**
 * Resend OTP.
 */
function miniorange_oauth_client_resend_otp(&$form, $form_state) {

  variable_del('miniorange_oauth_client_tx_id');
  $username = variable_get('miniorange_oauth_client_customer_admin_email', NULL);
  $phone = variable_get('miniorange_oauth_client_customer_admin_phone', NULL);
  $customer_config = new MiniorangeOAuthCustomer($username, $phone, NULL, NULL);
  $send_otp_response = json_decode($customer_config->sendOtp());
  if ($send_otp_response->status == 'SUCCESS') {
    // Store txID.
    variable_set('miniorange_oauth_client_tx_id', $send_otp_response->txId);
    $current_status = 'VALIDATE_OTP';
    variable_set('miniorange_oauth_client_status', $current_status);
    drupal_set_message(t('The passcode has been resent to @username', array('@username' => $username)));
  }
}



/**
* Handle submit for customer setup.
*/
function miniorange_oauth_client_customer_setup_submit(&$form, $form_state)
{

  $username = trim($form['miniorange_oauth_client_customer_setup_username']['#value']);
  $phone = $form['miniorange_oauth_client_customer_setup_phone']['#value'];
  $password = $form['miniorange_oauth_client_customer_setup_password']['#value']['pass1'];
    if(empty($username)||empty($password)){
        if(empty($username)) {
            drupal_set_message(t('The <b>Email Address</b> field is required.'), 'error');
        }
        if(empty($password)) {
            drupal_set_message(t('The <b>Password</b> field is required.'), 'error');
        }
        return;
    }
    if (!valid_email_address($username)) {
        drupal_set_message(t('The email address <b><u>' . $username . '</u></b> is not valid.'), 'error');
        return;
    }

  $customer_config = new MiniorangeOAuthCustomer($username, $phone, $password, NULL);

  $check_customer_response = json_decode($customer_config->checkCustomer());

  if ($check_customer_response->status == 'CUSTOMER_NOT_FOUND') {
    // Create customer.
    // Store email and phone.

    variable_set('miniorange_oauth_client_customer_admin_email', $username);
    variable_set('miniorange_oauth_client_customer_admin_phone', $phone);
    variable_set('miniorange_oauth_client_customer_admin_password', $password);

    $send_otp_response = json_decode($customer_config->sendOtp());
    if ($send_otp_response->status == 'SUCCESS') {
      // Store txID.
      variable_set('miniorange_oauth_client_tx_id', $send_otp_response->txId);
      $current_status = 'VALIDATE_OTP';
      variable_set('miniorange_oauth_client_status', $current_status);
      drupal_set_message(t('Verify email address by entering the passcode sent to @username', array('@username' => $username)));
    }
  }
  elseif ($check_customer_response->status == 'CURL_ERROR') {
    drupal_set_message(t('cURL is not enabled. Please enable cURL'), 'error');
    return;
  }
  elseif ($check_customer_response->status == 'TRANSACTION_LIMIT_EXCEEDED') {
      drupal_set_message(t('An error has been occured. Please try after sometime or contact us at <a href="mailto:drupalsupport@xecurify.com">drupalsupport@xecurify.com</a>'), 'error');
      return;
  }
  else
  {
    // Customer exists. Retrieve keys.
    $customer_keys_response = json_decode($customer_config->getCustomerKeys());
    if (json_last_error() == JSON_ERROR_NONE) {
      variable_set('miniorange_oauth_client_customer_id', $customer_keys_response->id);
      variable_set('miniorange_oauth_client_customer_admin_token', $customer_keys_response->token);
      variable_set('miniorange_oauth_client_customer_admin_email', $username);
      variable_set('miniorange_oauth_client_customer_admin_phone', $phone);
      variable_set('miniorange_oauth_client_customer_api_key', $customer_keys_response->apiKey);
      $current_status = 'PLUGIN_CONFIGURATION';
      variable_set('miniorange_oauth_client_status', $current_status);
      drupal_set_message(t('Successfully retrieved your account.'));
    }
    else {
      drupal_set_message(t('Invalid credentials'), 'error');
      return;
    }
            $current_status = 'PLUGIN_CONFIGURATION';
            variable_set('miniorange_oauth_client_status', $current_status);
  }
}

/**
 * Handle back button submit for customer setup.
 */
function miniorange_oauth_client_back(&$form, $form_state) {
  $current_status = 'CUSTOMER_SETUP';
  variable_set('miniorange_oauth_client_status', $current_status);
  variable_del('miniorange_oauth_client_customer_admin_email');
  variable_del('miniorange_oauth_client_customer_admin_phone');
  variable_del('miniorange_oauth_client_tx_id');
  drupal_set_message(t('Register/Login with your miniOrange Account'));
}

/**
 * Send support query.
 */
function send_support_query(&$form, $form_state)
{
    $email = trim($form['miniorange_oauth_email_address_support']['#value']);
    $phone = $form['miniorange_oauth_phone_number_support']['#value'];
    $query = trim($form['miniorange_oauth_support_query_support']['#value']);
    Utilities::send_query($email, $phone, $query, null);
}
function rfd(&$form, $form_state) {

    global $base_url;
    drupal_goto($base_url.'/admin/config/people/oauth_login_oauth2/request_for_demo');
}

?>