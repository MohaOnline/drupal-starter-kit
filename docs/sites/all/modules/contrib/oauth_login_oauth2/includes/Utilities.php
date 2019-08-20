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
            '#markup' => '<h3><b>Feature Request/Contact Us:</b></h3><div>Need any help? We can help you with configuring your Service Provider. Just send us a query and we will get back to you soon.<br /></div><br>',
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

        $form['miniorange_oauth_support_submit_click'] = array(
            '#type' => 'submit',
            '#value' => t('Submit Query'),
            '#submit' => array('send_support_query'),
            '#attributes' => array('style' => 'background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;display:block;margin-left:auto;margin-right:auto;'),
        );

        $form['miniorange_oauth_support_note'] = array(
            '#markup' => '<div><br/>If you want custom features in the plugin, just drop an email to <a href="mailto:info@xecurify.com">info@xecurify.com</a></div>'
        );

        $form['miniorange_oauth_support_div_cust'] = array(
            '#markup' => '</div></div><div hidden id="mosaml-feedback-overlay"></div>'
        );
    }

    /**
     * This function sends the support query
     */
    public static function send_query($email, $phone, $query)
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

        $support = new MiniorangeOAuthClientSupport($email, $phone, $query);
        $support_response = $support->sendSupportQuery();
        if($support_response) {
            drupal_set_message(t('Support query successfully sent'));
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
                        <tr><th class="mo_guide_text-center" style="font-weight:bold;">Providers</th><th class="mo_guide_text-center" style="font-weight:bold;">Links</th></tr>
                    </thead>
                    <tbody style="color:gray;">
                        <tr><td>AWS Cognito</td><td><strong><a href="https://plugins.miniorange.com/configure-aws-cognito-oauthopenid-connect-server-drupal" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>Reddit</td><td><strong><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-reddit-oauthopenid-connect-server-drupal" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>Google</td><td><strong><a class="mo_guide_text-color" href="https://plugins.miniorange.com/configure-google-apps-oauth-server-drupal" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>Facebook</td><td><strong><a href="https://plugins.miniorange.com/configure-facebook-oauth-server-drupal" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>FitBit</td><td><strong><a href="https://plugins.miniorange.com/configuring-fitbit-oauth-server-drupal" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>Instagram</td><td><strong><a href="https://plugins.miniorange.com/configure-instagram-oauth-openid-connect-server-drupal-client" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                        <tr><td>linkedin</td><td><strong><a href="https://plugins.miniorange.com/configure-linkedin-oauth-openid-connect-server-drupal-client" class="mo_guide_text-color" target="_blank">Click Here</a></strong></td></tr>
                    </tbody>
                </table>
                <div>In case you do not find your desired OAuth Provider listed here, please mail us on <a href="mailto:info@xecurify.com">info@xecurify.com</a>
                    and we will help you to set it up.</div>
            </div>',

        );
    }

    /**
     * Advertise OAuth Server Module
     */
    public static function advertiseServer(&$form, &$form_state){
        global $base_url;
        $module_path = drupal_get_path('module', 'oauth_login_oauth2');
        $form['miniorange_oauth_client_setup_guide_link'] = array(
            '#markup' => '<div class="mo_saml_table_layout mo_saml_container_2" id="mo_oauth_guide_vt">',
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

    /*=======Show attribute list coming from server on Attribute Mapping tab =======*/
    public static function show_attr_list_from_idp(&$form, $form_state)
    {
        global $base_url;
        $server_attrs =  variable_get('miniorange_oauth_client_attr_list_from_server');
        $client_id =  variable_get('miniorange_auth_client_client_id');
        $client_secret =  variable_get('miniorange_auth_client_client_secret');    

        if(empty($server_attrs) || empty($client_id  || $client_secret)){
            Utilities::spConfigGuide($form, $form_state);
            return;
        }

        $form['miniorange_idp_guide_link'] = array(
            '#markup' => '<div class="mo_saml_table_layout mo_saml_container_2">',
        );

        $form['miniorange_saml_attr_header'] = array(
            '#markup' => '<div style="font-size: 1.3em;font-weight: 600;font-family: sans-serif;">Attributes received from the OAuth Server:</div><br>'
        );

        /*$form['mo_saml_attrs_list_idp'] = array(
            '#markup' => '<embed src = "'.$base_url.'/?q=testConfig" style="width: 100%;height: 444px;">',
        );*/

        $module_path = drupal_get_path('module', 'miniorange_oauth_client');

        $form['miniorange_header_attr_list_server'] = array(
            '#markup' => '<div class="field_myiframe">                            
                              <iframe src="'.$base_url.'/?q=testConfig" width="100%" height="333px" target="[target_value]" class="iframe myclass">[URL]</iframe>
                          </div>'
        );


        /*Don't delete below commented code*/

       /* $form['mo_saml_attrs_list_idp'] = array(
            '#markup' => '<div class="table-responsive mo_guide_text-center" style="font-family: sans-serif;font-size: 12px;"><div class="scrollit">
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
                $someattrs .= '<tr><td>' . $attr_name . '</td><td>' ;
                if( $attr_name == 'roles' && is_array($server_attrs['roles']))
                {
                    foreach ($attr_values as $attr_roles => $role)
                    {
                        $attrroles .=  $role . ' | ';
                    }
                    $someattrs .=  $attrroles.'</td></tr>';
                }
                else
                {
                    $someattrs .= $attr_values . '</td></tr>';
                }
            }
        }

        $form['miniorange_saml_guide_table_list'] = array(
            '#markup' => '<tbody style="font-weight:bold;font-size: 12px;color:gray;">'.$someattrs.'</tbody></table></div>',
        );*/

        $form['miniorange_break'] = array(
            '#markup' => '<br>',
        );

        $form['miniorange_saml_clear_attr_list'] = array(
            '#type' => 'submit',
            '#value' => t('Clear Attribute List'),
            '#submit' => array('clear_attr_list'),
            '#id' => 'button_config_center',
        );

        $form['miniorange_saml_guide_clear_list_note'] = array(
            '#markup' => '<div style="font-size: 13px;"><b>NOTE : </b>Please clear this list after configuring the plugin to hide your confidential attributes.<br>
                            Click on <b>Test configuration</b> in <b>CONFIGURE OAUTH</b> tab to populate the list again.</div>',
        );

        $form['miniorange_saml_guide_table_end'] = array(
            '#markup' => '</div>',
        );
    }
}