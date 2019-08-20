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

    $current_status = variable_get('miniorange_saml_idp_status', '');
    if ($current_status == 'MOIDP_VALIDATE_OTP') {

        $form['markup_14'] = array(
            '#markup' => '<div class="mo_saml_table_layout_1"><div class="mo_saml_table_layout mo_saml_container">',
        );

        $form['miniorange_saml_idp_customer_otp_token'] = array(
            '#type' => 'textfield',
            '#title' => t('OTP<span class="miniorange_class_color_red">*</span>'),
            '#description' => 'Please enter the OTP received on your registered email.',
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
            '#markup' => '<br><br><br><br><br><br><br><br><br><br><br><br><br></div>'
        );

        Utilities::spConfigGuide($form, $form_state);
        Utilities::AddSupportButton($form, $form_state);

        return $form;
    }
    elseif ($current_status == 'MOIDP_PLUGIN_CONFIGURATION') {

        $form['markup_saml_idp_regsitration_message'] = array(
            '#markup' => '<div class="mo_saml_table_layout_1">
                        <div class="mo_saml_table_layout mo_saml_container">
                        <div class="mo_saml_welcome_message">Thank you for registering with miniOrange.</div>'
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

        $form['miniorange_saml_customer_Remove_Account_info'] = array(
            '#markup' => '<br><br><br><br/><br><h4>Remove Account: <a href="' . Utilities::getLicensingPageURL() . '"> [Premium]</a></br></h4>
                    <p>This section will help you to remove your current logged in account without losing your current configurations.</p>'
        );

        $form['miniorange_saml_remove_account_button'] = array(
            '#type' => 'submit',
            '#value' => t('Remove Account '),
            '#disabled' => TRUE,
            '#suffix' => '</div>'
        );

        Utilities::spConfigGuide($form, $form_state);
        Utilities::AddSupportButton($form, $form_state);

        return $form;
    }

    $form['markup_reg_msg'] = array(
        '#markup' => '<div class="mo_saml_table_layout_1"><div class="mo_saml_table_layout mo_saml_container"><b>
            <span style="font-size: 19px;">Register/Login with miniOrange (Optional)</span></b><br><br><hr>
            <div class="mo_saml_register_message">You need to <a>Register/login</a> with miniOrange before upgrade to our paid version of the module.</div>',
    );

    $form['markup_15'] = array(
        '#markup' => '<br><div class="mo_saml_highlight_background_note" style="width: auto"><h3>Why Should I register?</h3>
                        You should register so that in case you need help, we can help you with step by step instructions. 
                    <b>You will also need a miniOrange account to upgrade to the premium version of the modules.</b> 
                    We do not store any information except the email that you will use to register with us.</div><br>',
    );


    $form['miniorange_saml_idp_customer_setup_username'] = array(
        '#type' => 'textfield',
        '#title' => t('Email<span class="miniorange_class_color_red">*</span>'),
        '#attributes' => array('style' => 'width:60%'),
        '#description' => t('<b>Note:</b> Use valid EmailId. (We discourage the use of disposable emails)'),
    );

    $form['miniorange_saml_idp_customer_setup_phone'] = array(
        '#type' => 'textfield',
        '#title' => t('Phone'),
        '#attributes' => array('style' => 'width:60%'),
        '#description' => t('<b>Note:</b> We will only call if you need support.'),
    );

    $form['miniorange_saml_idp_customer_setup_password'] = array(
        '#type' => 'password_confirm',
    );

    $form['miniorange_saml_idp_customer_setup_button'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#submit' => array('miniorange_saml_idp_customer_setup_submit'),
        //'#attributes' => array('style' => 'width:100px;background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;'),
        '#suffix' => '</div>'
    );

    Utilities::spConfigGuide($form, $form_state);
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

            drupal_set_message(t('Thank you for registering with miniOrange. You can use this account to upgrade to the premium version of the plugin. '));
            drupal_goto('admin/config/people/miniorange_saml_idp/licensing');
        }
        else if(trim($create_customer_response->message) == 'Email is not enterprise email.')
        {
            drupal_set_message(t('There was an error creating an account for you.<br> You may have entered an invalid Email-Id
            <strong>(We discourage the use of disposable emails) </strong>
            <br>Please try again with a valid email.'), 'error');
        }
        else {
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
    if ( $send_otp_response->status == 'SUCCESS' ) {
        // Store txID.
        variable_set('miniorange_saml_idp_tx_id', $send_otp_response->txId);
        variable_set('miniorange_saml_idp_status', 'MOIDP_VALIDATE_OTP');
        drupal_set_message(t('Verify email address by entering the passcode sent to @username', array('@username' => $username)));
    } else {
        variable_set('miniorange_saml_idp_status', 'MOIDP_VALIDATE_OTP');
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
    if( empty( $username )|| empty( $password ) ) {
        drupal_set_message(t('The <b>Email Address</b> and <b>Password</b> fields are mandatory.'), 'error');
        return;
    }
    if ( !valid_email_address( $username ) ) {
        drupal_set_message(t('The email address <b><u>' . $username . '</u></b> is not valid.'), 'error');
        return;
    }
    $customer_config = new MiniorangeSAMLIdpCustomer($username, $phone, $password, NULL);
    $check_customer_response = json_decode($customer_config->checkCustomer());

    if (isset($check_customer_response->status) ? $check_customer_response->status == 'CURL_ERROR' : '') 
    {
        drupal_set_message(t('cURL is not enabled. Please enable cURL'), 'error');
    } 
    else if ($check_customer_response->status == 'CUSTOMER_NOT_FOUND') {
        // Create customer.
        // Store email and phone.
        variable_set('miniorange_saml_idp_customer_admin_email', $username);
        variable_set('miniorange_saml_idp_customer_admin_phone', $phone);
        variable_set('miniorange_saml_idp_customer_admin_password', $password);

        $send_otp_response = json_decode($customer_config->sendOtp());
        if ($send_otp_response->status == 'SUCCESS') {
            // Store txID.
            variable_set('miniorange_saml_idp_tx_id', $send_otp_response->txId);
            variable_set('miniorange_saml_idp_status', 'MOIDP_VALIDATE_OTP');
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
            user_save( user_load(1 ), array( 'data' => array( 'overlay' => 1 ) ) );
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
    variable_set('miniorange_saml_idp_status', 'MOIDP_CUSTOMER_SETUP');
    variable_del('miniorange_saml_idp_customer_admin_email');
    variable_del('miniorange_saml_idp_customer_admin_phone');
    variable_del('miniorange_saml_idp_tx_id');
    drupal_set_message(t('Register/Login with your miniOrange Account'));
}
