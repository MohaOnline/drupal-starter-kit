<?php
/**
 * @package    miniOrange
 * @subpackage Plugins
 * @license    GNU/GPLv3
 * @copyright  Copyright 2015 miniOrange. All Rights Reserved.
 *
 *
 * This file is part of miniOrange Joomla SAML IDP plugin.
 *
 * miniOrange Joomla SAML IDP plugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * miniOrange Joomla IDP plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with miniOrange SAML plugin.  If not, see <http://www.gnu.org/licenses/>.
 */


class Utilities {

    public static function faq(&$form, &$form_state){

        $form['miniorange_faq'] = array(
            '#markup' => '<div ><b></b>
                          <a style="margin-left: 25%" class="mo_saml_btn mo_saml_btn-primary-faq mo_saml_btn-large mo_faq_button_left" href="https://faq.miniorange.com/kb/drupal/saml-drupal/" target="_blank">FAQs</a>
                          <b></b><a class="mo_saml_btn mo_saml_btn-primary-faq mo_saml_btn-large mo_faq_button_right"  href="https://forum.miniorange.com/" target="_blank">Ask questions on forum</a></div>',
        );
    }

    public static function spConfigGuide(&$form, &$form_state,$ad_or_guide){

        if($ad_or_guide=='GUIDE') {
          $form['miniorange_idp_guide_link1'] = array(
            '#markup' => '<div class="mo_saml_table_layout mo_saml_container_2">
                        <div style="font-size: 15px; text-align: justify">To see detailed documentation of how to configure
                        Drupal SAML IdP with any Service Provider</div></br>',
          );

          $form['miniorange_saml_guide_table_list'] = array(
            '#markup' => '<div class="table-responsive mo_guide_text-center" style="font-family: sans-serif;font-size: 15px;">
                <table class="mo_guide_table mo_guide_table-striped mo_guide_table-bordered" style="border: 1px solid #ddd;max-width: 100%;border-collapse: collapse;">
                    <thead>
                        <tr><th class="mo_guide_text-center" style="font-weight:bold;">Service Providers</th><th class="mo_guide_text-center" style="font-weight:bold;">Links</th></tr>
                    </thead>
                    <tbody style="color:gray;">
                        <tr><td>Tableau</td><td><strong><a href="https://plugins.miniorange.com/configure-tableau-as-sp-in-drupal-7-idp" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>Zendesk	</td><td><strong><a href="https://plugins.miniorange.com/zendesk-sso-single-sign-on-for-drupal-7-idp" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>Workplace by Facebook</td><td><strong><a href="https://plugins.miniorange.com/guide-drupal-idp-workplace-sp" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>Canvas LMS</td><td><strong><a href="https://plugins.miniorange.com/guide-to-configure-canvas-lms-as-sp-and-drupal-as-idp" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>Owncloud</td><td><strong><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-owncloud-sp-and-drupal-as-idp" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>Inkling</td><td><strong><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-inkling-sso-as-sp-for-drupal-7-idp" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>AppStream2</td><td><strong><a href="https://plugins.miniorange.com/guide-to-setup-drupal-as-idp-and-aws-appstream2-as-sp" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>For any other SP</td><td><strong><a href="https://www.miniorange.com/contact" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                    </tbody>

                </table></div>',
          );

          self::faq($form, $form_state);
          $form['miniorange_end_of_guide0'] = array(
            '#markup' => '</div>',
          );
        }
        else{
          if(rand(1,2)==1){
            self::advertiseNetworkSecurity($form, $form_state);
          }
          else{
            self::advertise2FA($form, $form_state);
          }


          $form['miniorange_end_of_guide'] = array(
            '#markup' => '</div>',
          );
        }

    }

    public static function advertise2FA(&$form,&$form_state){
        global $base_url;


        $form['miniorange_idp_guide_link2'] = array(
            '#markup' => '<div class="mo_saml_table_layout mo_saml_container_2">
                        ',
        );

        $form['miniorangerr_otp_email_address'] = array(
            '#markup' => '<div><h3 class="mo_otp_h_3"  >Checkout our Drupal <br>Two-Factor Authentication(2FA) module<br></h3></div>
                        <div class="mo_otp_adv_tfa"><img src="'.$base_url . '/' . drupal_get_path("module", "miniorange_saml_idp") . '/includes/images/miniorange_i.png" alt="miniOrange icon" height="80px" width="80px" class="mo_otp_img_adv"><h3 class="mo_otp_txt_h3">Two-Factor Authentication (2FA)</h3></div>',

        );
        $form['minioranqege_otp_phone_number'] = array(
            '#markup' => '<div class="mo_otp_paragraph"><p>Two Factor Authentication (TFA) for your Drupal site is highly secure and easy to setup. Adds a second layer of security to your Drupal accounts. It protects your site from hacks and unauthorized login attempts.</p></div>',
        );

        $form['miniorange_otp_2fa_button'] = array(
            '#markup' => '<div style="align:center;margin-left:15px;"> <a href="https://www.drupal.org/project/miniorange_2fa" class="mo_otp_btn1 mo_saml_btn mo_saml_btn-success" target="_blank" id="tfa_btn_download">Download Module</a>
      <a href="https://plugins.miniorange.com/drupal-two-factor-authentication-2fa" class="mo_otp_btn2 mo_saml_btn mo_saml_btn-primary" target="_blank" id="tfa_btn_know">Know More</a><br><br></div></div>'

        );
    }
	public static function AddrfdButton(&$form, &$form_state)
    {
        $form['markup_idp_attr_header_top_support_btn'] = array(
            '#markup' => '<div id="mosaml-feedback-form" class="mo_saml_table_layout_support_btn">',
        );

        $form['miniorange_saml_idp_support_side_button'] = array(
            '#type' => 'button',
            '#value' => t('Request for Demo'),
            '#attributes' => array('style' => 'font-size: 15px;cursor: pointer;width: 170px;height: 35px;
                background: rgba(43, 141, 65, 0.93);color: #ffffff;border-radius: 3px;transform: rotate(90deg);text-shadow: none;
                position: relative;margin-left: -102px;top: 115px;'),
        );

        $form['markup_idp_attr_header_top_support'] = array(
            '#markup' => '<div id="Support_Section" class="mo_saml_table_layout_support_1">',
        );


        $form['markup_2'] = array(
            '#markup' => '<b>Want to test  the Premium module before purchasing?</b> <br>Just send us a request, We will setup a demo site for you on our cloud and provide you with the administrator credentials.
                So that you can test all the premium features as per your requirement.
        <br>',
        );

        $form['customer_email'] = array(
            '#type' => 'textfield',
			'#default_value'=>variable_get('miniorange_saml_idp_customer_admin_email',''),
            '#attributes' => array('style' => 'width:100%','placeholder' => 'Enter your Email'),
        );

        $form['description_doubt'] = array(
            '#type' => 'textarea',
            '#clos' => '10',
            '#rows' => '5',
            '#attributes' => array('style' => 'width:100%','placeholder' => 'Write your query here'),
        );
        $form['markup_div'] = array(
            '#markup' => '<div>'
        );

        $form['miniorange_oauth_support_submit_click'] = array(
            '#type' => 'submit',
            '#value' => t('Submit Query'),
            '#submit' => array('send_rfd_query'),
            '#limit_validation_errors' => array(),
            '#attributes' => array('style' => 'background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;display:block;float:left'),
        );

        $form['markup_div_end'] = array(
            '#markup' => '</div>'
        );

        $form['miniorange_oauth_support_note'] = array(
            '#markup' => '<br><br><br><div>If you want custom features in the module, just drop an email to <a href="mailto:drupalsupport@xecurify.com">drupalsupport@xecurify.com</a></div>'
        );

        $form['miniorange_oauth_support_div_cust'] = array(
            '#markup' => '</div></div><div hidden id="mosaml-feedback-overlay">'
        );
    }
	public static function send_demo_query($email, $query, $description)
    {
        if(empty($email)||empty($description)){
            if(empty($email)) {
                drupal_set_message(t('The <b>Email Address</b> field is required.'), 'error');
            }
            if(empty($description)) {
                drupal_set_message(t('The <b>Description</b> field is required.'), 'error');
            }
            return;
        }
        if (!valid_email_address($email)) {
            drupal_set_message(t('The email address <b><u>' . $email . '</u></b> is not valid.'), 'error');
            return;
        }

        $phone = variable_get('miniorange_saml_idp_customer_admin_phone');
        $support = new MiniOrangeSamlIdpSupport($email, $phone, $query,'demo');
        $support_response = $support->sendSupportQuery();
        if($support_response) {
            drupal_set_message(t('Your demo request has been sent successfully. We will get back to you soon.'));
        }else {
            drupal_set_message(t('Error sending support query. Please try again.'), 'error');
        }
    }

    public static function advertiseNetworkSecurity(&$form,&$form_state){
        global $base_url;
        $form['miniorange_idp_guide_link3'] = array(
            '#markup' => '<div class="mo_saml_table_layout mo_saml_container_2">
                        ',
        );
        $form['mo_idp_net_adv']=array(
            '#markup'=>'<form name="f1">
        <table id="idp_support" class="idp-table" style="border: none;">
        <h4 style="text-align: center;">Looking for a Drupal Web Security module?</h4>
            <tr>
                <th class="" style="border: none; padding-bottom: 4%; background-color: white; text-align: center;"><img
                            src="'.$base_url . '/' . drupal_get_path("module", "miniorange_saml_idp") . '/includes/images/security.jpg"
                            alt="miniOrange icon" height=150px width=44%>
		<br>
                        <img src="'.$base_url . '/' . drupal_get_path("module", "miniorange_saml_idp") . '/includes/images/miniorange_i.png"
                             alt="miniOrange icon" height=50px width=50px style="float: left; margin-left: 44px; margin-right: -76px;"><h3 style="margin-top: 16px;">&nbsp;&nbsp;&nbsp;Drupal Website Security</h3>
                </th>
            </tr>

            <tr style="border-right: hidden;">
                <td style="text-align: center">
                    Building a website is a time-consuming process that requires tremendous efforts. For smooth
                    functioning and protection from any sort of web attack appropriate security is essential and we
                    ensure to provide the best website security solutions available in the market.
                    We provide you enterprise-level security, protecting your Drupal site from hackers and malware.
                </td>
            </tr>
            <tr style="border-right: hidden;">
                <td style="padding-left: 15%"><br>
                    <a href="https://www.drupal.org/project/security_login_secure" target="_blank"
                       class="mo_saml_btn mo_saml_btn-primary" style="padding: 4px 10px;">Download Module</a>
                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a
                            href="https://plugins.miniorange.com/drupal-web-security-pro" class="mo_saml_btn mo_saml_btn-success"
                            style="padding: 4px 10px;" target="_blank">Know More</a>
                </td>
            </tr>
        </table>
    </form>'
        );
        return $form;
    }

    public static function getLicensingPageURL(){
        global $base_url;
        return $base_url . '/admin/config/people/miniorange_saml_idp/licensing';
    }

    public static function isModuleConfigured(){
        $SP_name   = variable_get('miniorange_saml_idp_sp_name', '');
        $entity_id = variable_get('miniorange_saml_idp_sp_entity_id', '');
        $acs_url   = variable_get('miniorange_saml_idp_acs_url', '');
        return ( empty( $SP_name ) || empty( $acs_url ) || empty( $entity_id ) ) ? TRUE : FALSE;
    }

    public static function AddSupportButton(&$form, &$form_state)
    {
        $form['markup_idp_attr_header_top_support_btn'] = array(
            '#markup' => '<div id="mosaml-feedback-form" class="mo_saml_table_layout_support_btn">',
        );

        $form['miniorange_saml_idp_support_side_button'] = array(
            '#type' => 'button',
            '#value' => t('Support'),
            '#attributes' => array('style' => 'font-size: 15px;cursor: pointer;text-align: center;width: 150px;height: 35px;
                background: rgba(43, 141, 65, 0.93);color: #ffffff;border-radius: 3px;transform: rotate(90deg);text-shadow: none;
                position: relative;margin-left: -92px;top: 107px;'),
        );

        $form['markup_idp_attr_header_top_support'] = array(
            '#markup' => '<div id="Support_Section" class="mo_saml_table_layout_support_1">',
        );


        $form['markup_support_1'] = array(
            '#markup' => '<h3><b>Feature Request/Contact Us:</b></h3><div>Need any help? We can help you with configuring your Service Provider. Just send us a query and we will get back to you soon.<br /></div><br>',
        );

        $form['miniorange_saml_email_address_support'] = array(
            '#type' => 'textfield',
            '#attributes' => array('style' => 'width:100%','placeholder' => 'Enter your Email'),
            '#default_value' => variable_get('miniorange_saml_idp_customer_admin_email', ''),
        );

        $form['miniorange_saml_phone_number_support'] = array(
            '#type' => 'textfield',
            '#attributes' => array('style' => 'width:100%','pattern' => '[\+][0-9]{1,4}\s?[0-9]{7,12}','placeholder' => 'Enter your Phone Number'),
            '#default_value' => variable_get('miniorange_saml_idp_customer_admin_phone', ''),
        );

        $form['miniorange_saml_support_query_support'] = array(
            '#type' => 'textarea',
            '#clos' => '10',
            '#rows' => '5',
            '#attributes' => array('style' => 'width:100%','placeholder' => 'Write your query here'),
        );

        $form['miniorange_saml_support_submit_click'] = array(
            '#type' => 'submit',
            '#value' => t('Submit Query'),
            '#submit' => array('Utilities::send_support_query'),
            '#attributes' => array('style' => 'background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;display:block;margin-left:auto;margin-right:auto;'),
        );

        $form['miniorange_saml_support_note'] = array(
            '#markup' => '<div><br/>If you want custom features in the module, just drop an email to <a href="mailto:drupalsupport@xecurify.com">drupalsupport@xecurify.com</a></div>'
        );

        $form['miniorange_saml_support_div_cust'] = array(
            '#markup' => '</div></div><div hidden id="mosaml-feedback-overlay"></div>'
        );
    }

    /*
     * Get Support form data
     */
    public static function send_support_query(&$form, $form_state) {
        $email = trim($form['miniorange_saml_email_address_support']['#value']);
        $phone = trim($form['miniorange_saml_phone_number_support']['#value']);
        $query = trim($form['miniorange_saml_support_query_support']['#value']);
        Utilities::send_query( $email, $phone, $query );
    }
    /*
     * Send Support query
     */
    public static function send_query( $email, $phone, $query ) {
        if( empty( $email ) || empty( $query ) ){
            drupal_set_message(t('The <b>Email Address</b> and <b>Query</b> fields are mandatory.'), 'error');
            return;
        }
        if ( !valid_email_address( $email ) ) {
            drupal_set_message(t('The email address <b><u>' . $email . '</u></b> is not valid.'), 'error');
            return;
        }
        $support = new MiniOrangeSamlIdpSupport($email, $phone, $query);
        $support_response = $support->sendSupportQuery();
        if($support_response) {
            drupal_set_message(t('Your support query has been sent successfully. We will get back to you soon.'));
        }else {
            drupal_set_message(t('Error sending support query. Please try again.'), 'error');
        }
    }

    public static function isCustomerRegistered() {
        if (variable_get('miniorange_saml_idp_customer_admin_email', NULL) == NULL||
            variable_get('miniorange_saml_idp_customer_id', NULL) == NULL ||
            variable_get('miniorange_saml_idp_customer_admin_token', NULL) == NULL ||
            variable_get('miniorange_saml_idp_customer_api_key', NULL) == NULL)
        {
            return TRUE;
        }else {
            return FALSE;
        }
    }

    public static function customer_setup_submit($username, $phone, $password, $called_from_popup=false, $payment_plan=NULL){
        $customer_config = new MiniorangeSAMLIdpCustomer($username, $phone, $password, NULL);
        $check_customer_response = json_decode($customer_config->checkCustomer());

        if ($check_customer_response->status == 'CUSTOMER_NOT_FOUND') {
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
                if ($called_from_popup == true) {
                    miniorange_otp(false,false,false);
                }else{
                    drupal_set_message(t('Verify email address by entering the passcode sent to @username', array('@username' => $username)));
                }
            }else{
                if ($called_from_popup == true) {
                    register_data(true);
                }else{
                    drupal_set_message(t('An error has been occured. Please try after some time.'),'error');
                }
            }
        }
        elseif ($check_customer_response->status == 'CURL_ERROR') {

            if ($called_from_popup == true) {
                register_data(true);
            }else{
                drupal_set_message(t('cURL is not enabled. Please enable cURL'), 'error');
            }
        }
        else {
            // Customer exists. Retrieve keys.
            $customer_keys_response = json_decode($customer_config->getCustomerKeys());
            if (json_last_error() == JSON_ERROR_NONE) {
                variable_set('miniorange_saml_idp_customer_id', $customer_keys_response->id);
                variable_set('miniorange_saml_idp_customer_admin_token', $customer_keys_response->token);
                variable_set('miniorange_saml_idp_customer_admin_email', $username);
                variable_set('miniorange_saml_idp_customer_admin_phone', $phone);
                variable_set('miniorange_saml_idp_customer_api_key', $customer_keys_response->apiKey);
                variable_set('miniorange_saml_idp_status', 'MOIDP_PLUGIN_CONFIGURATION');

                if ($called_from_popup == true) {
                    $payment_page_url = variable_get('redirect_plan_after_registration_' . $payment_plan,'');
                    $payment_page_url = str_replace('none', $username, $payment_page_url);
                    drupal_goto($payment_page_url);
                }else{
                    drupal_set_message(t('Successfully retrieved your account.'));
                    self::redirect_to_licensing();
                }

            }else if($check_customer_response->status=='TRANSACTION_LIMIT_EXCEEDED') {

                if ($called_from_popup == true) {
                    register_data(true);
                }else{
                    drupal_set_message(t('An error has been occured. Please try after some time.'), 'error');
                }
            }
            else {
                if ($called_from_popup == true) {
                    register_data(false, true);
                }else{
                    drupal_set_message(t('Invalid credentials'), 'error');
                }
            }
        }
    }


    public static function validate_otp_submit($otp_token, $called_from_popup=false, $payment_plan=NULL){
        $username = variable_get('miniorange_saml_idp_customer_admin_email', NULL);
        $phone = variable_get('miniorange_saml_idp_customer_admin_phone', NULL);
        $tx_id = variable_get('miniorange_saml_idp_tx_id', NULL);
        $customer_config = new MiniorangeSAMLIdpCustomer($username, $phone, NULL, $otp_token);
        // Validate OTP.
        $validate_otp_response = json_decode($customer_config->validateOtp($tx_id));
        if ($validate_otp_response->status == 'SUCCESS') {
            // OTP Validated. Show Configuration page.
            //variable_set('miniorange_saml_idp_status', 'MOIDP_PLUGIN_CONFIGURATION');
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
                drupal_set_message(t('Customer account created successfully. Now you can upgrade to the premium version of the module.'));

                if ($called_from_popup == true) {
                    $payment_page_url = variable_get('redirect_plan_after_registration_' . $payment_plan,'');
                    $payment_page_url = str_replace('none', $username, $payment_page_url);
                    miniorange_redirect_successfull($payment_page_url);
                    /*drupal_goto($payment_page_url);*/
                }else{
                    self::redirect_to_licensing();
                }
            }
            else if(trim($create_customer_response->message) == 'Email is not enterprise email.' || trim($create_customer_response->message)=='This is not a valid email. please enter a valid email.' || $create_customer_response->status=='INVALID_EMAIL_QUICK_EMAIL') {
              drupal_set_message(t('There was an error in creating an account for you.<br> You may have entered an invalid Email-Id
                            <strong>(We discourage the use of disposable emails) </strong>
                            <br>Please try again with a valid email.'), 'error');
              if ($called_from_popup == true) {
                    self::redirect_to_licensing();
                }
            }else {

              drupal_set_message(t('Error while creating a account for you. You may create a account from <a href="https://www.miniorange.com/businessfreetrial" target="_blank">here</a>'), 'error');
                if ($called_from_popup == true) {
                    self::redirect_to_licensing();
                }
            }
        } else {
            if ($called_from_popup == true) {
                miniorange_otp(true,false,false);
            }else{
                drupal_set_message(t('Error validating OTP'), 'error');
            }
        }
    }

    public static function saml_resend_otp($called_from_popup=false){
        variable_del('miniorange_saml_idp_tx_id');
        $username = variable_get('miniorange_saml_idp_customer_admin_email', NULL);
        $phone = variable_get('miniorange_saml_idp_customer_admin_phone', NULL);
        $customer_config = new MiniorangeSAMLIdpCustomer($username, $phone, NULL, NULL);
        $send_otp_response = json_decode($customer_config->sendOtp());
        if ($send_otp_response->status == 'SUCCESS') {
            // Store txID.
            variable_set('miniorange_saml_idp_tx_id', $send_otp_response->txId);
            variable_set('miniorange_saml_idp_status', 'MOIDP_VALIDATE_OTP');

            if ($called_from_popup == true) {
                miniorange_otp(false,true,false);
            }else{
                drupal_set_message(t('Verify email address by entering the passcode sent to @username', array('@username' => $username)));
            }
        }else{
            if ($called_from_popup == true) {
                miniorange_otp(false,false,true);
            }else{
                drupal_set_message(t('An error has been occured. Please try after some time'),'error');
            }
        }
    }

    public static function redirect_to_licensing(){
        $redirect = self::getLicensePageURL();
        drupal_goto($redirect);
    }

    public static function getLicensePageURL(){
        global $base_url;
        return $base_url.'/admin/config/people/miniorange_saml_idp/licensing';
    }

   public static function upload_metadata( $file ){

        if( empty( variable_get('miniorange_saml_idp_sp_name' ) ) ) {
            variable_set('miniorange_saml_idp_sp_name', 'Service Provider');
        }
        require_once drupal_get_path('module', 'miniorange_saml_idp') . DIRECTORY_SEPARATOR . 'includes' . DIRECTORY_SEPARATOR . 'MetadataReader.php';
        $document = new DOMDocument();
        $document->loadXML( $file );
        restore_error_handler();
        $first_child = $document->firstChild;

        if( !empty( $first_child ) ) {
            $metadata = new MetadataReader($document);
            $service_providers = $metadata->getServiceProviders();

            if( empty( $service_providers ) ) {
                drupal_set_message(t('<b>Please provide a valid metadata file.</b>'),'error');
                return;
            }
            foreach( $service_providers as $key => $sp ) {
                $entityID_issuer = $sp->getEntityID();
                $acs_url = $sp->getAcsURL();
                $is_assertion_signed = $sp->getAssertionsSigned() == 'true' ? TRUE : FALSE;

                variable_set('miniorange_saml_idp_sp_entity_id', $entityID_issuer);
                variable_set('miniorange_saml_idp_acs_url', $acs_url);
                variable_set('miniorange_saml_idp_assertion_signed', $is_assertion_signed);
            }
            drupal_set_message( t('Service Provider Configuration successfully saved.') );
            return;
        }
        else {
            drupal_set_message(t('<b>Please provide a valid metadata file.</b>'),'error');
            return;
        }
    }

    public static function getVariableNames($class_name) {
        if($class_name == "mo_options_enum_identity_provider") {
            $class_object = array (
                'IdP_Entity_ID' => 'miniorange_saml_issuer_id',
                'IdP_Login_URL' => 'miniorange_saml_login_url',
            );
        }
        else if($class_name == "mo_options_enum_service_provider") {
            $class_object = array(
                'Service_Provider_Name' => 'miniorange_saml_idp_sp_name',
                'ACS_URL'               => 'miniorange_saml_idp_acs_url',
                'Issuer'                => 'miniorange_saml_idp_sp_entity_id',
                'NameId_Format'         => 'miniorange_saml_idp_nameid_format',
                'Relay_State'           => 'miniorange_saml_idp_relay_state',
                'Assertion_Signed'      => 'miniorange_saml_idp_assertion_signed',
            );
        }
        return $class_object;
    }

    public static function miniorange_saml_is_sp_configured() {
        $saml_login_url  = variable_get( 'miniorange_saml_idp_acs_url' );
        $saml_idp_issuer = variable_get( 'miniorange_saml_idp_sp_entity_id' );
        if ( ! empty( $saml_login_url ) && ! empty( $saml_idp_issuer ) ) {
            return 1;
        } else {
            return 0;
        }
    }

    public static function isCurlInstalled() {
        if ( in_array( 'curl', get_loaded_extensions() ) ) {
            return 1;
        } else {
            return 0;
        }
    }

	public static function generateID() {
		return '_' . self::stringToHex(self::generateRandomBytes(21));
	}

	public static function stringToHex($bytes) {
		$ret = '';
		for($i = 0; $i < strlen($bytes); $i++) {
			$ret .= sprintf('%02x', ord($bytes[$i]));
		}
		return $ret;
	}

	public static function generateRandomBytes($length, $fallback = TRUE) {
        return openssl_random_pseudo_bytes($length);
	}

	public static function generateTimestamp($instant = NULL) {
		if($instant === NULL) {
			$instant = time();
		}
		return gmdate('Y-m-d\TH:i:s\Z', $instant);
	}

	public static function xpQuery(DOMNode $node, $query){
        static $xpCache = NULL;

        if ($node instanceof DOMDocument) {
            $doc = $node;
        } else {
            $doc = $node->ownerDocument;
        }

        if ($xpCache === NULL || !$xpCache->document->isSameNode($doc)) {
            $xpCache = new DOMXPath($doc);
            $xpCache->registerNamespace('soap-env', 'http://schemas.xmlsoap.org/soap/envelope/');
            $xpCache->registerNamespace('saml_protocol', 'urn:oasis:names:tc:SAML:2.0:protocol');
            $xpCache->registerNamespace('saml_assertion', 'urn:oasis:names:tc:SAML:2.0:assertion');
            $xpCache->registerNamespace('saml_metadata', 'urn:oasis:names:tc:SAML:2.0:metadata');
            $xpCache->registerNamespace('ds', 'http://www.w3.org/2000/09/xmldsig#');
            $xpCache->registerNamespace('xenc', 'http://www.w3.org/2001/04/xmlenc#');
        }

        $results = $xpCache->query($query, $node);
        $ret = array();
        for ($i = 0; $i < $results->length; $i++) {
            $ret[$i] = $results->item($i);
        }
		return $ret;
    }

	public static function parseNameId(DOMElement $xml)
    {
        $ret = array('Value' => trim($xml->textContent));

        foreach (array('NameQualifier', 'SPNameQualifier', 'Format') as $attr) {
            if ($xml->hasAttribute($attr)) {
                $ret[$attr] = $xml->getAttribute($attr);
            }
        }

        return $ret;
    }

	public static function xsDateTimeToTimestamp($time)
    {
        $matches = array();

        // We use a very strict regex to parse the timestamp.
        $regex = '/^(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d)T(\\d\\d):(\\d\\d):(\\d\\d)(?:\\.\\d+)?Z$/D';
        if (preg_match($regex, $time, $matches) == 0) {
            echo sprintf("nvalid SAML2 timestamp passed to xsDateTimeToTimestamp: ".$time);
            exit;
        }

        // Extract the different components of the time from the  matches in the regex.
        // intval will ignore leading zeroes in the string.
        $year   = intval($matches[1]);
        $month  = intval($matches[2]);
        $day    = intval($matches[3]);
        $hour   = intval($matches[4]);
        $minute = intval($matches[5]);
        $second = intval($matches[6]);

        // We use gmmktime because the timestamp will always be given
        //in UTC.
        $ts = gmmktime($hour, $minute, $second, $month, $day, $year);

        return $ts;
    }

	public static function extractStrings(DOMElement $parent, $namespaceURI, $localName)
    {
        $ret = array();
        for ($node = $parent->firstChild; $node !== NULL; $node = $node->nextSibling) {
            if ($node->namespaceURI !== $namespaceURI || $node->localName !== $localName) {
                continue;
            }
            $ret[] = trim($node->textContent);
        }

        return $ret;
    }

	public static function validateElement(DOMElement $root)
    {

        /* Create an XML security object. */
        $objXMLSecDSig = new XMLSecurityDSig();

        /* Both SAML messages and SAML assertions use the 'ID' attribute. */
        $objXMLSecDSig->idKeys[] = 'ID';


        /* Locate the XMLDSig Signature element to be used. */
        $signatureElement = self::xpQuery($root, './ds:Signature');

        if (count($signatureElement) === 0) {
            /* We don't have a signature element to validate. */
            return FALSE;
        } elseif (count($signatureElement) > 1) {
        	echo sprintf("XMLSec: more than one signature element in root.");
        	exit;
        }

        $signatureElement = $signatureElement[0];
        $objXMLSecDSig->sigNode = $signatureElement;

        /* Canonicalize the XMLDSig SignedInfo element in the message. */
        $objXMLSecDSig->canonicalizeSignedInfo();

       /* Validate referenced xml nodes. */
        if (!$objXMLSecDSig->validateReference()) {
        	echo sprintf("XMLsec: digest validation failed");
        	exit;
        }

		/* Check that $root is one of the signed nodes. */
        $rootSigned = FALSE;
        /** @var DOMNode $signedNode */
        foreach ($objXMLSecDSig->getValidatedNodes() as $signedNode) {
            if ($signedNode->isSameNode($root)) {
                $rootSigned = TRUE;
                break;
            } elseif ($root->parentNode instanceof DOMDocument && $signedNode->isSameNode($root->ownerDocument)) {
                /* $root is the root element of a signed document. */
                $rootSigned = TRUE;
                break;
            }
        }

		if (!$rootSigned) {
			echo sprintf("XMLSec: The root element is not signed.");
			exit;
        }

        /* Now we extract all available X509 certificates in the signature element. */
        $certificates = array();
        foreach (self::xpQuery($signatureElement, './ds:KeyInfo/ds:X509Data/ds:X509Certificate') as $certNode) {
            $certData = trim($certNode->textContent);
            $certData = str_replace(array("\r", "\n", "\t", ' '), '', $certData);
            $certificates[] = $certData;
        }

        $ret = array(
            'Signature' => $objXMLSecDSig,
            'Certificates' => $certificates,
            );


        return $ret;
    }

	public static function validateSignature(array $info, XMLSecurityKey $key)
    {
        /** @var XMLSecurityDSig $objXMLSecDSig */
        $objXMLSecDSig = $info['Signature'];

        $sigMethod = self::xpQuery($objXMLSecDSig->sigNode, './ds:SignedInfo/ds:SignatureMethod');
        if (empty($sigMethod)) {
            echo sprintf('Missing SignatureMethod element');
            exit();
        }
        $sigMethod = $sigMethod[0];
        if (!$sigMethod->hasAttribute('Algorithm')) {
            echo sprintf('Missing Algorithm-attribute on SignatureMethod element.');
            exit;
        }
        $algo = $sigMethod->getAttribute('Algorithm');

        if ($key->type === XMLSecurityKey::RSA_SHA1 && $algo !== $key->type) {
            $key = self::castKey($key, $algo);
        }

        /* Check the signature. */
        if (! $objXMLSecDSig->verify($key)) {
        	echo sprintf('Unable to validate Sgnature');
        	exit;
        }
    }

    public static function castKey(XMLSecurityKey $key, $algorithm, $type = 'public')
    {
    	// do nothing if algorithm is already the type of the key
    	if ($key->type === $algorithm) {
    		return $key;
    	}

    	$keyInfo = openssl_pkey_get_details($key->key);
    	if ($keyInfo === FALSE) {
    		echo sprintf('Unable to get key details from XMLSecurityKey.');
    		exit;
    	}
    	if (!isset($keyInfo['key'])) {
    		echo sprintf('Missing key in public key details.');
    		exit;
    	}

    	$newKey = new XMLSecurityKey($algorithm, array('type'=>$type));
    	$newKey->loadKey($keyInfo['key']);

    	return $newKey;
    }

	    /**
     * Decrypt an encrypted element.
     *
     * This is an internal helper function.
     *
     * @param  DOMElement     $encryptedData The encrypted data.
     * @param  XMLSecurityKey $inputKey      The decryption key.
     * @param  array          &$blacklist    Blacklisted decryption algorithms.
     * @return DOMElement     The decrypted element.
     * @throws Exception
     */
    private static function doDecryptElement(DOMElement $encryptedData, XMLSecurityKey $inputKey, array &$blacklist)
    {
        $enc = new XMLSecEnc();
        $enc->setNode($encryptedData);

        $enc->type = $encryptedData->getAttribute("Type");
        $symmetricKey = $enc->locateKey($encryptedData);
        if (!$symmetricKey) {
        	echo sprintf('Could not locate key algorithm in encrypted data.');
        	exit;
        }

        $symmetricKeyInfo = $enc->locateKeyInfo($symmetricKey);
        if (!$symmetricKeyInfo) {
			echo sprintf('Could not locate <dsig:KeyInfo> for the encrypted key.');
			exit;
        }
        $inputKeyAlgo = $inputKey->getAlgorith();
        if ($symmetricKeyInfo->isEncrypted) {
            $symKeyInfoAlgo = $symmetricKeyInfo->getAlgorith();
            if (in_array($symKeyInfoAlgo, $blacklist, TRUE)) {
                echo sprintf('Algorithm disabled: ' . var_export($symKeyInfoAlgo, TRUE));
                exit;
            }
            if ($symKeyInfoAlgo === XMLSecurityKey::RSA_OAEP_MGF1P && $inputKeyAlgo === XMLSecurityKey::RSA_1_5) {
                /*
                 * The RSA key formats are equal, so loading an RSA_1_5 key
                 * into an RSA_OAEP_MGF1P key can be done without problems.
                 * We therefore pretend that the input key is an
                 * RSA_OAEP_MGF1P key.
                 */
                $inputKeyAlgo = XMLSecurityKey::RSA_OAEP_MGF1P;
            }
            /* Make sure that the input key format is the same as the one used to encrypt the key. */
            if ($inputKeyAlgo !== $symKeyInfoAlgo) {
                echo sprintf( 'Algorithm mismatch between input key and key used to encrypt ' .
                    ' the symmetric key for the message. Key was: ' .
                    var_export($inputKeyAlgo, TRUE) . '; message was: ' .
                    var_export($symKeyInfoAlgo, TRUE));
                exit;
            }
            /** @var XMLSecEnc $encKey */
            $encKey = $symmetricKeyInfo->encryptedCtx;
            $symmetricKeyInfo->key = $inputKey->key;
            $keySize = $symmetricKey->getSymmetricKeySize();
            if ($keySize === NULL) {
                /* To protect against "key oracle" attacks, we need to be able to create a
                 * symmetric key, and for that we need to know the key size.
                 */
				echo sprintf('Unknown key size for encryption algorithm: ' . var_export($symmetricKey->type, TRUE));
				exit;
            }
            try {
                $key = $encKey->decryptKey($symmetricKeyInfo);
                if (strlen($key) != $keySize) {
                	echo sprintf('Unexpected key size (' . strlen($key) * 8 . 'bits) for encryption algorithm: ' .
                        var_export($symmetricKey->type, TRUE));
                	exit;
                }
            } catch (Exception $e) {
                /* We failed to decrypt this key. Log it, and substitute a "random" key. */

                /* Create a replacement key, so that it looks like we fail in the same way as if the key was correctly padded. */
                /* We base the symmetric key on the encrypted key and private key, so that we always behave the
                 * same way for a given input key.
                 */
                $encryptedKey = $encKey->getCipherValue();
                $pkey = openssl_pkey_get_details($symmetricKeyInfo->key);
                $pkey = sha1(serialize($pkey), TRUE);
                $key = sha1($encryptedKey . $pkey, TRUE);
                /* Make sure that the key has the correct length. */
                if (strlen($key) > $keySize) {
                    $key = substr($key, 0, $keySize);
                } elseif (strlen($key) < $keySize) {
                    $key = str_pad($key, $keySize);
                }
            }
            $symmetricKey->loadkey($key);
        } else {
            $symKeyAlgo = $symmetricKey->getAlgorith();
            /* Make sure that the input key has the correct format. */
            if ($inputKeyAlgo !== $symKeyAlgo) {
            	echo sprintf( 'Algorithm mismatch between input key and key in message. ' .
                    'Key was: ' . var_export($inputKeyAlgo, TRUE) . '; message was: ' .
                    var_export($symKeyAlgo, TRUE));
            	exit;
            }
            $symmetricKey = $inputKey;
        }
        $algorithm = $symmetricKey->getAlgorith();
        if (in_array($algorithm, $blacklist, TRUE)) {
            echo sprintf('Algorithm disabled: ' . var_export($algorithm, TRUE));
            exit;
        }
        /** @var string $decrypted */
        $decrypted = $enc->decryptNode($symmetricKey, FALSE);
        /*
         * This is a workaround for the case where only a subset of the XML
         * tree was serialized for encryption. In that case, we may miss the
         * namespaces needed to parse the XML.
         */
        $xml = '<root xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion" '.
                     'xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">' .
            $decrypted .
            '</root>';
        $newDoc = new DOMDocument();
        if (!@$newDoc->loadXML($xml)) {
        	echo sprintf('Failed to parse decrypted XML. Maybe the wrong sharedkey was used?');
        	throw new Exception('Failed to parse decrypted XML. Maybe the wrong sharedkey was used?');
        }
        $decryptedElement = $newDoc->firstChild->firstChild;
        if ($decryptedElement === NULL) {
        	echo sprintf('Missing encrypted element.');
        	throw new Exception('Missing encrypted element.');
        }

        if (!($decryptedElement instanceof DOMElement)) {
        	echo sprintf('Decrypted element was not actually a DOMElement.');
        }

        return $decryptedElement;
    }
    /**
     * Decrypt an encrypted element.
     *
     * @param  DOMElement     $encryptedData The encrypted data.
     * @param  XMLSecurityKey $inputKey      The decryption key.
     * @param  array          $blacklist     Blacklisted decryption algorithms.
     * @return DOMElement     The decrypted element.
     * @throws Exception
     */
    public static function decryptElement(DOMElement $encryptedData, XMLSecurityKey $inputKey, array $blacklist = array(), XMLSecurityKey $alternateKey = NULL)
    {
        try {
        	echo "trying primary";
            return self::doDecryptElement($encryptedData, $inputKey, $blacklist);
        } catch (Exception $e) {
        	//Try with alternate key
        	try {
        		echo "trying secondary";
        		return self::doDecryptElement($encryptedData, $alternateKey, $blacklist);
        	} catch(Exception $t) {

        	}
        	/*
        	 * Something went wrong during decryption, but for security
        	 * reasons we cannot tell the user what failed.
        	 */
        	echo sprintf('Failed to decrypt XML element.');
        	exit;
        }
    }

    /**
     * Parse a boolean attribute.
     *
     * @param  \DOMElement $node          The element we should fetch the attribute from.
     * @param  string     $attributeName The name of the attribute.
     * @param  mixed      $default       The value that should be returned if the attribute doesn't exist.
     * @return bool|mixed The value of the attribute, or $default if the attribute doesn't exist.
     * @throws \Exception
     */
    public static function parseBoolean(DOMElement $node, $attributeName, $default = null)
    {
        if (!$node->hasAttribute($attributeName)) {
            return $default;
        }
        $value = $node->getAttribute($attributeName);
        switch (strtolower($value)) {
            case '0':
            case 'false':
                return false;
            case '1':
            case 'true':
                return true;
            default:
                throw new Exception('Invalid value of boolean attribute ' . var_export($attributeName, true) . ': ' . var_export($value, true));
        }
    }

	public static function getEncryptionAlgorithm($method){
		switch($method){
			case 'http://www.w3.org/2001/04/xmlenc#tripledes-cbc':
				return XMLSecurityKey::TRIPLEDES_CBC;
				break;

			case 'http://www.w3.org/2001/04/xmlenc#aes128-cbc':
				return XMLSecurityKey::AES128_CBC;

			case 'http://www.w3.org/2001/04/xmlenc#aes192-cbc':
				return XMLSecurityKey::AES192_CBC;
				break;

			case 'http://www.w3.org/2001/04/xmlenc#aes256-cbc':
				return XMLSecurityKey::AES256_CBC;
				break;

			case 'http://www.w3.org/2001/04/xmlenc#rsa-1_5':
				return XMLSecurityKey::RSA_1_5;
				break;

			case 'http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p':
				return XMLSecurityKey::RSA_OAEP_MGF1P;
				break;

			case 'http://www.w3.org/2000/09/xmldsig#dsa-sha1':
				return XMLSecurityKey::DSA_SHA1;
				break;

			case 'http://www.w3.org/2000/09/xmldsig#rsa-sha1':
				return XMLSecurityKey::RSA_SHA1;
				break;

			case 'http://www.w3.org/2001/04/xmldsig-more#rsa-sha256':
				return XMLSecurityKey::RSA_SHA256;
				break;

			case 'http://www.w3.org/2001/04/xmldsig-more#rsa-sha384':
				return XMLSecurityKey::RSA_SHA384;
				break;

			case 'http://www.w3.org/2001/04/xmldsig-more#rsa-sha512':
				return XMLSecurityKey::RSA_SHA512;
				break;

			default:
				echo sprintf('Invalid Encryption Method: '.$method);
				exit;
				break;
		}
	}

	public static function sanitize_certificate( $certificate ) {
		$certificate = preg_replace("/[\r\n]+/", "", $certificate);
		$certificate = str_replace( "-", "", $certificate );
		$certificate = str_replace( "BEGIN CERTIFICATE", "", $certificate );
		$certificate = str_replace( "END CERTIFICATE", "", $certificate );
		$certificate = str_replace( " ", "", $certificate );
		$certificate = chunk_split($certificate, 64, "\r\n");
		$certificate = "-----BEGIN CERTIFICATE-----\r\n" . $certificate . "-----END CERTIFICATE-----";
		return $certificate;
	}

	public static function desanitize_certificate( $certificate ) {
		$certificate = preg_replace("/[\r\n]+/", "", $certificate);
		$certificate = str_replace( "-----BEGIN CERTIFICATE-----", "", $certificate );
		$certificate = str_replace( "-----END CERTIFICATE-----", "", $certificate );
		$certificate = str_replace( " ", "", $certificate );
		return $certificate;
	}
}
