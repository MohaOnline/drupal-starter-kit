<?php

class Utilities {
    /**
     * This function adds the support block
     */
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
            '#markup' => '<h3><b>Feature Request/Contact Us:</b></h3><div>Need any help? We can help you with configuring your OAuth Provider. Just send us a query and we will get back to you soon.</div>',
        );

        $form['miniorange_oauth_email_address_support'] = array(
            '#type' => 'textfield',
            '#attributes' => array('style' => 'width:100%','placeholder' => 'Enter your Email'),
        );

        $form['miniorange_oauth_phone_number_support'] = array(
            '#type' => 'textfield',
            '#attributes' => array('style' => 'width:100%','placeholder' => 'Enter your Phone Number'),
        );

        $form['miniorange_oauth_support_query_support'] = array(
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
            '#submit' => array('send_support_query'),
            '#attributes' => array('style' => 'background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;display:block;float:left'),
        );

        $form['miniorange_oauth_client_redirect_demo'] = array(
            '#type' => 'submit',
            '#value' => t('Request for Demo'),
            '#submit' => array('rfd'),
            '#limit_validation_errors' => array(),
            '#attributes' => array('style' => 'background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;display:block;float:right;'),
        );

        $form['markup_div_end'] = array(
            '#markup' => '</div>'
        );

        $form['miniorange_oauth_support_note'] = array(
            '#markup' => '<br><br><br><div>If you want custom features in the module, just drop an email to <a href="mailto:drupalsupport@xecurify.com">drupalsupport@xecurify.com</a></div>'
        );

        $form['miniorange_oauth_support_div_cust'] = array(
            '#markup' => '</div></div><div hidden id="mosaml-feedback-overlay"></div>'
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
            '#markup' => '<b>Want to test any of the Premium module before purchasing?</b> <br>Just send us a request, We will setup a demo site for you on our cloud and provide you with the administrator credentials.
                So that you can test all the premium features as per your requirement.
        <br>',
        );

        $form['customer_email'] = array(
            '#type' => 'textfield',
            '#attributes' => array('style' => 'width:100%','placeholder' => 'Enter your Email'),
        );

        $form['demo_plan'] = array(
            '#type' => 'select',
            '#title' => t('Demo Plan'),
            '#attributes' => array('style' => 'width:100%;'),
            '#options' => [
                'Drupal 7 OAuth Standard Module' => t('Drupal 7 OAuth Standard Module'),
                'Drupal 7 OAuth Premium Module' => t('Drupal 7 OAuth Premium Module'),
                'Drupal 7 OAuth Enterprise Module' => t('Drupal 7 OAuth Enterprise Module'),
                'Not Sure' => t('Not Sure'),
            ],
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

    /**
     * This function sends the support query
     */
    public static function send_query($email, $phone, $query, $plan)
    {

        if(empty($email)||empty($query)){
            if(empty($email)) {
                drupal_set_message(t('The <b>Email Address</b> field is required.'), 'error');
            }
            if(empty($query)) {
                drupal_set_message(t('The <b>Query</b> field is required.'), 'error');
            }
            return;
        }
        if (!valid_email_address($email)) {
            drupal_set_message(t('The email address <b><u>' . $email . '</u></b> is not valid.'), 'error');
            return;
        }
        if(empty($phone))
            $phone = variable_get('miniorange_oauth_client_customer_admin_phone');
        $support = new MiniorangeOAuthClientSupport($email, $phone, $query, $plan);
        $support_response = $support->sendSupportQuery();
        if($support_response) {
            drupal_set_message(t('Support query sent successfully. We will get back to you shortly!'));
        }
        else {
            drupal_set_message(t('Error sending support query'), 'error');
        }
    }

    public static function isCurlInstalled() {
        if (in_array('curl', get_loaded_extensions())) {
            return 1;
        }
        else {
            return 0;
        }
    }
    public static function get_oauth_timestamp() {
        $url = 'https://login.xecurify.com/moas/rest/mobile/get-timestamp';
        $ch  = curl_init( $url );
        curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
        curl_setopt( $ch, CURLOPT_ENCODING, "" );
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
        curl_setopt( $ch, CURLOPT_AUTOREFERER, true );
        curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );
        curl_setopt( $ch, CURLOPT_SSL_VERIFYHOST, false ); // required for https urls
        curl_setopt( $ch, CURLOPT_MAXREDIRS, 10 );
        curl_setopt( $ch, CURLOPT_POST, true );
        $content = curl_exec( $ch );
        if ( curl_errno( $ch ) ) {
            echo 'Error in sending curl Request';
            exit ();
        }
        curl_close( $ch );
        if(empty( $content )){
            $currentTimeInMillis = round( microtime( true ) * 1000 );
            $currentTimeInMillis = number_format( $currentTimeInMillis, 0, '', '' );
        }
        return empty( $content ) ? $currentTimeInMillis : $content;
    }
    public static function faq(&$form, &$form_state){

        $form['miniorange_faq'] = array(
            '#markup' => '<br><div class="mo_saml_text_center"><b></b>
                          <a class="btn1 btn-primary-faq btn-large mo_faq_button_left" href="https://faq.miniorange.com/kb/oauth-openid-connect/" target="_blank">FAQs</a>
                          <b></b><a class="btn1 btn-primary-faq btn-large mo_faq_button_right" href="https://forum.miniorange.com/" target="_blank">Ask questions on forum</a></div>',
        );
    }

    /**
     * OAuth Provider Guide block
     */
    public static function spConfigGuide(&$form, &$form_state){
        $form['miniorange_idp_setup_guide_link'] = array(
            '#markup' => '<div class="mo_saml_table_layout mo_saml_container_2" id="mo_oauth_guide_vt">',
        );

        $form['miniorange_idp_guide_link1'] = array(
            '#markup' => '<div style="font-size: 15px;"><b>To see detailed documentation of how to configure Drupal OAuth Client with any OAuth Server</b></div></br>',
        );

        $form['miniorange_oauth_guide_table_list'] = array(
            '#markup' => '<div class="table-responsive mo_guide_text-center" style="font-family: sans-serif;font-size: 15px;">          
                <table class="mo_guide_table mo_guide_table-striped mo_guide_table-bordered" style="border: 1px solid #ddd;max-width: 100%;border-collapse: collapse;">
                    <thead>
                        <tr><th class="mo_guide_text-center" colspan="2"><strong>Providers</strong></th></tr>
                    </thead>
                    <tbody>
                        <tr><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/setup-guide-to-configure-azure-ad-with-drupal-oauth-client" target="_blank">Azure AD</a></td> <td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/setup-guide-to-configure-line-with-drupal-oauth-client" target="_blank">Line</a></td></tr>
                        <tr><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-slack-as-as-oauth-openid-connect-server-in-drupal" target="_blank">Slack</a></td>   <td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-fitbit-oauth-server-for-drupal-8" target="_blank">Fitbit</a></td></tr>
                        <tr><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-google-oauth-server-drupal-8" target="_blank">Google</a></td>                       <td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-linkedin-as-an-oauth-openid-connect-server-for-drupal-8-client" target="_blank">LinkedIn</a></td></tr>
                        <tr><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/guide-to-configure-keycloak-for-drupal-oauth-client-module" target="_blank">Keycloak</a></td><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/guide-salesforce-configuration-drupal-oauth-client-module" target="_blank">Salesforce</a></td></tr>
                        <tr><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-github-oauthopenid-connect-server-drupal-8" target="_blank">Github</a></td>         <td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/guide-configure-box-drupal" target="_blank">Box</a></td></tr>
                        <tr><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-facebook-oauth-server-for-drupal-8" target="_blank">Facebook</a> </td>              <td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-instagram-as-an-oauth-openid-connect-server-for-drupal-8-client" target="_blank">Instagram</a></strong></td></tr>
                        <tr><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/setup-guide-to-configure-discord-with-drupal-oauth-client" target="_blank">Discord</a> </td>  <td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-reddit-oauthopenid-connect-server-drupal-8" target="_blank">Reddit</a></strong></td></tr>
                        <tr><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/guide-to-configure-wildapricot-with-drupal" target="_blank">Wild Apricot</a> </td>            <td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/guide-configure-zendesk-drupal" target="_blank">Zendesk</a> </td>
                        <tr><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/guide-to-configure-okta-with-drupal" target="_blank">Okta</a></strong></td><td class="mo_guide_text-center"><a class="mo_guide_text-color" href="https://plugins.miniorange.com/guide-to-enable-miniorange-oauth-client-for-drupal" target="_blank">Other Providers</a></strong></td></tr>
                    </tbody>
                </table>
                <div>In case you do not find your desired OAuth Server listed here, please mail us on <a href="mailto:drupalsupport@xecurify.com">drupalsupport@xecurify.com</a>
                    and we will help you to set it up.</div>
            </div>',
        );
        self::faq($form, $form_state);
    }

    /**
     * Advertise OAuth Server Module
     */
    public static function advertiseServer(&$form, &$form_state){
        global $base_url;
        $module_path = drupal_get_path('module', 'oauth_login_oauth2');
        $form['miniorange_oauth_client_setup_guide_link'] = array(
            '#markup' => '<div id="mo_advertise" class="mo_saml_table_layout mo_saml_container_2" id="mo_oauth_guide_vt">',
        );

        $form['miniorange_oauth_client_guide_link1'] = array(
            '#markup' => '<div style="font-size: 15px;"><i>Looking for a Drupal OAuth Server module? Now create your own Drupal site as an OAuth Server.</i></div></br>',
        );

        $form['miniorange_oauth_client_guide_table_list'] = array(
            '#markup' => '<div class="table-responsive mo_guide_text-center" style="font-family: sans-serif;font-size: 15px;">
                <table class="" style="border: none !important;max-width: 100%;border-collapse: collapse;">
                    <thead>
                        <tr><th class="mo_guide_text-center" style="border: none;"><img src="'.$base_url.'/'. $module_path . '/includes/images/miniorange.png" alt="Simply Easy Learning" height = 80px width = 80px ></th><th class="mo_guide_text-center" style = "border: none;"><b>Drupal OAuth Server( OAuth Provider) - Single Sign On (SSO)</b></th></tr>
                    </thead>
                </table>
                <div>
                    <p>OAuth Server allows Single Sign-On to your client apps with Drupal. It allows you to use Drupal as your OAuth Server and access OAuth API’s</p>
                    <br>
                </div>
                <table>
                    <tr>
                    <a class="btn btn-primary-color btn-large" style="padding:4px 8px;margin-right:14px;margin-bottom: 4px;" href="https://www.drupal.org/project/oauth_server_sso" target ="_blank">
                        Download module
                    </a>
                    <a class="btn btn-primary-color btn-large" style="background-color:blue;padding:4px 8px;margin-right:14px;margin-bottom: 4px;" href="https://plugins.miniorange.com/drupal-oauth-server" target ="_blank">
                        Know more
                    </a>
                    </tr>
                </table>
            </div>',
        );
    }

    public static function isCustomerRegistered(){
        if (variable_get('miniorange_oauth_client_customer_admin_email', NULL) == NULL||
            variable_get('miniorange_oauth_client_customer_id', NULL) == NULL
            || variable_get('miniorange_oauth_client_customer_admin_token', NULL) == NULL ||
            variable_get('miniorange_oauth_client_customer_api_key', NULL) == NULL)
        {
            return TRUE;
        }else{
            return FALSE;
        }
    }

    public static function getVariableNames($class_name) {

        if($class_name == "mo_options_enum_client_configuration") {
            $class_object = array (
                'App_selected'  => 'miniorange_oauth_client_app',
                'App_name'  => 'miniorange_auth_client_app_name',
                'Display_link' => 'miniorange_auth_client_display_name',
                'Client_ID' => 'miniorange_auth_client_client_id',
                'Client_secret' => 'miniorange_auth_client_client_secret',
                'Client_scope' => 'miniorange_auth_client_scope',
                'Authorized_endpoint' => 'miniorange_auth_client_authorize_endpoint',
                'Access_token_endpoint' => 'miniorange_auth_client_access_token_ep',
                'Userinfo_endpoint' => 'miniorange_auth_client_user_info_ep',
                'Callback_url' => 'miniorange_auth_client_callback_uri',
                'credentials_via_header' => 'miniorange_oauth_send_with_header_oauth',
                'credentials_via_body' => 'miniorange_oauth_send_with_body_oauth',
                'Enable_login_with_oauth' => 'miniorange_oauth_enable_login_with_oauth',
            );
        }
        else if($class_name == "mo_options_enum_attribute_mapping") {
            $class_object = array(
                'Email_attribute_value'    => 'miniorange_oauth_client_email_attr_val',
                'Username_attribute_value' => 'miniorange_oauth_client_name_attr_val',
            );
        }
        else if($class_name == "mo_options_enum_signin_settings") {
            $class_object = array(
                'Base_URL_value'    => 'miniorange_oauth_client_base_url',
            );
        }
        return $class_object;
    }

    /*=======Show attribute list coming from server on Attribute Mapping tab =======*/

    public static function show_attr_list_from_idp(&$form, $form_state)
    {
        global $base_url;
        $server_attrs = variable_get('miniorange_oauth_client_attr_list_from_server');

        if(empty($server_attrs)){
            Utilities::spConfigGuide($form, $form_state);
            return;
        }

        $form['miniorange_idp_guide_link'] = array(
            '#markup' => '<div class="mo_saml_table_layout mo_saml_container_2">',
        );

        $form['miniorange_saml_attr_header'] = array(
            '#markup' => '<b>Attributes received from the OAuth Server:</b><br><br>'
        );

        $icnt =  count($server_attrs);
        if($icnt >= 8){
            $scrollkit = 'scrollit';
        }else{
            $scrollkit = '';
        }

        $form['mo_saml_attrs_list_idp'] = array(
            '#markup' => '<div class="table-responsive mo_guide_text-center" style="font-family: sans-serif;font-size: 12px;"><div class='.$scrollkit.'>
                <table class="mo_guide_table mo_guide_table-striped mo_guide_table-bordered" style="border: 1px solid #ddd;max-width: 100%;border-collapse: collapse;">
                    <thead>
                        <tr>
                            <th class="mo_guide_text-center mo_td_values">ATTRIBUTE NAME</th>
                            <th class="mo_guide_text-center mo_td_values">ATTRIBUTE VALUE</th>                         
                        </tr>
                    </thead>',
        );

        $someattrs = '';
        $attrroles = '';

        if(isset($server_attrs) && !empty($server_attrs))
        {
            foreach ($server_attrs as $attr_name => $attr_values)
            {
                if(is_array($attr_values) || is_object($attr_values)){
                    foreach ($attr_values as $key1=>$val)
                    {
                        $someattrs .= '<tr><td>' . $attr_name.'.'.$key1 . '</td><td>' ;
                        $someattrs .= $val . '</td></tr>';
                    }
                }
                else{
                    $someattrs .= '<tr><td>' . $attr_name . '</td><td>' ;
                    $someattrs .= $attr_values . '</td></tr>';
                }

                if( $attr_name == 'roles' && is_array($server_attrs['roles']))
                {
                    foreach ($attr_values as $attr_roles => $role)
                    {
                        $attrroles .=  $role . ' | ';
                    }
                    $someattrs .=  $attrroles.'</td></tr>';
                }

            }
        }

        $form['miniorange_saml_guide_table_list'] = array(
            '#markup' => '<tbody style="font-weight:bold;font-size: 12px;color:gray;">'.$someattrs.'</tbody></table></div>',
        );

        $form['miniorange_break'] = array(
            '#markup' => '<br>',
        );

        $form['miniorange_saml_clear_attr_list'] = array(
            '#type' => 'submit',
            '#value' => t('Clear Attribute List'),
            '#submit' => array('clear_attr_list'),
            '#id' => 'button_config_center',
            '#limit_validation_errors' => array(),
        );

        $form['miniorange_saml_guide_clear_list_note'] = array(
            '#markup' => '<br><div style="font-size: 13px;"><b>NOTE : </b>Please clear this list after configuring the module to hide your confidential attributes.<br>
                            Click on <b>Test configuration</b> in <b>CONFIGURE OAUTH</b> tab to populate the list again.</div>',
        );

        $form['miniorange_saml_guide_table_end'] = array(
            '#markup' => '</div>',
        );
    }
}