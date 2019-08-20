<?php
//@author: miniOrange
include_once('includes/miniorange_support.php');
/**
 * @file
 * Contains support form for miniOrange OAuth Client Login Module.
 */


 /**
 * Showing Support form info.
 */
 function miniorange_oauth_client_support($form, &$form_state) {
     drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
     drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

     $form['mo_header_style'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');
      $form['mo_top_div'] = array(
         '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
      );
      $form['markup_1'] = array(
        '#markup' => '<h3>Support</h3><div>Need any help? Just send us a query so we can help you.<br /><br /></div>',
      );
      $form['miniorange_oauth_client_email_address'] = array(
        '#type' => 'textfield',
        '#id' =>'text_field',
        '#title' => t('Email Address'),
        '#attributes' => array('style' => 'width:73%;','placeholder' => 'Enter your email'),
        '#required' => TRUE
      );
      $form['miniorange_oauth_client_phone_number'] = array(
        '#type' => 'textfield',
        '#id' =>'text_field',
        '#title' => t('Phone number'),
        '#attributes' => array('style' => 'width:73%;','placeholder' => 'Enter your phone number'),
      );
      $form['miniorange_oauth_client_support_query'] = array(
        '#type' => 'textarea',
        '#id' =>'text_field',
        '#title' => t('Query'),
        '#cols' => '10',
        '#rows' => '5',
        '#attributes' => array('style' => 'width:73%;','placeholder' => 'Write your query here'),
        '#required' => TRUE,
        '#resizable' => FALSE,
      );
      $form['miniorange_oauth_client_support_submit'] = array(
        '#type' => 'submit',
        '#id' => 'button_config_center',
        '#value' => t('Submit Query'),
        '#submit' => array('miniorange_oauth_client_send_query'),
      );
      return $form;
 }

 /**
  * Sends support query.
  */
 function miniorange_oauth_client_send_query($form, &$form_state) {
    $email = $form['miniorange_oauth_client_email_address']['#value'];
    $phone = $form['miniorange_oauth_client_phone_number']['#value'];
    $query = $form['miniorange_oauth_client_support_query']['#value'];
    $support = new MiniorangeOAuthClientSupport($email, $phone, $query);
    $support_response = $support->sendSupportQuery();
    if($support_response) {
      drupal_set_message(t('Support query successfully sent'));
    }
    else {
    	drupal_set_message(t('Error sending support query'), 'error');
    }
 }