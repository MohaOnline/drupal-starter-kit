<?php

/**
 * @author: miniOrange
 */
include "mo_saml_visualTour.php";
include_once('includes/Utilities.php');

function miniorange_config($form, &$formstate)
{
    global $base_url;
    drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    $login_path = '<a href='.$base_url.'/?q=moLogin>Enter what you want to display on the link</a>';
    $module_path = drupal_get_path('module', 'oauth_login_oauth2');
    $google_path = '';
    $strava_path = '';
    $fitbit_path='';
    $app_name_selected = variable_get('miniorange_oauth_client_app', NULL);
    $client_id = variable_get('miniorange_auth_client_client_id', NULL);
    if(!empty($app_name_selected) && !empty($client_id)){
        $disabled = TRUE;
        $attributes_arr =  array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;');
    }
    else{
        $disabled = FALSE;
        $attributes_arr =  array('style' => 'width:73%;');
    }

    $form['header_top_style_1'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

    $form['markup_top'] = array(
        '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
    );

    $form['markup_top_vt_start'] = array(
        '#markup' => '<b><span style="font-size: 17px;">CONFIGURE OAUTH APPLICATION</span></b>
                        <a class="btn btn-primary btn-large restart_button" id="restart_tour_button">
                        Restart Tour</a><br><br><hr><br/>'
    );

    $form['mo_configure_select_vt'] = array(
        '#markup'=>'<div id="mo_configure_selectapp_vt">'
    );

    $form['miniorange_oauth_client_app_options'] = array(
        '#type' => 'value',
        '#id' => 'cminiorange_oauth_client_app_options',
        '#value' => array(
            'Select' => t('Select'),
            'Google' => t('Google'),
            'Facebook' => t('Facebook'),
            'Windows Account' => t('Windows Account'),
            'Strava' => t('Strava'),
            'FitBit' => t('FitBit'),
            'Custom' => t('Custom OAuth 2.0 Provider')),
    );

    $form['miniorange_oauth_client_app'] = array(
        '#title' => t('Select Application:<span class="form-required"> *</span>'),
        '#id' => 'miniorange_oauth_client_app',
        '#type' => 'select',
        '#description' => "Select an OAuth Server",
        '#options' => $form['miniorange_oauth_client_app_options']['#value'],
        '#default_value' => variable_get('miniorange_oauth_client_app', NULL),
        '#disabled' => $disabled,
        '#attributes' => $attributes_arr,
    );

    $form['mo_configure_vt'] = array(
        '#markup'=>'</div><div id="mo_oauth_callback_vt">'
    );


    $form['miniorange_oauth_callback'] = array(
        '#type' => 'textfield',
        '#title' => t('Callback/Redirect URL: '),
        '#id'  => 'callbackurl',
        '#default_value' => variable_get('miniorange_auth_client_callback_uri',NULL),
        '#disabled' => true,
        '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;'),
    );

    $form['mo_configure_vt_1'] = array(
        '#markup'=>'</div><div id="mo_select_app_config_vt">'
    );

    $form['miniorange_oauth_app_name'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('miniorange_auth_client_app_name',NULL),
        '#id'  => 'miniorange_oauth_client_app_name',
        '#title' => t('Custom App Name:<span class="form-required"> *</span>'),
        '#disabled' => $disabled,
        '#attributes' => $attributes_arr,
    );

    $form['miniorange_oauth_client_display_name'] = array(
        '#type' => 'textfield',
        '#id'  => 'miniorange_oauth_client_display_name',
        '#default_value' => variable_get('miniorange_auth_client_display_name',NULL),
        '#title' => t('Display Name: '),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_id'] = array(
        '#type' => 'textfield',
        '#id'  => 'miniorange_oauth_client_client_id',
        '#default_value' => variable_get('miniorange_auth_client_client_id',NULL),
        '#title' => t('Client Id:<span class="form-required"> *</span>'),
        '#description' => "You will get this value from your OAuth Server",
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_secret'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('miniorange_auth_client_client_secret',NULL),
        '#description' => "You will get this value from your OAuth Server",
        '#id'  => 'miniorange_oauth_client_client_secret',
        '#title' => t('Client Secret:<span class="form-required"> *</span>'),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_scope'] = array(
        '#type' => 'textfield',
        '#id'  => 'miniorange_oauth_client_scope',
        '#default_value' => variable_get('miniorange_auth_client_scope',NULL),
        '#description' => "You can edit the value of this field but we highly recommend not change the default values of this field",
        '#title' => t('Scope: '),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_authorize_endpoint'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('miniorange_auth_client_authorize_endpoint',NULL),
        '#id'  => 'miniorange_oauth_client_auth_ep',
        '#title' => t('Authorize Endpoint:<span class="form-required"> *</span>'),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_access_token_endpoint'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('miniorange_auth_client_access_token_ep',NULL),
        '#id'  => 'miniorange_oauth_client_access_token_ep',
        '#title' => t('Access Token Endpoint:<span class="form-required"> *</span>'),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_userinfo_endpoint'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('miniorange_auth_client_user_info_ep',NULL),
        '#id'  => 'miniorange_oauth_client_user_info_ep',
        '#title' => t('Get User Info Endpoint:<span class="form-required"> *</span>'),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['mo_btn_breaks'] = array(
        '#markup' => "</div><br>",
    );

    $disable_true="";
    $disableval = False;
    $miniorange_auth_client_client_id = variable_get('miniorange_auth_client_client_id',NULL);
    $miniorange_auth_client_client_secret = variable_get('miniorange_auth_client_client_secret',NULL);
    $miniorange_auth_client_authorize_endpoint = variable_get('miniorange_auth_client_authorize_endpoint',NULL);
    $miniorange_auth_client_access_token_ep = variable_get('miniorange_auth_client_access_token_ep',NULL);
    $miniorange_auth_client_user_info_ep = variable_get('miniorange_auth_client_user_info_ep',NULL);
    if(empty($miniorange_auth_client_client_id) || empty($miniorange_auth_client_client_secret) || empty($miniorange_auth_client_authorize_endpoint)
        || empty($miniorange_auth_client_access_token_ep) || empty($miniorange_auth_client_user_info_ep)){
        $disable_true = 'disabled="True"';
        $disableval = TRUE;
    }


    $form['miniorange_oauth_client_config_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Configuration'),
        '#submit' => array('miniorange_oauth_client_save_config'),
        '#id' => 'button_config',
    );

    $form['miniorange_oauth_client_test_config_button'] = array(
        '#id' => 'miniorange_oauth_client_test_config_button',
        '#markup' => '<a '.$disable_true.' class="btn btn-primary-color btn-large" style="padding:4px 8px;margin-right:14px;margin-bottom: 4px;" onclick="testConfig(\'' . getTestUrl() . '\');">'
            . 'Test Configuration</a>'
    );

    $form['miniorange_oauth_client_reset_config_button'] = array(
        '#type' => 'submit',
        '#id' => 'button_config',
        '#value' => t('Reset Configuration'),
        '#disabled' => $disableval,
        '#submit' => array('miniorange_oauth_client_reset_config'),
        '#attributes' => array('style' => 'color: #fff;background-color: #d7342e;
                                        text-shadow: 0 -1px 1px #d7342e, 1px 0 1px #d7342e, 0 1px 1px #d7342e, -1px 0 1px #d7342e;box-shadow: 0 1px 0 #d7342e;border-color: #d7342e;'),
    );

    $form['miniorange_oauth_login_link'] = array(
        '#id'  => 'miniorange_oauth_login_link',
        '#markup' => "<br><br><div style='background-color: rgba(173,216,230,0.3); padding: 15px;font-family: sans-serif '>
            <br><div style='font-size: 1.3em;'> <strong>Instructions to add login link to different pages in your Drupal site: <br><br></strong></div>
            <div style='font-size: 1.1em;'>After completing your configurations, by default you will see a login link on your drupal site's login page. However, if you want to add login link somewhere else, please follow the below given steps:</div>
            <div style='padding-left: 15px;padding-top:5px;'>
            <div style='font-size: 0.9em;'>
            <li style='padding: 3px'>Go to <b>Structure</b> -> <b>Blocks</b></li>
            <li style='padding: 3px'> Click on <b>Add block</b></li>
            <li style='padding: 3px'>Enter <b>Block Title</b> and the <b>Block description</b></li>
            <li style='padding: 3px'>Under the <b>Block body</b> enter the following URL:
                <ol><pre>&lt;a href=&lt;your domain&gt;/?q=moLogin&lt;enter text you want to show on the link&lt;/a&gt;</pre></ol>
                <ol>For example: If your domain name is <b>https://www.miniorange.com</b> then, enter: <b>&lt;a href= 'https://www.miniorange.com/?q=moLogin'&gt Click here to Login&lt;/a&gt;</b> in the <b>Block body</b> textfield </ol>
            </li>
            <li style='padding: 3px'>From the text filtered dropdown select either <b>Filtered HTML</b> or <b>Full HTML</b></li>
            <li style='padding: 3px'>From the division under <b>REGION SETTINGS</b> select where do you want to show the login link</li>
            <li style='padding: 3px'>Click on the <b>SAVE block</b> button to save your settings</li><br>
            </div>
        </div>
    </div>",

        '#attributes' => array(),
    );

    $form['miniorange_oauth_break'] = array(
        '#markup' => '<br><br>',
    );

    $form['mo_ma_div_close'] = array('#markup' => '</div>',);

    Utilities::spConfigGuide($form, $form_state);
    $form['mo_ma_1_div_close'] = array('#markup' => '</div>',);
    Utilities::advertiseServer($form, $form_state);

    $form['mo_markup_div_imp']=array('#markup'=>'</div>');

    Utilities::AddSupportButton($form, $form_state);

    $form['mo_markup_div_imp']=array('#markup'=>'</div>');

    $form['miniorange_oauth_client_config_button'] = array(
        '#markup' => "<script>
                            jQuery(document).ready(function() {
                            jQuery('.form-item-miniorange-oauth-client-facebook-instr').show();
                            jQuery('.form-item-miniorange-oauth-client-eve-instr').hide();
                            jQuery('.form-item-miniorange-oauth-client-google-instr').hide();
                            jQuery('.form-item-miniorange-oauth-client-other-instr').hide();
                            jQuery('.form-item-miniorange-oauth-client-strava-instr').hide();
                            jQuery('.form-item-miniorange-oauth-client-fitbit-instr').hide();
                            jQuery('#miniorange_oauth_client_app').parent().show();
                            jQuery('#miniorange_oauth_client_app').change(function()
                            {
                                var appname = document.getElementById('miniorange_oauth_client_app').value;
                                if(appname=='Facebook' || appname=='Google' || appname=='Windows Account' || appname=='Custom' || appname=='Strava' || appname=='FitBit' || appname=='Eve Online'){
                                    jQuery('#mo_oauth_app_name_div').parent().show();
                                    jQuery('#miniorange_oauth_client_app_name').parent().show();
                                    jQuery('#miniorange_oauth_client_display_name').parent().show();
                                    jQuery('#miniorange_oauth_client_client_id').parent().show();
                                    jQuery('#miniorange_oauth_client_client_secret').parent().show();
                                    jQuery('#miniorange_oauth_client_scope').parent().show();
                                    jQuery('#miniorange_oauth_login_link').parent().show();
                                    jQuery('#test_config_button').show();
                                    jQuery('#callbackurl').val('".$base_url.'/?q=mo_login'."').parent().show();
                                    jQuery('#mo_oauth_authorizeurl').attr('required','true');
                                    jQuery('#mo_oauth_accesstokenurl').attr('required','true');
                                    jQuery('#mo_oauth_resourceownerdetailsurl').attr('required','true');
                                    jQuery('#miniorange_oauth_client_auth_ep').parent().show();
                                    jQuery('#miniorange_oauth_client_access_token_ep').parent().show();
                                    jQuery('#miniorange_oauth_client_user_info_ep').parent().show();

                                    if(appname=='Facebook'){
                                        document.getElementById('miniorange_oauth_client_scope').value='email';
                                        document.getElementById('miniorange_oauth_client_auth_ep').value='https://www.facebook.com/dialog/oauth';
                                        document.getElementById('miniorange_oauth_client_access_token_ep').value='https://graph.facebook.com/v2.8/oauth/access_token';
                                        document.getElementById('miniorange_oauth_client_user_info_ep').value='https://graph.facebook.com/me/?fields=id,name,email,age_range,first_name,gender,last_name,link&access_token=';
                                    }else if(appname=='Google'){
                                        document.getElementById('miniorange_oauth_client_scope').value='email';
                                        document.getElementById('miniorange_oauth_client_auth_ep').value='https://accounts.google.com/o/oauth2/auth';
                                        document.getElementById('miniorange_oauth_client_access_token_ep').value='https://www.googleapis.com/oauth2/v3/token';
                                        document.getElementById('miniorange_oauth_client_user_info_ep').value='https://www.googleapis.com/plus/v1/people/me';
                                    }else if(appname=='Windows Account'){
                                        document.getElementById('miniorange_oauth_client_scope').value='email';
                                        document.getElementById('miniorange_oauth_client_auth_ep').value='https://login.live.com/oauth20_authorize.srf';
                                        document.getElementById('miniorange_oauth_client_access_token_ep').value='https://login.live.com/oauth20_token.srf';
                                        document.getElementById('miniorange_oauth_client_user_info_ep').value='https://apis.live.net/v5.0/me';
                                    }else if(appname=='Custom'){
                                        document.getElementById('miniorange_oauth_client_auth_ep').value='';
                                        document.getElementById('miniorange_oauth_client_access_token_ep').value='';
                                        document.getElementById('miniorange_oauth_client_user_info_ep').value='';
                                    }
                                    if(appname=='Strava'){
                                        document.getElementById('miniorange_oauth_client_scope').value='public';
                                        document.getElementById('miniorange_oauth_client_auth_ep').value='https://www.strava.com/oauth/authorize';
                                        document.getElementById('miniorange_oauth_client_access_token_ep').value='https://www.strava.com/oauth/token';
                                        document.getElementById('miniorange_oauth_client_user_info_ep').value='https://www.strava.com/api/v3/athlete';
                                    }else if(appname=='FitBit'){
                                        document.getElementById('miniorange_oauth_client_scope').value='profile';
                                        document.getElementById('miniorange_oauth_client_auth_ep').value='https://www.fitbit.com/oauth2/authorize';
                                        document.getElementById('miniorange_oauth_client_access_token_ep').value='https://api.fitbit.com/oauth2/token';
                                        document.getElementById('miniorange_oauth_client_user_info_ep').value='https://api.fitbit.com/1/user/-/profile.json';
                                    }
                                    else if(appname == 'Eve Online')
                                    {
                                        document.getElementById('miniorange_oauth_client_scope').value='characterContactsRead';
                                    }
                                }
                            })
                        }
                        );
                </script>"
    );

    return $form;
}

/**
 * Save configuration function
 */
function miniorange_oauth_client_save_config($form, &$form_state)
{
    global $base_url;
    if((isset($_GET)) && ($_GET['action'] = 'update') )
        $_GET['action'] = NULL;

    if(isset($form['miniorange_oauth_client_app']))
        $client_app =  $form['miniorange_oauth_client_app']['#value'];
    if(isset($form['miniorange_oauth_app_name']['#value']))
        $app_name = $form['miniorange_oauth_app_name']['#value'];
    if(isset($form['miniorange_oauth_client_display_name']['#value']))
        $display_name = $form['miniorange_oauth_client_display_name'] ['#value'];
    if(isset($form['miniorange_oauth_client_id']))
        $client_id = $form['miniorange_oauth_client_id']['#value'];
    if(isset($form['miniorange_oauth_client_secret']['#value']))
        $client_secret = $form['miniorange_oauth_client_secret'] ['#value'];
    if(isset($form['miniorange_oauth_client_scope']['#value']))
        $scope = $form['miniorange_oauth_client_scope']['#value'];
    if(isset($form['miniorange_oauth_client_authorize_endpoint']['#value']))
        $authorize_endpoint = $form['miniorange_oauth_client_authorize_endpoint'] ['#value'];
    if(isset($form['miniorange_oauth_client_access_token_endpoint']['#value']))
        $access_token_ep = $form['miniorange_oauth_client_access_token_endpoint']['#value'];
    if(isset($form['miniorange_oauth_client_userinfo_endpoint']['#value']))
        $user_info_ep = $form['miniorange_oauth_client_userinfo_endpoint'] ['#value'];

    if(($client_app=='Select') || empty($client_app) || empty($app_name) || empty($client_id) || empty($client_secret)
        || empty($authorize_endpoint) || empty($access_token_ep) || empty($user_info_ep))
    {
        if(empty($client_app)|| $client_app == 'Select'){
            drupal_set_message(t('The <b>Select Application</b> dropdown is required. Please Select your application.'), 'error');
            return;
        }
        drupal_set_message(t('The <b>Custom App name</b>, <b>Client ID</b>, <b>Client Secret</b>, <b>Authorize Endpoint</b>, <b>Access Token Endpoint</b>
                , <b>Get User Info Endpoint</b> fields are required.'), 'error');
    }

    if(empty($client_app))
    {
        $client_app = variable_get('miniorange_oauth_client_app','');
    }
    if(empty($app_name))
    {
        $client_app = variable_get('miniorange_auth_client_app_name','');
    }
    if(empty($client_id))
    {
        $client_id = variable_get('miniorange_auth_client_client_id','');
    }
    if(empty($client_secret))
    {
        $client_secret = variable_get('miniorange_auth_client_client_secret','');
    }
    if(empty($scope))
    {
        $scope = variable_get('miniorange_auth_client_scope','');
    }
    if(empty($authorize_endpoint))
    {
        $authorize_endpoint = variable_get('miniorange_auth_client_authorize_endpoint','');
    }
    if(empty($access_token_ep))
    {
        $access_token_ep = variable_get('miniorange_auth_client_access_token_ep','');
    }
    if(empty($user_info_ep))
    {
        $user_info_ep = variable_get('miniorange_auth_client_user_info_ep','');
    }

    $callback_uri = $base_url."/?q=mo_login";
    $app_values = array();
    $app_values['client_id'] = $client_id;
    $app_values['client_secret'] = $client_secret;
    $app_values['app_name'] = $app_name;
    $app_values['display_name'] = $display_name;
    $app_values['scope'] = $scope;
    $app_values['authorize_endpoint'] = $authorize_endpoint;
    $app_values['access_token_ep'] = $access_token_ep;
    $app_values['user_info_ep'] = $user_info_ep;
    $app_values['callback_uri'] = $callback_uri;
    $app_values['client_app'] = $client_app;

    variable_set('miniorange_oauth_client_app', $client_app);
    variable_set('miniorange_oauth_client_appval', $app_values);
    variable_set('miniorange_auth_client_app_name', $app_name);
    variable_set('miniorange_auth_client_display_name', $display_name);
    variable_set('miniorange_auth_client_client_id', $client_id);
    variable_set('miniorange_auth_client_client_secret', $client_secret);
    variable_set('miniorange_auth_client_scope', $scope);
    variable_set('miniorange_auth_client_authorize_endpoint', $authorize_endpoint);
    variable_set('miniorange_auth_client_access_token_ep', $access_token_ep);
    variable_set('miniorange_auth_client_user_info_ep', $user_info_ep);
    variable_set('miniorange_auth_client_callback_uri',$callback_uri);
    variable_set('miniorange_auth_client_stat',"edit-application");
    drupal_set_message(t('Configurations saved successfully.'));
}

/**
 * Reset configuration function
 */

function miniorange_oauth_client_reset_config($form, &$form_state)
{
    variable_del('miniorange_oauth_client_app');
    variable_del('miniorange_oauth_client_appval');
    variable_del('miniorange_auth_client_client_id');
    variable_del('miniorange_auth_client_app_name');
    variable_del('miniorange_auth_client_display_name');
    variable_del('miniorange_auth_client_client_secret');
    variable_del('miniorange_auth_client_scope');
    variable_del('miniorange_auth_client_authorize_endpoint');
    variable_del('miniorange_auth_client_access_token_ep');
    variable_del('miniorange_oauth_client_email_attr_val');
    variable_del('miniorange_oauth_client_name_attr_val');
    variable_del('miniorange_auth_client_user_info_ep');
    variable_del('miniorange_oauth_client_attr_list_from_server');
    variable_set('miniorange_auth_client_stat',"new-application");
    drupal_set_message(t('Configurations deleted successfully.'));
    return;
}

/**
 * Send support query.
 */
function send_support_query(&$form, $form_state)
{
    $email = $form['miniorange_oauth_email_address_support']['#value'];
    $phone = $form['miniorange_oauth_phone_number_support']['#value'];
    $query = $form['miniorange_oauth_support_query_support']['#value'];
    Utilities::send_query($email, $phone, $query);
}

function getTestUrl() {
    global $base_url;
    $testUrl = $base_url.'/?q=testConfig';
    return $testUrl;
}

echo '
        <script>
              function redirect_to_attribute_mapping(){
                 
                  var baseurl = window.location.href.replace("miniorange_oauth_client","miniorange_oauth_client/attr_mapping");
                  alert(baseurl);
                  window.location.href= baseurl;
              }
          </script>';