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

     global $base_url;
    $form['header_top_style_1'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

    $form['markup_top'] = array(
         '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
    );

    $form['markup_top_vt_start'] = array(
         '#markup' => '<b><span style="font-size: 17px;">Sign in Settings</span></b><br><br><hr><br/>'
     );
    $form['miniorange_oauth_force_auth'] = array(
        '#type' => 'checkbox',
        '#title' => t('Protect website against anonymous access <a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Premium, Enterprise]</b></a>'),
        '#default_value' => variable_get('miniorange_oauth_force_auth', ''),
        '#disabled' => TRUE,
        '#description' => t('<b>Note: </b>Users will be redirected to your IdP for login in case user is not logged in and tries to access website.<br><br>'),
    );
    $form['miniorange_oauth_auto_redirect'] = array(
        '#type' => 'checkbox',
        '#title' => t('Check this option if you want to <b> Auto-redirect to OAuth Provider/Server </b><a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Premium, Enterprise]</b></a>'),
        '#default_value' => variable_get('miniorange_oauth_auto_redirect_to_idp'),
        '#disabled' => TRUE,
        '#description' => t('<b>Note: </b>Users will be redirected to your IdP for login when the login page is accessed.<br><br>'),
    );
    $form['miniorange_oauth_enable_backdoor'] = array(
        '#type' => 'checkbox',
        '#title' => t('Check this option if you want to enable <b>backdoor login </b><a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Premium, Enterprise]</b></a>'),
        '#default_value' => variable_get('miniorange_oauth_enable_backdoor', ''),
        '#disabled' => TRUE,
        '#description' => t('Note: Checking this option creates a backdoor to login to your Website using Drupal credentials<br> incase you get locked out of your IdP.
                <b>Note down this URL: </b>Available in <a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>Premium, Enterprise</b></a> versions of the module.<br><br><br><br>'),
    );
    $form['miniorange_oauth_client_siginin'] = array(
            '#type' => 'submit',
            '#id' => 'button_config_center',
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
 	drupal_set_message(t('Signin Settings successfully saved'));
 }