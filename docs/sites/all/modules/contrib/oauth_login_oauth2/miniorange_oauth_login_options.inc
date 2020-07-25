<?php
/**
 * @file
 * Contains Login Settings for miniOrange OAuth Login Module.
 */
 /**
 * Showing Settings form.
 */
 function miniorange_oauth_login_options($form, &$form_state) {

     drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
     drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
     drupal_add_js(drupal_get_path('module', 'oauth_login_oauth2') . '/js/slide_support_button.js');

     global $base_url;
     $baseUrlValue = variable_get('miniorange_oauth_client_base_url');
    $form['header_top_style_1'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

    $form['markup_top'] = array(
         '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
    );

    $form['markup_top_vt_start'] = array(
         '#markup' => '<b><span style="font-size: 17px;">Sign in Settings</span></b><br><hr><br/>'
     );
     $form['miniorange_oauth_client_base_url'] = array(
         '#type' => 'textfield',
         '#title' => t('Base URL: '),
         '#default_value' => $baseUrlValue,
         '#attributes' => array('id'=>'mo_oauth_vt_baseurl','style' => 'width:73%;','placeholder' => 'Enter Base URL'),
         '#description' => '<b>Note: </b>You can customize base URL here. (For eg: https://www.xyz.com or http://localhost/abc)',
         '#suffix' => '<br>',
     );

     $form['miniorange_oauth_client_siginin_update_base'] = array(
        '#type' => 'submit',
        '#id' => 'button_config_center',
        '#value' => t('Update'),
        '#submit' => array('miniorange_oauth_save_signin_settings'),
        '#suffix' => '<br>',
    );

     $form['markup_bottom_vt_start4'] = array(
         '#markup' => '<b><span style="font-size: 17px;">Advanced Sign in Settings</span></b><br><hr><br/>'
    );

    $form['miniorange_oauth_force_auth'] = array(
        '#type' => 'checkbox',
        '#title' => t('Protect website against anonymous access <a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Premium, Enterprise]</b></a>'),
        '#disabled' => TRUE,
        '#description' => t('<b>Note: </b>Users will be redirected to your OAuth server for login in case user is not logged in and tries to access website.<br><br>'),
    );
    $form['miniorange_oauth_auto_redirect'] = array(
        '#type' => 'checkbox',
        '#title' => t('Check this option if you want to <b> Auto-redirect to OAuth Provider/Server </b><a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Premium, Enterprise]</b></a>'),
        '#disabled' => TRUE,
        '#description' => t('<b>Note: </b>Users will be redirected to your OAuth server for login when the login page is accessed.<br><br>'),
    );
    $form['miniorange_oauth_enable_backdoor'] = array(
        '#type' => 'checkbox',
        '#title' => t('Check this option if you want to enable <b>backdoor login </b><a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Premium, Enterprise]</b></a>'),
        '#disabled' => TRUE,
        '#description' => t('<b>Note</b>: Checking this option creates a backdoor to login to your Website using Drupal credentials<br> incase you get locked out of your OAuth server.
                <br><b>Note down this URL: </b>Available in <a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>Premium, Enterprise</b></a> versions of the module.<br><br><br><br>'),
    );

    $form['markup_bottom_vt_start5'] = array(
         '#markup' => '<b><span style="font-size: 17px;">Login Button Customization</span></b> <a href="' . $base_url . '/admin/config/people/miniorange_oauth_client/licensing"><b>[Standard, Premium, Enterprise]</b></a><br><hr><br/>'
    );

    $form['miniorange_oauth_client_icon_width'] = array(
        '#title' => t('<b>Icon Width</b>'),
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width:73%','placeholder' => 'For eg. 200px or 10%'),
        '#disabled' => TRUE,
    );

    $form['miniorange_oauth_client_icon_height'] = array(
        '#title' => t('<b>Icon Height</b>'),
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width:73%','placeholder' => 'For eg. 60px or auto'),
        '#disabled' => TRUE,
    );

    $form['miniorange_oauth_client_icon_margins'] = array(
        '#title' => t('<b>Icon Margins</b>'),
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width:73%','placeholder' => 'For eg. 2px 3px or auto'),
        '#disabled' => TRUE,
    );

    $form['miniorange_oauth_client_custom_btntxt'] = array(
        '#title' => t('<b>Custom Button Text</b>'),
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width:73%','placeholder' => 'Login using appname'),
        '#disabled' => TRUE,
     );

    $form['miniorange_oauth_custom_css'] = array(
            '#type' => 'textarea',
            '#title' => t('Custom CSS'),
	        '#disabled' => TRUE,
            '#resizable' => FALSE,
            '#attributes' => array('style' => 'width:73%','placeholder' => 'For eg.  .oauthloginbutton{ background: #7272dc; height:40px; padding:8px; text-align:center; color:#fff; }'),
    );

     $form['markup_bottom_vt_start'] = array(
         '#markup' => '<br><b><span style="font-size: 17px;">Domain & Page Restriction</span></b><br><hr><br/>'
     );

     $form['miniorange_oauth_client_white_list_url'] = array(
         '#type' => 'textfield',
         '#title' => t('Allowed Domains <a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Enterprise]</b></a>'),
         '#attributes' => array('style' => 'width:73%','placeholder' => 'Enter semicolon(;) separated domains (Eg. xxxx.com; xxxx.com)'),
         '#description' => t('<b>Note: </b> Enter <b>semicolon(;) separated</b> domains to allow SSO. Other than these domains will not be allowed to do SSO.'),
         '#disabled' => TRUE,
     );

     $form['miniorange_oauth_client_black_list_url'] = array(
         '#type' => 'textfield',
         '#title' => t('Restricted Domains <a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Enterprise]</b></a>'),
         '#attributes' => array('style' => 'width:73%','placeholder' => 'Enter semicolon(;) separated domains (Eg. xxxx.com; xxxx.com)'),
         '#description' => t('<b>Note: </b> Enter <b>semicolon(;) separated</b> domains to restrict SSO. Other than these domains will be allowed to do SSO.'),
         '#disabled' => TRUE,
     );

     $form['miniorange_oauth_client_page_restrict_url'] = array(
         '#type' => 'textfield',
         '#title' => t('Page Restriction <a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Enterprise]</b></a>'),
         '#attributes' => array('style' => 'width:73%','placeholder' => 'Enter semicolon(;) separated page URLs (Eg. xxxx.com/yyy; xxxx.com/yyy)'),
         '#description' => t('<b>Note: </b> Enter <b>semicolon(;) separated</b> URLs to restrict unauthorized access.'),
         '#disabled' => TRUE,
     );


     $form['miniorange_oauth_client_siginin'] = array(
            '#type' => 'button',
            '#value' => t('Save Configuration'),
            '#disabled' => TRUE,
        );
    $form['mo_header_style_end'] = array('#markup' => '</div>');
     Utilities::spConfigGuide($form, $form_state);
     $form['mo_markup_div_imp']=array('#markup'=>'</div>');
     Utilities::AddSupportButton($form, $form_state);
     return $form;
 }

 /**
  * This function saves the values in the Sign in Settings tab.
  */
 function miniorange_oauth_save_signin_settings($form, &$form_state) {
     $baseUrlvalue = trim($form['miniorange_oauth_client_base_url']['#value']);
     if(!empty($baseUrlvalue) && filter_var($baseUrlvalue, FILTER_VALIDATE_URL) == FALSE) {
         drupal_set_message(t('Please enter a valid URL'),'error');
         return;
     }
     variable_set('miniorange_oauth_client_base_url', $baseUrlvalue);
 	drupal_set_message(t('Signin Settings successfully saved'));
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