<?php

/**
 * @file
 * Admin page callback file for the lr_ciam module.
 */
module_load_include('inc', 'lr_ciam', 'includes/lr_ciam.functions');
module_load_include('php', 'lr_ciam', 'LoginRadiusSDK/Utility/Functions');
module_load_include('php', 'lr_ciam', 'LoginRadiusSDK/LoginRadiusException');
module_load_include('php', 'lr_ciam', 'LoginRadiusSDK/Clients/IHttpClient');
module_load_include('php', 'lr_ciam', 'LoginRadiusSDK/Clients/DefaultHttpClient');
module_load_include('php', 'lr_ciam', 'LoginRadiusSDK/CustomerRegistration/Social/SocialLoginAPI');
module_load_include('php', 'lr_ciam', 'LoginRadiusSDK/Advance/CloudAPI');

use \LoginRadiusSDK\Utility\Functions;
use \LoginRadiusSDK\LoginRadiusException;
use \LoginRadiusSDK\Clients\IHttpClient;
use \LoginRadiusSDK\Clients\DefaultHttpClient;
use \LoginRadiusSDK\CustomerRegistration\Social\SocialLoginAPI;
use \LoginRadiusSDK\Advance\CloudAPI;


/**
 * Function that build an abstract representation of a HTML form for Social Login settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 * @return array A renderable form array for the respective request.
 */
function lr_ciam_admin_settings($form, &$form_state) {
        $podata = isset($_POST['op']) ? $_POST['op'] : '';
    if (!variable_get('lr_ciam_apikey', FALSE) || !variable_get('lr_ciam_apisecret', FALSE)) {
        if (isset($podata) && $podata != 'Save configuration') {
            drupal_set_message(t('To activate the social login, insert LoginRadius Site Name, API Key and Secret in the social login section below.'), 'warning');
        }
    }
    $api_key = variable_get('lr_ciam_apikey', '');
    $api_secret = variable_get('lr_ciam_apisecret', '');
    try {
        if (isset($api_key) && $api_key != '' && isset($api_secret) && $api_secret != '' && isset($podata) && $podata != 'Save configuration') {
            $cloudObject = new CloudAPI($api_key, $api_secret, array('output_format' => 'json'));
            $cloudObject = $cloudObject->getConfigurationList();
            if($cloudObject->IsPhoneLogin)
            {
                drupal_set_message(t('If user is registering using only PhoneID then a random Email Id will be generated in the format "PhoneID@yourdomain.com"'), 'warning');
            }
        }
        }catch (Exception $e) {
        watchdog('loginradius_logging', $e, array(), WATCHDOG_ERROR);
    }

    drupal_add_css(drupal_get_path('module', 'lr_ciam') . '/css/lr_ciam.admin.min.css');
    drupal_add_js('jQuery(document).ready(function() {
    jQuery("#edit-thanks-block").css({"background-color":"#FFFFE0"});
   });', array('type' => 'inline', 'scope' => 'header', 'weight' => 5));
    $get_lr_ciam = '';

    if (!variable_get('lr_ciam_apikey', FALSE)) {
        $get_lr_ciam = t('For creating a social login account, please visit <a href="@get_lr_ciam" target="_blank">LoginRadius</a>', array('@get_lr_ciam' => 'http://www.Loginradius.com/'));
    }

    $thanks_text = t('To activate the module, you will need to first configure it (manage your desired social networks, etc.) from your LoginRadius account. If you do not have an account, click <a href="@loginradius" target="_blank">here</a>.<br/>We also offer Social Plugins for
  <a href="@wordpress" target="_blank">Wordpress</a>,
  <a href="@drupal" target="_blank">Drupal</a>,
  <a href="@joomla" target="_blank">Joomla</a>,
  <a href="@magento" target="_blank">Magento</a>,
  <a href="@prestashop" target="_blank">Prestashop</a> and
  <a href="@dotnetnuke" target="_blank">DotNetNuke</a>  <br/><a href="@loginradius" target="_blank"><br/><input class="form-submit" type="button" value="Set up my account!"></a> (<a href="@get_sociallogin" target="_blank">How to set up an account?</a>)', array(
      '@loginradius' => 'http://ish.re/4',
      '@wordpress' => 'http://ish.re/10E78',
      '@drupal' => 'http://ish.re/TRXK',
      '@joomla' => 'http://ish.re/12B23',
      '@magento' => 'http://ish.re/UF5L',
      '@prestashop' => 'http://ish.re/TRXU',
      '@dotnetnuke' => 'http://ish.re/TRY1',
      '@get_sociallogin' => 'http://ish.re/1EVFR',
    ));
    $form['thanks_block'] = array(
      '#type' => 'fieldset',
      '#title' => t('Thank you for installing the LoginRadius CIAM Module!'),
      '#description' => $thanks_text,
    );
    $setup_group = 'setup';
    $form[$setup_group] = array(
      '#type' => 'fieldset',
      '#title' => t('LoginRadius API Settings'),
      '#id' => 'setup',
      '#collapsible' => TRUE,
      '#description' => $get_lr_ciam,
    );

    $form[$setup_group]['lr_ciam_apikey'] = array(
      '#type' => 'textfield',
      '#title' => t('API Key'),
      '#required' => TRUE,
      '#default_value' => variable_get('lr_ciam_apikey', ''),
      '#description' => t('To activate the module, enter LoginRadius API Key ( <a href="@get_lr_ciam" target="_blank">How to get it?</a> )', array('@get_lr_ciam' => 'http://ish.re/1EVFR')),
    );
    $form[$setup_group]['lr_ciam_apisecret'] = array(
      '#type' => 'textfield',
      '#title' => t('API Secret'),
      '#required' => TRUE,
      '#default_value' => variable_get('lr_ciam_apisecret', ''),
      '#description' => t('To activate the module, enter LoginRadius API Secret ( <a href="@get_lr_ciam" target="_blank">How to get it?</a> )', array('@get_lr_ciam' => 'http://ish.re/1EVFR')),
    );

    $basic_group = 'basic';
    $form[$basic_group] = array(
      '#type' => 'fieldset',
      '#title' => t('CIAM REDIRECT SETTINGS'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form[$basic_group]['lr_ciam_userlogin_redirect'] = array(
      '#type' => 'radios',
      '#title' => t('Redirection settings after login'),
      '#default_value' => variable_get('lr_ciam_userlogin_redirect', 0),
      '#options' => array(
        0 => t('Redirect to same page of site'),
        1 => t('Redirect to profile page of site'),
        2 => t('Redirect to custom page of site (If you want user to be redirected to specific URL after login)'),
      ),
    );
    $form[$basic_group]['lr_ciam_userlogin_redirect']['lr_ciam_custom_redirection'] = array(
      '#type' => 'textfield',
      '#weight' => 50,
      '#default_value' => variable_get('lr_ciam_custom_redirection'),
    );
    
    // Submit handler.
    $form['#submit'][] = 'lr_ciam_admin_settings_submit';
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
    return $form;
}


 
function lr_ciam_select_template($template_array)
{
    $templates = array();
    if(!in_array('default', $template_array) || empty($template_array))
            {
                array_push($template_array, 'default');
            }

            foreach ($template_array as $template) {
                $templates[$template] = $template;
            }
            array_unshift($templates, '-- Select Template --');
            
            return $templates;
}

function lr_ciam_lr_advance_settings($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'lr_ciam') . '/css/lr_ciam.admin.min.css');
    $api_key = variable_get('lr_ciam_apikey', '');
    $api_secret = variable_get('lr_ciam_apisecret', '');
    try {
        if (isset($api_key) && $api_key != '' && isset($api_secret) && $api_secret != '') {
            $cloudObject = new CloudAPI($api_key, $api_secret, array('output_format' => 'json'));
            $cloudObject = $cloudObject->getConfigurationList();
            $query_array = array(
              'apiKey' => $api_key,
              'apiSecret' => $api_secret
            );
            $options = array(
              'output_format' => 'json',
            );
            $url = 'https://config.lrcontent.com/ciam/appInfo/templates';
            $email_templates = Functions::apiClient($url, $query_array, $options);
        }
    }
    catch (Exception $e) {
        watchdog('loginradius_logging', $e, array(), WATCHDOG_ERROR);
    }

    // Basic user settings section.
    $user_settings_group = 'user_settings';
    $form[$user_settings_group] = array(
      '#type' => 'fieldset',
      '#title' => t('CIAM additional settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    ); 
    $form[$user_settings_group]['lr_ciam_terms_and_condition_html'] = array(
      '#type' => 'textarea',
      '#title' => t('Enter text to be displayed under the Terms and Condition on the registration page.'),
      '#rows' => 2,
      '#default_value' => variable_get('lr_ciam_terms_and_condition_html'),
      '#attributes' => array('placeholder' => t('terms and conditon text'))
    );
   
    $form[$user_settings_group]['lr_ciam_custom_options'] = array(
      '#type' => 'textarea',
      '#title' => t('Please enter custom ciam options for LoginRadius interface.'),
      '#id' => 'add_custom_options',
      '#rows' => 4,
      '#default_value' => variable_get('lr_ciam_custom_options'),
      '#attributes' => array(
        'placeholder' => t('ciam custom options'),
        'onchange' => "lrCheckValidJson();",
      ),
      '#description' => t('Insert custom option like commonOptions.loginOnEmailVerification = true;'),
    );
    
     $form[$user_settings_group]['lr_ciam_prompt_password_on_social_login'] = array(
      '#type' => 'radios',
      '#id' => 'prompt_password',
      '#title' => t('Do you want to prompt for password after registration with social provider<a title="This feature when enabled, will prompt the user to set the password at the time of login for the time from any social provider."  class="lrTooltip"> (<span class="lrTooltipColor">?</span>)</a>'),
      '#default_value' => variable_get('lr_ciam_prompt_password_on_social_login', 'false'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      )
    );
     $form[$user_settings_group]['lr_ciam_enable_user_name'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable login with username<a title="This feature when enabled, will let the user to login with username as well as password."  class="lrTooltip"> (<span class="lrTooltipColor">?</span>)</a>'),
      '#default_value' => variable_get('lr_ciam_enable_user_name', 'false'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      )
    );
    $form[$user_settings_group]['lr_ciam_ask_email_always_for_unverified'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to ask for email every time an unverified user tries to log in<a title="This feature when enabled, will ask for email every time user tries to login if email is not verified."  class="lrTooltip"> (<span class="lrTooltipColor">?</span>)</a>'),
      '#default_value' => variable_get('lr_ciam_ask_email_always_for_unverified', 'false'),
      '#options' => array(
        'true' => t('Yes, (ask for email address every time an unverified user logs in)'),
        'false' => t('No'),
      )
    );
     
    $form[$user_settings_group]['lr_ciam_display_password_strength'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable to check password strength<a title="To enable check password strength feature" class="lrTooltip"> (<span class="lrTooltipColor">?</span>)</a> '),
      '#default_value' => variable_get('lr_ciam_display_password_strength', 'false'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      )
    );
    $form[$user_settings_group]['lr_ciam_ask_required_field_on_traditional_login'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable ask required field on traditional login<a title="Enabling this property would prompt an interface of required fields for a traditional legacy or old user account, if the registration schema has changed"  class="lrTooltip"> (<span class="lrTooltipColor">?</span>)</a> '),
      '#default_value' => variable_get('lr_ciam_ask_required_field_on_traditional_login', 'false'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      )
    ); 
      $form[$user_settings_group]['lr_ciam_instant_link_login'] = array(
      '#type' => 'radios',            
      '#title' => t('Do you want to initiate instant login with email<a class="lrTooltip" title="Using this you can also log into​ the account if you forgot your password without using the password reset interface"> (<span class="lrTooltipColor">?</span>)</a>'),
      '#default_value' => variable_get('lr_ciam_instant_link_login', 'false'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      ),
           '#attributes' => array(
        'onchange' => "showAndHideinstantemailOptions();",
      )
    ); 
   
    if(isset($cloudObject) && $cloudObject->IsPhoneLogin)
    {
      $form[$user_settings_group]['lr_ciam_exist_phone_number'] = array(
      '#type' => 'radios',    
      '#title' => t('Do you want to enable option to check phone number exist or not<a class="lrTooltip" title="Turn on, if you want to enable Phone Exist functionality"> (<span class="lrTooltipColor">?</span>)</a>'),
      '#default_value' => variable_get('lr_ciam_exist_phone_number', 'false'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      )
    ); 
      $form[$user_settings_group]['lr_ciam_instant_otp_login'] = array(
      '#type' => 'radios',    
      '#title' => t('Do you want to initiate one click OTP login<a title="To initiate one click OTP login when phone number login enabled at your site" class="lrTooltip"> (<span class="lrTooltipColor">?</span>)</a>'),
      '#default_value' => variable_get('lr_ciam_instant_otp_login', 'false'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      ),
          '#attributes' => array(
        'onchange' => "showAndHideinstantPhoneOptions();",
      )
    );
    }
    
    $form[$user_settings_group]['lr_ciam_minify_js_css'] = array(
      '#type' => 'radios',
      '#title' => t('Enable minified version of JS/CSS file?<a title="This feature enables minified version of js/css file." class="lrTooltip"> (<span class="lrTooltipColor">?</span>)</a> '),
      '#default_value' => variable_get('lr_ciam_minify_js_css', 'false'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      )
    );
    
    $form[$user_settings_group]['lr_auto_hide_messages'] = array(
      '#type' => 'textfield',
        '#element_validate' => array('lr_auto_hide_messages_validate'),
        '#attributes' => array(
        ' type' => 'number',
    ),
      '#title' => t('Auto hide success message<a title="This will hide success and error message in given seconds"  class="lrTooltip"> (<span class="lrTooltipColor">?</span>)</a> '),
      '#default_value' => variable_get('lr_auto_hide_messages')
    );
    
    
    
    $login_settings_group = 'login_settings';
    if(isset($email_templates->EmailTemplates))
    {
    $form[$login_settings_group] = array(
      '#type' => 'fieldset',
      '#title' => t('CIAM Template Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $form[$login_settings_group]['lr_ciam_welcome_email_template'] = array(
      '#type' => 'select',
      '#title' => t('Choose welcome email template name'),
      '#options' => lr_ciam_select_template($email_templates->EmailTemplates->Welcome),
      '#default_value' => variable_get('lr_ciam_welcome_email_template')
    );
    $form[$login_settings_group]['lr_ciam_email_verification_template'] = array(
      '#type' => 'select',
      '#title' => t('Choose email verification template name'),
      '#options' => lr_ciam_select_template($email_templates->EmailTemplates->Verification),
      '#default_value' => variable_get('lr_ciam_email_verification_template')
    );
    $form[$login_settings_group]['lr_ciam_forgot_password_template'] = array(
      '#type' => 'select',
      '#title' => t('Choose forgot password email template name'),
      '#options' => lr_ciam_select_template($email_templates->EmailTemplates->ResetPassword),
      '#default_value' => variable_get('lr_ciam_forgot_password_template')
    );
    $form[$login_settings_group]['lr_ciam_instant_link_login_email_template'] = array(
      '#type' => 'select',
      '#title' => t('Choose instant link login email template name'),
      '#options' => lr_ciam_select_template($email_templates->EmailTemplates->InstantSignIn),
      '#default_value' => variable_get('lr_ciam_instant_link_login_email_template')
    );
    }
    if(isset($email_templates->SMSTemplates))
    {
    if(isset($cloudObject) && $cloudObject->IsPhoneLogin)
    {
     $form[$login_settings_group]['lr_ciam_sms_template'] = array(
      '#type' => 'select',
      '#title' => t('Choose welcome SMS template name'),
      '#options' => lr_ciam_select_template($email_templates->SMSTemplates->Welcome),
      '#default_value' => variable_get('lr_ciam_sms_template')
    );
     $form[$login_settings_group]['lr_ciam_sms_template_phone_verification'] = array(
      '#type' => 'select',
      '#title' => t('Choose SMS template name for phone number verification'),
      '#options' => lr_ciam_select_template($email_templates->SMSTemplates->Verification),
      '#default_value' => variable_get('lr_ciam_sms_template_phone_verification')
    );
     $form[$login_settings_group]['lr_ciam_sms_template_password_reset'] = array(
      '#type' => 'select',
      '#title' => t('Choose SMS template name for Password Reset'),
      '#options' => lr_ciam_select_template($email_templates->SMSTemplates->ResetPassword),
      '#default_value' => variable_get('lr_ciam_sms_template_password_reset')
    );
     $form[$login_settings_group]['lr_ciam_sms_template_change_number'] = array(
      '#type' => 'select',
      '#title' => t('Choose SMS template name for Changing Phone Number'),
      '#options' => lr_ciam_select_template($email_templates->SMSTemplates->ChangePhoneNo),
      '#default_value' => variable_get('lr_ciam_sms_template_change_number')
    );
    }
     $form[$login_settings_group]['lr_ciam_sms_template_one_time_passcode'] = array(
      '#type' => 'select',
      '#title' => t('Choose instant OTP Login SMS template name'),
      '#options' => lr_ciam_select_template($email_templates->SMSTemplates->OneTimePassCode),
      '#default_value' => variable_get('lr_ciam_sms_template_one_time_passcode')
    );
    if(isset($cloudObject) && $cloudObject->TwoFactorAuthentication->IsEnabled)
    {
    $form[$login_settings_group]['lr_ciam_sms_template_2fa'] = array(
      '#type' => 'select',
      '#title' => t('Choose SMS template name for Two-factor Authentication'),
      '#options' => lr_ciam_select_template($email_templates->SMSTemplates->SecondFactorAuthentication),
      '#default_value' => variable_get('lr_ciam_sms_template_2fa')
    ); 
    }
    }
    
    module_load_include('inc', 'lr_ciam', 'includes/lr_field_mapping');
    //field mapping section
    $field_map_group = 'fieldmap';
    $form[$field_map_group] = array(
      '#type' => 'fieldset',
      '#title' => t('CIAM field mapping'),
      '#description' => t('Each of your <a href="!url" target="_blank">fields that are attached to users</a> are listed below. Map social provider data to field by choosing from select list and configure your user fields from <a href="!url"> FIELD MAPPING </a>section.<br/><br/> For a list of all fields according to the providers:<a href="!data_url" target="_blank"> https://www.loginradius.com/datapoints/ </a>', array(
        '!url' => url('admin/config/people/accounts/fields'),
        '!data_url' => url('http://ish.re/1F4AD')
      )),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$field_map_group]['lr_ciam_user_fields'] = array(
      '#title' => 'user fields',
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#weight' => 5,
      '#collapsible' => TRUE,
    );
    $properties = lr_field_mapping_ciam_user_properties();
    $property_options = array();

    foreach ($properties as $property => $property_info) {
        if (isset($property_info['field_types'])) {
            foreach ($property_info['field_types'] as $field_type) {
                $property_options[$field_type][$property] = $property_info['label'];
            }
        }
    }

    $field_defaults = variable_get('lr_ciam_user_fields', array());
    $instances = field_info_instances('user', 'user');

    foreach ($instances as $field_name => $instance) {
        $field = field_info_field($instance['field_name']);

        if (isset($property_options[$field['type']])) {
            $options = array_merge(array('' => t('- Do not import -')), $property_options[$field['type']]);
            $form[$field_map_group]['lr_ciam_user_fields'][$field_name] = array(
              '#title' => check_plain(t($instance['label'])),
              '#type' => 'select',
              '#options' => $options,
              '#default_value' => isset($field_defaults[$field_name]) ? $field_defaults[$field_name] : '',
            );
        }
        else {
            $form[$field_map_group]['lr_ciam_user_fields'][$field_name] = array(
              '#title' => check_plain(t($instance['label'])),
              '#type' => 'form_element',
              '#children' => '<em>' . t('Not any mappable properties.') . '</em>',
              '#theme_wrappers' => array('form_element'),
            );
        }
    }

    if (module_exists('profile2')) {
        $form[$field_map_group]['lr_ciam_profile_fields'] = array(
          '#title' => 'Profile2 fields',
          '#type' => 'fieldset',
          '#tree' => TRUE,
          '#weight' => 5,
          '#collapsible' => TRUE,
        );

        $properties = lr_field_mapping_ciam_user_properties();
        $property_options = array();

        foreach ($properties as $property => $property_info) {
            if (isset($property_info['field_types'])) {
                foreach ($property_info['field_types'] as $field_type) {
                    $property_options[$field_type][$property] = $property_info['label'];
                }
            }
        }

        $field_defaults = variable_get('lr_ciam_profile_fields', array());

        foreach (field_info_instances('profile2') as $field_name => $instances) {
            $form[$field_map_group]['lr_ciam_profile_fields'][$field_name] = array(
              '#type' => 'fieldset',
              '#title' => check_plain(t($field_name)),
              '#tree' => TRUE,
              '#weight' => 5,
              '#collapsible' => TRUE,
            );
            foreach ($instances as $instance => $array) {
                $field = field_info_field($array['field_name']);
                if (isset($property_options[$field['type']])) {
                    $options = array_merge(array('' => t('- Do not import -')), $property_options[$field['type']]);
                    $form[$field_map_group]['lr_ciam_profile_fields'][$field_name][$array['field_name']] = array(
                      '#title' => check_plain(t($array['label'])),
                      '#type' => 'select',
                      '#options' => $options,
                      '#default_value' => isset($field_defaults[$field_name][$array['field_name']]) ? $field_defaults[$field_name][$array['field_name']] : '',
                    );
                }
                else {
                    $form[$field_map_group]['lr_ciam_profile_fields'][$field_name][$array['field_name']] = array(
                      '#title' => check_plain(t($array['label'])),
                      '#type' => 'form_element',
                      '#children' => '<em>' . t('Not any mappable properties.') . '</em>',
                      '#theme_wrappers' => array('form_element'),
                    );
                }
            }
        }
    }

    //debug mode section
    $debug_group = 'debug';
    $form[$debug_group] = array(
      '#type' => 'fieldset',
      '#title' => t('Debug'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$debug_group]['lr_ciam_debug_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable Debugging mode<a title="Choosing yes will add debug log in database" class="lrTooltip"> (<span class="lrTooltipColor">?</span>)</a>'),
      '#default_value' => variable_get('lr_ciam_debug_mode', 'false'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      )
    );
    

    $form['#submit'][] = 'lr_ciam_admin_settings_submit';
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
    return $form;
}

function lr_auto_hide_messages_validate($form,$form_state)
{
    if($form['#value'] < 0)
    {
        form_error($form, t('Auto hide success message time should be positive'));
    }
}

/**
 * Submit Social Login tab settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 */
function lr_ciam_admin_settings_submit($form, &$form_state) {
    if (!lr_ciam_save_settings($form_state)) {
        return FALSE;
    }
    $settings = array(
      'lr_ciam_attach_login_form' => '1',
      'lr_ciam_loginpage_position' => '1',
      'lr_ciam_registerpage_position' => '1',
      'lr_ciam_force_registration' => '1',
      'lr_ciam_send_user_credentials' => '1',
      'lr_ciam_update_profile' => '0',
    );

    foreach ($settings as $key => $values) {
        if (!variable_get($key, FALSE) && variable_get($key, '') != '0') {
            variable_set($key, $values);
        }
    }
}

/**
 * Save settings to database.
 *
 * @param array $form_state A keyed array containing the current state of the form
 */
function lr_ciam_save_settings(&$form_state) {
    
    
   
    $api_key = isset($form_state['values']['lr_ciam_apikey']) ? trim($form_state['values']['lr_ciam_apikey']) : trim(variable_get('lr_ciam_apikey', ''));
    $api_secret = isset($form_state['values']['lr_ciam_apisecret']) ? trim($form_state['values']['lr_ciam_apisecret']) : trim(variable_get('lr_ciam_apisecret', ''));
    $data = lr_ciam_get_authentication($api_key, $api_secret);
    if (isset($data['status']) && $data['status'] != 'status') {
        drupal_set_message($data['message'], $data['status']);
        return FALSE;
    }
    form_state_values_clean($form_state);
    if(!variable_get('lr_ciam_apikey', FALSE) || !variable_get('lr_ciam_apisecret', FALSE) || variable_get('lr_ciam_apikey' , '') != $api_key || variable_get('lr_ciam_apisecret' , '') != $api_secret)
    {
        $cloudObject = new CloudAPI($api_key, $api_secret, array('output_format' => 'json'));
        $config = (array)$cloudObject->getConfigurationList();
                    if(isset($config['AppName']))
                    {
                    variable_set('lr_ciam_site_name', $config['AppName']);
                    }
                    if(isset($config['AskPasswordOnSocialLogin']) && !variable_get('lr_ciam_prompt_password_on_social_login',FALSE))
                    {
                         if($config['AskPasswordOnSocialLogin'] == 1)
                        {
                         variable_set('lr_ciam_prompt_password_on_social_login', 'true');
                        }
                        else
                        {
                            variable_set('lr_ciam_prompt_password_on_social_login', 'false');
                        }
                    }
                    if(isset($config['IsUserNameLogin']) && !variable_get('lr_ciam_enable_user_name',FALSE))
                    {
                         if($config['IsUserNameLogin'] == 1)
                        {
                         variable_set('lr_ciam_enable_user_name', 'true');
                        }
                        else
                        {
                            variable_set('lr_ciam_enable_user_name', 'false');
                        }
                    }
                    if(isset($config['AskEmailIdForUnverifiedUserLogin']) && !variable_get('lr_ciam_ask_email_always_for_unverified',FALSE))
                    {
                         if($config['AskEmailIdForUnverifiedUserLogin'] == 1)
                        {
                         variable_set('lr_ciam_ask_email_always_for_unverified', 'true');
                        }
                        else
                        {
                            variable_set('lr_ciam_ask_email_always_for_unverified', 'false');
                        }
                    }
                    if(isset($config['AskRequiredFieldsOnTraditionalLogin']) && !variable_get('lr_ciam_ask_required_field_on_traditional_login',FALSE))
                    {
                         if($config['AskRequiredFieldsOnTraditionalLogin'] == 1)
                        {
                         variable_set('lr_ciam_ask_required_field_on_traditional_login', 'true');
                        }
                        else
                        {
                            variable_set('lr_ciam_ask_required_field_on_traditional_login', 'false');
                        }
                    }
                    if(isset($config['CheckPhoneNoAvailabilityOnRegistration']) && !variable_get('lr_ciam_ask_required_field_on_traditional_login',FALSE))
                    {
                         if($config['CheckPhoneNoAvailabilityOnRegistration'] == 1)
                        {
                         variable_set('lr_ciam_exist_phone_number', 'true');
                        }
                        else
                        {
                            variable_set('lr_ciam_exist_phone_number', 'false');
                        }
                    }
    }

    foreach ($form_state['values'] as $key => $value) {
        if (is_array($value) && isset($form_state['values']['array_filter'])) {
            $value = array_keys(array_filter($value));
        }
        variable_set($key, $value);
    }
    
    

    drupal_set_message(t('The configuration options have been saved.'));
    // Clear the cached pages and blocks.
    cache_clear_all();
    menu_rebuild();
    return TRUE;
}

/**
 * Function validates against GUID format of keys
 *
 * @param string $key LoginRadius API key/secret
 * @return boolean If valid - true, else - false
 */
function lr_ciam_isvalid_key($key) {
    return preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $key);
}

/**
 * Check API credentials and API connection methos is working or not
 *
 * @param string $apikey LoginRadius API Key
 * @param string $apisecret LoginRadius API Secret
 * @param string $api_handler API connection method curl/drupal_http_request
 * @return array Contain message and status of data
 */
function lr_ciam_get_authentication($apikey, $apisecret) {
    if (isset($apikey)) {
        $data = array();
        try {
            $socialLoginObject = new SocialLoginAPI($apikey, $apisecret, array('output_format' => 'json'));
            $result = $socialLoginObject->validateKeyandSecret();

            if (empty($result)) {
                $data['message'] = check_plain('please check your php.ini settings to enable CURL or FSOCKOPEN');
                $data['status'] = 'error';
            }
            elseif (isset($result->Status) && !$result->Status) {
                $error = array(
                  "API_KEY_NOT_VALID" => "LoginRadius API key is invalid. Get your LoginRadius API key from <a href='http://www.loginradius.com' target='_blank'>LoginRadius</a>",
                  "API_SECRET_NOT_VALID" => "LoginRadius API Secret is invalid. Get your LoginRadius API Secret from <a href='http://www.loginradius.com' target='_blank'>LoginRadius</a>",
                  "API_KEY_NOT_FORMATED" => "LoginRadius API Key is not formatted correctly",
                  "API_SECRET_NOT_FORMATED" => "LoginRadius API Secret is not formatted correctly"
                );

                foreach ($result->Messages as $value) {
                    $data['message'] = $error["$value"];
                    $data['status'] = 'error';
                    break;
                }
            }
            return $data;
        }
        catch (Exception $e) {
            $data['message'] = 'Something went wrong, check your credentials';
            $data['status'] = 'error';
            return $data;          
        }
    }
}
