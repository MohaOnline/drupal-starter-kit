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
    $module_path = drupal_get_path('module', 'oauth_login_oauth2');
    $baseUrlValue = variable_get('miniorange_oauth_client_base_url');
    $baseUrlValue = empty($baseUrlValue) ? $base_url : $baseUrlValue;

    if(!empty(variable_get('miniorange_auth_client_app_name'))){
        $miniorange_auth_client_callback_uri = variable_get('miniorange_auth_client_callback_uri');
    }
    else{
        $miniorange_auth_client_callback_uri = $baseUrlValue."/?q=mo_login";
        variable_set('miniorange_auth_client_callback_uri',$miniorange_auth_client_callback_uri);
    }

    $login_path = '<a href='.$baseUrlValue.'/?q=moLogin>Enter what you want to display on the link</a>';
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

    $disabledButton = NULL;
    if(empty($client_id) || empty($app_name_selected)){
        $disabledButton = 'disabled';
    }

    $form['header_top_style_1'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

    $form['markup_top'] = array(
        '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
    );

    $form['markup_top_vt_start'] = array(
        '#markup' => '<div id="tabhead"><b><span style="font-size: 17px;">CONFIGURE OAUTH APPLICATION</span></b>&nbsp;&nbsp;
        <a class="btn btn-success btn-large" style="padding:5px 10px;margin-left: 0px;box-shadow: 0 1px 0 #006799;" '.$disabledButton.' onclick="show_backup_form()">Backup/Import</a>
        <a class="btn btn-primary btn-large restart_button" id="restart_tour_button"> Restart Tour</a><br><br><hr><br/></div>'
    );

    $form['markup_top_head'] = array(
        '#markup' => '<div border="1" id="backup_import_form" class="mo_oauth_backup_download">
        <h3>Backup/ Import Configurations</h3><hr><span class="mo_oauth_backup_cancel">
        <input type="button" class="btn btn-sm btn-danger" value="Cancel" onclick = "hide_backup_form()"/></a><br></span>'
      );

      $form['markup_1s'] = array(
        '#markup' => '<br><br><div class="mo_oauth_highlight_background_note_2" style="width: auto" ><p><b>NOTE: </b>This tab will help you to transfer your module configurations when you change your Drupal instance.
                            <br>Example: When you switch from test environment to production.<br>Follow these 3 simple steps to do that:<br>
                            <br><strong>1.</strong> Download module configuration file by clicking on the Download Configuration button given below.
                            <br><strong>2.</strong> Install the module on new Drupal instance.<br><strong>3.</strong> Upload the configuration file in Import module Configurations section.<br>
                            <br><b>And just like that, all your module configurations will be transferred!</b></p></div>',
      );

      $form['mo_markup_top'] = array(
        '#markup' => '<div><br><br><div class="mo_oauth_container_3" style="text-align: center;float: left;margin-right: 20px;border: solid 1px #00000024;padding-bottom: 55px;">
                        <div><b><span style="font-size: 17px;">EXPORT CONFIGURATION</span></b><br><br><hr><br/><br>'
      );

    $form['miniorange_oauth_export'] = array(
          '#type' => 'submit',
          '#value' => t('Download Module Configuration'),
          '#prefix' => '<td>',
          '#suffix' => '</td>',
          '#submit' => array('miniorange_import_export'),
        );

      $form['miniorange_oauth_import'] = array(
        '#markup' => '</div></div><div class="mo_oauth_container_3" style="float: left;text-align:center;padding-bottom: 20px;border: solid 1px #00000024;;">
                          <b><span style="font-size: 17px;">IMPORT CONFIGURATION <b><a href="'.$base_url.'/admin/config/people/oauth_login_oauth2/licensing"> [Standard]</a></b></span></b><br><br><hr><br>  ',
      );

      $form['import_Config_file'] = array(
        '#type' => 'file',
        '#disabled' => TRUE,
        '#attributes' => array('style'=>'width: 175px;'),
      );

      $form['miniorange_oauth_import_file'] = array(
        '#type' => 'button',
        '#value' => t('Upload'),
        '#disabled' => TRUE,
        '#suffix' => '<br></div></div></div><div id="clientdata">',
      );

    $form['mo_configure_select_vt'] = array(
        '#markup'=>'<div id="mo_configure_selectapp_vt">'
    );

    $form['miniorange_oauth_client_app_options'] = array(
        '#type' => 'value',
        '#id' => 'cminiorange_oauth_client_app_options',
        '#value' => array(
            'Select' => t('Select'),
            'Azure AD' => t('Azure AD'),
            'Box' => t('Box'),
            'Discord' => t('Discord'),
            'Facebook' => t('Facebook'),
            'FitBit' => t('FitBit'),
            'Github' => t('Github'),
            'Google' => t('Google'),
            'Keycloak' => t('Keycloak'),
            'Line' => t('Line'),
            'LinkedIn' => t('LinkedIn'),
            'Okta_oauth' => t('Okta (OAuth)'),
            'Salesforce' => t('Salesforce'),
            'Slack' => t('Slack'),
            'Strava' => t('Strava'),
            'Wild Apricot' => t('Wild Apricot'),
            'Zendesk' => t('Zendesk'),
            'Custom' => t('Custom OAuth 2.0 Provider'),
            'Azure AD B2C' => t('Azure AD B2C (Premium and Enterprise)'),
            'AWS Cognito' => t('AWS Cognito (Premium and Enterprise)'),
            'Onelogin' => t('Onelogin (Premium and Enterprise)'),
            'miniOrange' => t('miniOrange (Premium and Enterprise)'),
            'Okta_openid' => t('Okta (OpenID) (Premium and Enterprise)'),
            'Custom_openid' => t('Custom OpenID Provider (We support OpenID protocol in Premium and Enterprise version)')),
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
        '#default_value' => $miniorange_auth_client_callback_uri,
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
        '#title' => t('Display Name:<span class="form-required"> *</span>'),
        '#disabled' => $disabled,
        '#attributes' => $attributes_arr,
    );

    $form['miniorange_oauth_client_display_name'] = array(
        '#type' => 'textfield',
        '#id'  => 'miniorange_oauth_client_display_name',
        '#default_value' => variable_get('miniorange_auth_client_display_name','Login using ##app_name##'),
        '#title' => t('Login link on the login page: '),
        '#description' => t('<b>Note:</b> The login link will appear on the user login page in this manner.'),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_id'] = array(
        '#type' => 'textfield',
        '#id'  => 'miniorange_oauth_client_client_id',
        '#maxlength' => 1048,
        '#default_value' => variable_get('miniorange_auth_client_client_id',NULL),
        '#title' => t('Client Id:<span class="form-required"> *</span>'),
        '#description' => "You will get this value from your OAuth Server",
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_secret'] = array(
        '#type' => 'textfield',
        '#maxlength' => 1048,
        '#default_value' => variable_get('miniorange_auth_client_client_secret',NULL),
        '#description' => "You will get this value from your OAuth Server",
        '#id'  => 'miniorange_oauth_client_client_secret',
        '#title' => t('Client Secret:<span class="form-required"> *</span>'),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['mo_vt_id_data2'] = array(
        '#markup' => '</div><div id = "mo_vt_add_data2">',
    );

    $form['miniorange_oauth_client_scope'] = array(
        '#type' => 'textfield',
        '#id'  => 'miniorange_oauth_client_scope',
        '#default_value' => variable_get('miniorange_auth_client_scope',NULL),
        '#description' => "You can edit the value of this field but we highly recommend not change the default values of this field",
        '#title' => t('Scope: '),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['mo_vt_id_data3'] = array(
        '#markup' => '</div><div id = "mo_vt_add_data5">',
    );

    $form['miniorange_oauth_client_authorize_endpoint'] = array(
        '#type' => 'textfield',
        '#maxlength' => 1048,
        '#default_value' => variable_get('miniorange_auth_client_authorize_endpoint',NULL),
        '#id'  => 'miniorange_oauth_client_auth_ep',
        '#title' => t('Authorize Endpoint:<span class="form-required"> *</span>'),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_access_token_endpoint'] = array(
        '#type' => 'textfield',
        '#maxlength' => 1048,
        '#default_value' => variable_get('miniorange_auth_client_access_token_ep',NULL),
        '#id'  => 'miniorange_oauth_client_access_token_ep',
        '#title' => t('Access Token Endpoint:<span class="form-required"> *</span>'),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['miniorange_oauth_client_userinfo_endpoint'] = array(
        '#type' => 'textfield',
        '#maxlength' => 1048,
        '#default_value' => variable_get('miniorange_auth_client_user_info_ep',NULL),
        '#id'  => 'miniorange_oauth_client_user_info_ep',
        '#title' => t('Get User Info Endpoint:<span class="form-required"> *</span>'),
        '#attributes' => array('style' => 'width:73%'),
    );

    $form['background_2'] = array(
        '#markup' => t('<b>Send Client ID and secret in: </b> <div class="mo_oauth_tooltip"><img src="'.$base_url.'/'. $module_path . '/includes/images/info.png" alt="info icon" height="15px" width="15px"></div><div class="mo_oauth_tooltiptext"><b>Note:</b> This option depends upon the OAuth provider. In case you are unaware about what to save, keeping this default is the best practice.</div>'),
    );

    $form['background_1'] = array(
        '#markup' => "<div class='mo_oauth_highlight_background_note_2'>"
    );

    $form['miniorange_oauth_send_with_header_oauth'] = array(
        '#type' => 'checkbox',
        '#default_value' => variable_get('miniorange_oauth_send_with_header_oauth',1),
        '#title' => t('<b>Header</b>'),
    );

    $form['miniorange_oauth_send_with_body_oauth'] = array(
        '#type' => 'checkbox',
        '#default_value' => variable_get('miniorange_oauth_send_with_body_oauth'),
        '#title' => t('<b>Body</b>'),
    );

    $form['background_1_end'] = array(
        '#markup' => '</div>',
	'#suffix' => '<br>',
    );

    $form['mo_vt_id_data4'] = array(
        '#markup' => '</div><div id = "mo_vt_add_data3">',
    );

    $form['miniorange_oauth_enable_login_with_oauth'] = array(
        '#type' => 'checkbox',
        '#default_value' => variable_get('miniorange_oauth_enable_login_with_oauth',1),
        '#title' => t('<b>Enable login with OAuth</b>'),
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

    $form['mo_ma_div_close'] = array('#markup' => '</div></div>',);

    Utilities::spConfigGuide($form, $form_state);
    $form['mo_ma_1_div_close'] = array('#markup' => '</div>',);
    Utilities::advertiseServer($form, $form_state);

    $form['mo_markup_div_imp']=array('#markup'=>'</div>');

    Utilities::AddSupportButton($form, $form_state);

    $form['mo_markup_div_imp']=array('#markup'=>'</div>');

    $form['miniorange_oauth_client_config_button'] = array(
        '#markup' => "<script>
                            jQuery(document).ready(function() {
                            var v=document.getElementById('miniorange_oauth_client_app');
                            var i;
                            for (i = 18; i < 24; i++) {
                                v.options[i].disabled=true;
                            }
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
                                var callbackUrl = document.getElementById('callbackurl').value;
                                if(appname == 'Azure AD'){
                                    callbackUrl = callbackUrl.replace(/^http:\/\//i, 'https://');
                                    document.getElementById('callbackurl').value=callbackUrl;
                                }
                                else
                                    document.getElementById('callbackurl').value=callbackUrl;

                            if(appname=='Facebook' || appname=='Google' || appname=='Okta_oauth' || appname=='Slack' || appname=='Zendesk' || appname=='Box' || appname=='Github' || appname=='Wild Apricot' || appname=='Salesforce' || appname=='LinkedIn' || appname=='Azure AD' || appname=='Keycloak' || appname=='Custom' || appname=='Strava' || appname=='FitBit' || appname=='Discord' || appname=='Line'){
                                    jQuery('#mo_oauth_app_name_div').parent().show();
                                    jQuery('#miniorange_oauth_client_app_name').parent().show();
                                    jQuery('#miniorange_oauth_client_display_name').parent().show();
                                    jQuery('#miniorange_oauth_client_client_id').parent().show();
                                    jQuery('#miniorange_oauth_client_client_secret').parent().show();
                                    jQuery('#miniorange_oauth_client_scope').parent().show();
                                    jQuery('#miniorange_oauth_login_link').parent().show();
                                    jQuery('#test_config_button').show();
                                    jQuery('#callbackurl').parent().show();
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
                                    document.getElementById('miniorange_oauth_client_scope').value='email+profile';
                                    document.getElementById('miniorange_oauth_client_auth_ep').value='https://accounts.google.com/o/oauth2/auth';
                                    document.getElementById('miniorange_oauth_client_access_token_ep').value='https://www.googleapis.com/oauth2/v4/token';
                                    document.getElementById('miniorange_oauth_client_user_info_ep').value='https://www.googleapis.com/oauth2/v1/userinfo';
                                }else if(appname=='LinkedIn'){
                                  document.getElementById('miniorange_oauth_client_scope').value='r_basicprofile';
                                  document.getElementById('miniorange_oauth_client_auth_ep').value='https://www.linkedin.com/oauth/v2/authorization';
                                  document.getElementById('miniorange_oauth_client_access_token_ep').value='https://www.linkedin.com/oauth/v2/accessToken';
                                  document.getElementById('miniorange_oauth_client_user_info_ep').value='https://api.linkedin.com/v2/me';
                                }else if(appname=='Salesforce'){
                                  document.getElementById('miniorange_oauth_client_scope').value='id';
                                  document.getElementById('miniorange_oauth_client_auth_ep').value='https://login.salesforce.com/services/oauth2/authorize';
                                  document.getElementById('miniorange_oauth_client_access_token_ep').value='https://login.salesforce.com/services/oauth2/token';
                                  document.getElementById('miniorange_oauth_client_user_info_ep').value='https://login.salesforce.com/services/oauth2/userinfo';
                                }else if(appname=='Wild Apricot'){
                                  document.getElementById('miniorange_oauth_client_scope').value='auto';
                                  document.getElementById('miniorange_oauth_client_auth_ep').value='https://{your_account_url}/sys/login/OAuthLogin';
                                  document.getElementById('miniorange_oauth_client_access_token_ep').value='https://oauth.wildapricot.org/auth/token';
                                  document.getElementById('miniorange_oauth_client_user_info_ep').value='https://api.wildapricot.org/v2.1/accounts/{account_id}/contacts/me';
                                }else if(appname=='Azure AD'){
                                    document.getElementById('miniorange_oauth_client_scope').value='openid';
                                    document.getElementById('miniorange_oauth_client_auth_ep').value='https://login.microsoftonline.com/[tenant-id]/oauth2/authorize';
                                    document.getElementById('miniorange_oauth_client_access_token_ep').value='https://login.microsoftonline.com/[tenant-id]/oauth2/token';
                                    document.getElementById('miniorange_oauth_client_user_info_ep').value='https://login.windows.net/common/openid/userinfo';
                                }else if(appname=='Keycloak'){
                                    document.getElementById('miniorange_oauth_client_scope').value='email profile';
                                    document.getElementById('miniorange_oauth_client_auth_ep').value='{Keycloak_base_URL}/realms/{realm-name}/protocol/openid-connect/auth';
                                    document.getElementById('miniorange_oauth_client_access_token_ep').value='{Keycloak_base_URL}/realms/{realm-name}/protocol/openid-connect/token';
                                    document.getElementById('miniorange_oauth_client_user_info_ep').value='{Keycloak_base_URL}/realms/{realm-name}/protocol/openid-connect/userinfo';
                                }else if(appname=='Custom'){
                                    document.getElementById('miniorange_oauth_client_scope').value='email profile';
                                    document.getElementById('miniorange_oauth_client_auth_ep').value='';
                                    document.getElementById('miniorange_oauth_client_access_token_ep').value='';
                                    document.getElementById('miniorange_oauth_client_user_info_ep').value='';
                                } else if(appname=='Strava'){
                                    document.getElementById('miniorange_oauth_client_scope').value='public';
                                    document.getElementById('miniorange_oauth_client_auth_ep').value='https://www.strava.com/oauth/authorize';
                                    document.getElementById('miniorange_oauth_client_access_token_ep').value='https://www.strava.com/oauth/token';
                                    document.getElementById('miniorange_oauth_client_user_info_ep').value='https://www.strava.com/api/v3/athlete';
                                }else if(appname=='FitBit'){
                                    document.getElementById('miniorange_oauth_client_scope').value='profile';
                                    document.getElementById('miniorange_oauth_client_auth_ep').value='https://www.fitbit.com/oauth2/authorize';
                                    document.getElementById('miniorange_oauth_client_access_token_ep').value='https://api.fitbit.com/oauth2/token';
                                    document.getElementById('miniorange_oauth_client_user_info_ep').value='https://api.fitbit.com/1/user/-/profile.json';
                                }else if(appname=='Discord'){
                                  document.getElementById('miniorange_oauth_client_scope').value='identify email';
                                  document.getElementById('miniorange_oauth_client_auth_ep').value='https://discordapp.com/api/oauth2/authorize';
                                  document.getElementById('miniorange_oauth_client_access_token_ep').value='https://discordapp.com/api/oauth2/token';
                                  document.getElementById('miniorange_oauth_client_user_info_ep').value='https://discordapp.com/api/users/@me';
                                }else if(appname=='Line'){
                                  document.getElementById('miniorange_oauth_client_scope').value='Profile openid email';
                                  document.getElementById('miniorange_oauth_client_auth_ep').value='https://access.line.me/oauth2/v2.1/authorize';
                                  document.getElementById('miniorange_oauth_client_access_token_ep').value='https://api.line.me/oauth2/v2.1/token';
                                  document.getElementById('miniorange_oauth_client_user_info_ep').value='https://api.line.me/ v2/profile';
                                }else if(appname=='Zendesk'){
                                  document.getElementById('miniorange_oauth_client_scope').value='read write';
                                  document.getElementById('miniorange_oauth_client_auth_ep').value='https://{subdomain}.zendesk.com/oauth/authorizations/new';
                                  document.getElementById('miniorange_oauth_client_access_token_ep').value='https://{subdomain}.zendesk.com/oauth/tokens';
                                  document.getElementById('miniorange_oauth_client_user_info_ep').value='https://{subdomain}.zendesk.com/api/v2/users';
                                }else if(appname=='Box'){
                                  document.getElementById('miniorange_oauth_client_scope').value='root_readwrite';
                                  document.getElementById('miniorange_oauth_client_auth_ep').value='https://account.box.com/api/oauth2/authorize';
                                  document.getElementById('miniorange_oauth_client_access_token_ep').value='https://api.box.com/oauth2/token';
                                  document.getElementById('miniorange_oauth_client_user_info_ep').value='https://api.box.com/2.0/users/me';
                                }else if(appname=='Github'){
                                  document.getElementById('miniorange_oauth_client_scope').value='user repo';
                                  document.getElementById('miniorange_oauth_client_auth_ep').value='https://github.com/login/oauth/authorize';
                                  document.getElementById('miniorange_oauth_client_access_token_ep').value='https://github.com/login/oauth/access_token';
                                  document.getElementById('miniorange_oauth_client_user_info_ep').value='https://api.github.com/user';
                                }else if(appname=='Slack'){
                                    document.getElementById('miniorange_oauth_client_scope').value='users.profile:read';
                                    document.getElementById('miniorange_oauth_client_auth_ep').value='https://slack.com/oauth/authorize';
                                    document.getElementById('miniorange_oauth_client_access_token_ep').value='https://slack.com/api/oauth.access';
                                    document.getElementById('miniorange_oauth_client_user_info_ep').value='https://slack.com/api/users.profile.get';
                                }else if(appname=='Okta_oauth'){
                                    document.getElementById('miniorange_oauth_client_scope').value='openid email profile';
                                    document.getElementById('miniorange_oauth_client_auth_ep').value='https://{yourOktaDomain}.com/oauth2/default/v1/authorize';
                                    document.getElementById('miniorange_oauth_client_access_token_ep').value='https://{yourOktaDomain}.com/oauth2/default/v1/token';
                                    document.getElementById('miniorange_oauth_client_user_info_ep').value='https://{yourOktaDomain}.com/oauth2/default/v1/userinfo';
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
    $baseUrlValue = variable_get('miniorange_oauth_client_base_url');
    $baseUrlValue = empty($baseUrlValue) ? $base_url : $baseUrlValue;
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
        drupal_set_message(t('The <b>Display name</b>, <b>Client ID</b>, <b>Client Secret</b>, <b>Authorize Endpoint</b>, <b>Access Token Endpoint</b>
                , <b>Get User Info Endpoint</b> fields are required.'), 'error');
        return;
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

    if($client_app == 'Azure AD'){
        $baseUrlValue = preg_replace('/^http:/i', 'https:', $baseUrlValue);
        $callback_uri = $baseUrlValue."/?q=mo_login";
    }else
        $callback_uri = $baseUrlValue."/?q=mo_login";

    $app_values = variable_get('miniorange_oauth_client_appval');
    if(!is_array($app_values))
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
    $enable_login_with_oauth = $form['miniorange_oauth_enable_login_with_oauth']['#value'];
    $enable_with_header = $form['miniorange_oauth_send_with_header_oauth']['#value'];
    $enable_with_body = $form['miniorange_oauth_send_with_body_oauth']['#value'];
    $enable_header = $enable_with_header == 1 ? TRUE : FALSE ;
    $enable_body = $enable_with_body == 1 ? TRUE : FALSE ;

    variable_set('miniorange_oauth_enable_login_with_oauth',$enable_login_with_oauth);
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
    variable_set('miniorange_oauth_send_with_header_oauth', $enable_header);
    variable_set('miniorange_oauth_send_with_body_oauth', $enable_body);
    variable_set('miniorange_auth_client_stat',"edit-application");
    drupal_set_message(t('Configurations saved successfully.  Please click on the <b>Test Configuration</b> button to test the connection.'));
}

/**
 * Reset configuration function
 */

function miniorange_oauth_client_reset_config($form, &$form_state)
{
    global $base_url;
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

    if(!empty(variable_get('miniorange_oauth_client_base_url')))
            $baseUrlValue = variable_get('miniorange_oauth_client_base_url');
        else
            $baseUrlValue = $base_url;
    $miniorange_auth_client_callback_uri = $baseUrlValue."/?q=mo_login";
    variable_set('miniorange_auth_client_callback_uri',$miniorange_auth_client_callback_uri);

    drupal_set_message(t('Configurations deleted successfully.'));
    return;
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

function getTestUrl() {
    global $base_url;
    $testUrl = $base_url.'/?q=testConfig';
    return $testUrl;
}

//Export Feature

function miniorange_import_export() {
    $tab_class_name = array(
        'OAuth Client Configuration' => 'mo_options_enum_client_configuration',
        'Attribute Mapping' => 'mo_options_enum_attribute_mapping',
        'Sign In Settings' => 'mo_options_enum_signin_settings'
    );

	$configuration_array = array();
	foreach($tab_class_name as $key => $value) {
		$configuration_array[$key] = mo_get_configuration_array($value);
	}

	$configuration_array["Version_dependencies"] = mo_get_version_informations();
	header("Content-Disposition: attachment; filename = miniorange_oauth_client_config.json");
	echo(json_encode($configuration_array, JSON_PRETTY_PRINT|JSON_UNESCAPED_SLASHES));
	exit;
	}

  function mo_get_configuration_array( $class_name ) {
    $class_object = Utilities::getVariableNames($class_name);
    $mo_array = array();
    foreach ( $class_object as $key => $value ) {
      $mo_option_exists=variable_get($value);
      if($mo_option_exists){
        $mo_array[ $key ] = $mo_option_exists;
      }
    }
    return $mo_array;
  }

  function mo_get_version_informations(){
    $array_version = array();
    $array_version["PHP_version"] = phpversion();
    $array_version["Drupal_version"] = VERSION;
    $array_version["OPEN_SSL"] = mo_oauth_is_openssl_installed();
    $array_version["CURL"] = mo_oauth_is_curl_installed();
    $array_version["ICONV"] = mo_oauth_is_iconv_installed();
    $array_version["DOM"] = mo_oauth_is_dom_installed();
    return $array_version;
  }

  function mo_oauth_is_openssl_installed() {
    if ( in_array( 'openssl', get_loaded_extensions() ) ) {
      return 1;
    } else {
      return 0;
    }
  }

  function mo_oauth_is_curl_installed() {
    if ( in_array( 'curl', get_loaded_extensions() ) ) {
      return 1;
    } else {
      return 0;
    }
  }

  function mo_oauth_is_iconv_installed(){
    if ( in_array( 'iconv', get_loaded_extensions() ) ) {
      return 1;
    } else {
      return 0;
    }
  }

  function mo_oauth_is_dom_installed(){
    if ( in_array( 'dom', get_loaded_extensions() ) ) {
      return 1;
    } else {
      return 0;
    }
  }
