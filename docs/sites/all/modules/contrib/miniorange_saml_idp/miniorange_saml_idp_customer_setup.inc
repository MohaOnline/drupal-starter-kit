<?php
/**
 * @file
 * Contains form for customer setup.
 */

/**
 * Customer setup form().
 */
include "mo_saml_visualTour.php";
function miniorange_saml_idp_customer_setup($form, &$form_state) {
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));


    global $base_url;
    $current_status = variable_get('miniorange_saml_idp_status', '');
    if ($current_status == 'MOIDP_VALIDATE_OTP') {

        $form['header_top_style_1'] = array('#markup' => '<div class="mo_saml_table_layout_1">');

        $form['markup_14'] = array(
            '#markup' => '<div class="mo_saml_table_layout mo_saml_container">',
        );

        $form['miniorange_saml_idp_customer_otp_token'] = array(
            '#type' => 'textfield',
            '#title' => t('OTP'),
            '#attributes' => array('style' => 'width:45%'),
        );

        $form['miniorange_saml_idp_customer_validate_otp_button'] = array(
            '#type' => 'submit',
            '#value' => t('Validate OTP'),
            '#submit' => array('miniorange_saml_idp_validate_otp_submit'),
        );

        $form['miniorange_saml_idp_customer_setup_resendotp'] = array(
            '#type' => 'submit',
            '#value' => t('Resend OTP'),
            '#submit' => array('miniorange_saml_idp_resend_otp'),
        );

        $form['miniorange_saml_idp_customer_setup_back'] = array(
            '#type' => 'submit',
            '#value' => t('Back'),
            '#submit' => array('miniorange_saml_idp_back'),
        );

        $form['miniorange_saml_support_end_div'] = array(
            '#markup' => '</div>'
        );

        Utilities::AddSupportButton($form, $form_state);

        return $form;
    }
    elseif ($current_status == 'MOIDP_PLUGIN_CONFIGURATION')
    {
        $form['header_top_style_1'] = array('#markup' => '<div class="mo_saml_table_layout_1">',
        );

        $form['header_top_style_2'] = array('#markup' => '<div class="mo_saml_table_layout mo_saml_container_register">');

        // Show customer configuration here.
        $form['markup_saml_idp_regsitration_message'] = array(
            '#markup' => '<div style="display:block;margin-top:10px;text-align: center;font-size: 15px;color:rgba(0, 128, 0, 0.80);background-color:rgba(0, 255, 0, 0.15);padding:5px;">
                        Thank you for registering with miniOrange
                    </div>'
        );

        $form['markup_idp_top'] = array(
            '#markup' => '<br/><br/><h4>Your Profile: </h4>',
        );

        $header = array(
            'email' => array('data' => t('Customer Email')),
            'customerid' => array('data' => t('Customer ID')),
            'token' => array('data' => t('Token Key')),
            'apikey' => array('data' => t('API Key')),
        );

        $options = array();

        $options[0] = array(
            'email' => variable_get('miniorange_saml_idp_customer_admin_email', ''),
            'customerid' => variable_get('miniorange_saml_idp_customer_id', ''),
            'token' => variable_get('miniorange_saml_idp_customer_admin_token', ''),
            'apikey' => variable_get('miniorange_saml_idp_customer_api_key', ''),
        );

        $form['fieldset']['customerinfo'] = array(
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $options,
        );

        $form['markup_idp_attr_header_top_div_close'] = array('#markup' => '<br/><br/><br/><br/><br/><br/><br/></div>',);

        Utilities::AddSupportButton($form, $form_state);

        return $form;
    }

    $form['header_top_style_1'] = array('#markup' => '<div class="mo_saml_table_layout_1">',);

    $form['markup_idp_header_1'] = array(
        '#markup' => '<div class="mo_saml_table_layout mo_saml_container_register"><b><span style="font-size: 19px;">Register/Login with miniOrange</span></b><br><br><hr><br/>',
    );

    $form['markup_reg_msg'] = array(
        '#markup' => '<div class="mo_saml_register_message">You need to Register with miniOrange before upgrading to any of our premium versions of the module.</center></div>',
    );

    $form['markup_idp_2'] = array(
        '#markup' => '<br>Just complete the short registration below to configure'
            . ' the SAML IDP Plugin. Please enter a valid email id <br>that you have'
            . ' access to. You will be able to move forward after verifying an OTP'
            . ' that we will send to this email.',
    );

    $form['miniorange_saml_idp_customer_setup_username'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#attributes' => array('style' => 'width:60%'),
    );

    $form['miniorange_saml_idp_customer_setup_phone'] = array(
        '#type' => 'textfield',
        '#title' => t('Phone'),
        '#attributes' => array('style' => 'width:60%'),
    );

    $form['markup_idp_3'] = array(
        '#markup' => '<b>NOTE:</b> We will only call if you need support.',
    );

    $form['miniorange_saml_idp_customer_setup_password'] = array(
        '#type' => 'password_confirm',
    );

    $form['miniorange_saml_idp_customer_setup_button'] = array(
        '#type' => 'submit',
        '#value' => t('Register'),
        '#submit' => array('miniorange_saml_idp_customer_setup_submit'),
        '#attributes' => array('style' => 'width:100px;background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;'),
    );

    $form['markup_divEnd'] = array(
        '#markup' => '</div>'
    );

    Utilities::AddSupportButton($form, $form_state);

    return $form;

}

/**
 * Validate OTP.
 */
function miniorange_saml_idp_validate_otp_submit(&$form, $form_state) {

    $otp_token = $form['miniorange_saml_idp_customer_otp_token']['#value'];
    $username = variable_get('miniorange_saml_idp_customer_admin_email', NULL);
    $phone = variable_get('miniorange_saml_idp_customer_admin_phone', NULL);
    $tx_id = variable_get('miniorange_saml_idp_tx_id', NULL);
    $customer_config = new MiniorangeSAMLIdpCustomer($username, $phone, NULL, $otp_token);

    // Validate OTP.
    $validate_otp_response = json_decode($customer_config->validateOtp($tx_id));
    if ($validate_otp_response->status == 'SUCCESS') {
        // OTP Validated. Show Configuration page.
        $current_status = 'MOIDP_PLUGIN_CONFIGURATION';
        variable_set('miniorange_saml_idp_status', $current_status);
        variable_del('miniorange_saml_idp_tx_id');

        // OTP Validated. Create customer.
        $password = variable_get('miniorange_saml_idp_customer_admin_password', '');
        $customer_config = new MiniorangeSAMLIdpCustomer($username, $phone, $password, NULL);
        $create_customer_response = json_decode($customer_config->createCustomer());
        if ($create_customer_response->status == 'SUCCESS') {
            // Customer created.
            $current_status = 'MOIDP_PLUGIN_CONFIGURATION';
            variable_set('miniorange_saml_idp_status', $current_status);
            variable_set('miniorange_saml_idp_customer_admin_email', $username);
            variable_set('miniorange_saml_idp_customer_admin_phone', $phone);
            variable_set('miniorange_saml_idp_customer_admin_token', $create_customer_response->token);
            variable_set('miniorange_saml_idp_customer_id', $create_customer_response->id);
            variable_set('miniorange_saml_idp_customer_api_key', $create_customer_response->apiKey);
            variable_del('miniorange_saml_idp_customer_admin_password');

            drupal_set_message(t('Customer account created.'));

            /*$redirect = $base_url.'/admin/config/people/miniorange_saml/licensing';
            drupal_goto($redirect);*/
            drupal_goto('admin/config/people/miniorange_saml_idp/licensing');
        } else {
            drupal_set_message(t('Error creating customer'), 'error');
        }
    } else {
        drupal_set_message(t('Error validating OTP'), 'error');
    }
}

/**
 * Resend OTP.
 */
function miniorange_saml_idp_resend_otp(&$form, $form_state) {

    variable_del('miniorange_saml_idp_tx_id');
    $username = variable_get('miniorange_saml_idp_customer_admin_email', NULL);
    $phone = variable_get('miniorange_saml_idp_customer_admin_phone', NULL);
    $customer_config = new MiniorangeSAMLIdpCustomer($username, $phone, NULL, NULL);
    $send_otp_response = json_decode($customer_config->sendOtp());
    if ($send_otp_response->status == 'SUCCESS') {
        // Store txID.
        variable_set('miniorange_saml_idp_tx_id', $send_otp_response->txId);
        $current_status = 'MOIDP_VALIDATE_OTP';
        variable_set('miniorange_saml_idp_status', $current_status);
        drupal_set_message(t('Verify email address by entering the passcode sent to @username', array('@username' => $username)));
    } else {
        $current_status = 'MOIDP_VALIDATE_OTP';
        variable_set('miniorange_saml_idp_status', $current_status);
        drupal_set_message(t('There was an error in sending OTP to your email. Please try again.'));
    }
}

/**
 * Handle submit for customer setup.
 */
function miniorange_saml_idp_customer_setup_submit(&$form, $form_state) {
    $username = $form['miniorange_saml_idp_customer_setup_username']['#value'];
    $phone = $form['miniorange_saml_idp_customer_setup_phone']['#value'];
    $password = $form['miniorange_saml_idp_customer_setup_password']['#value']['pass1'];
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
    $customer_config = new MiniorangeSAMLIdpCustomer($username, $phone, $password, NULL);
    $check_customer_response = json_decode($customer_config->checkCustomer());

    if ($check_customer_response->status == 'CURL_ERROR') {
        drupal_set_message(t('cURL is not enabled. Please enable cURL'), 'error');
    } else if ($check_customer_response->status == 'CUSTOMER_NOT_FOUND') {
        // Create customer.
        // Store email and phone.
        variable_set('miniorange_saml_idp_customer_admin_email', $username);
        variable_set('miniorange_saml_idp_customer_admin_phone', $phone);
        variable_set('miniorange_saml_idp_customer_admin_password', $password);

        $send_otp_response = json_decode($customer_config->sendOtp());
        if ($send_otp_response->status == 'SUCCESS') {
            // Store txID.
            variable_set('miniorange_saml_idp_tx_id', $send_otp_response->txId);
            $current_status = 'MOIDP_VALIDATE_OTP';
            variable_set('miniorange_saml_idp_status', $current_status);
            drupal_set_message(t('Verify email address by entering the passcode sent to @username', array('@username' => $username)));
        }
    } else {
        // Customer exists. Retrieve keys.
        $customer_keys_response = json_decode($customer_config->getCustomerKeys());
        if (json_last_error() == JSON_ERROR_NONE) {
            variable_set('miniorange_saml_idp_customer_id', $customer_keys_response->id);
            variable_set('miniorange_saml_idp_customer_admin_token', $customer_keys_response->token);
            variable_set('miniorange_saml_idp_customer_admin_email', $username);
            variable_set('miniorange_saml_idp_customer_admin_phone', $phone);
            variable_set('miniorange_saml_idp_customer_api_key', $customer_keys_response->apiKey);
            $current_status = 'MOIDP_PLUGIN_CONFIGURATION';
            variable_set('miniorange_saml_idp_status', $current_status);
            user_save(user_load(1), array('data' => array('overlay' => 1)));
            drupal_set_message(t('Successfully retrieved your account, now you can upgrade to our premium version of the module.'));
            drupal_goto('admin/config/people/miniorange_saml_idp/licensing');
        } else {
            drupal_set_message(t('Invalid credentials'), 'error');
        }
    }
}

/**
 * Handle back button submit for customer setup.
 */
function miniorange_saml_idp_back(&$form, $form_state) {
    $current_status = 'MOIDP_CUSTOMER_SETUP';
    variable_set('miniorange_saml_idp_status', $current_status);
    variable_del('miniorange_saml_idp_customer_admin_email');
    variable_del('miniorange_saml_idp_customer_admin_phone');
    variable_del('miniorange_saml_idp_tx_id');
    drupal_set_message(t('Register/Login with your miniOrange Account'));
}

/**
 * Send support query.
 */
function send_support_query(&$form, $form_state)
{
    $email = $form['miniorange_saml_email_address_support']['#value'];
    $phone = $form['miniorange_saml_phone_number_support']['#value'];
    $query = $form['miniorange_saml_support_query_support']['#value'];
    Utilities::send_query($email, $phone, $query);
}