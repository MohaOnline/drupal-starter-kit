<?php
/**
 * @file
 * Set up Attribute Mapping for miniOrange SAML IDP Module.
 */

 /**
 * Showing Attribute Mappping form info.
 */
include "mo_saml_visualTour.php";
function miniorange_saml_idp_attribute_mapping($form, &$form_state) {
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));


  global $base_url;
  
  $mo_admin_email = variable_get('miniorange_saml_idp_customer_admin_email',NULL);

    global $base_url;

     $form['header_top_style_1'] = array('#markup' => '<div class="mo_saml_table_layout_1">',);

    $form['markup_idp_attr_header'] = array(
        '#markup' => '<div class="mo_saml_table_layout mo_saml_container"><b><span style="font-size: 17px;">ATTRIBUTE MAPPING (OPTIONAL)</span></b><a class="btn btn-primary btn-large restart_button" id="restart_tour_button" onclick="show_visual_tour_form()">'
            . 'Take a Tour</a><br><br><hr><br/>',
    );

  $form['miniorange_saml_idp_nameid_attr_map'] = array(
    '#type' => 'select',
	'#title' => t('NameID Attribute:'),
	'#options' => array(
	  'emailAddress' => t('Drupal Email Address'),
	  'username' => t('Drupal Username'),
	),
      '#attributes' => array('style' => 'width:80%'),
	'#default_value' => variable_get('miniorange_saml_idp_nameid_attr_map', ''),
  );

     $form['markup_idp_nameid_attr_map_note'] = array(
         '#markup' => '<div style="background-color: #e0e0e0;border-radius: 4px;padding: 0.55em;"><b>'
             . '<span style="color:red"><b>Note:</b></span> This attribute value is sent in SAML Response. 
                Users in your Service Provider will be searched (existing users) or created (new users) based on this attribute. 
                Use <b><i><u>EmailAddress</u></i></b> by default.</b></div><br/>',
     );


     $form['markup_idp_attr_header2'] = array(
         '#markup' => '<br><div id="mo_saml_idp_vt_attrmapp"><h4>ATTRIBUTE STATEMENT (OPTIONAL)<a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/licensing"> [Premium]</a></h4><hr><br>',
     );

     $form['miniorange_saml_idp_attr1_name'] = array(
         '#type' => 'textfield',
         '#title' => t('Attribute Name 1 '),
         '#default_value' => variable_get('miniorange_saml_idp_attr1_name', ''),
         '#disabled' => TRUE,
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Attribute Name'),
         '#required' => FALSE,
     );

     $form['miniorange_saml_idp_attr1_value'] = array(
         '#type' => 'select',
         '#title' => t('Attribute Value'),
         '#options' => array(
             '' => t('Select Attribute Value'),
         ),
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;'),
         '#disabled' => TRUE,
     );

     $form['miniorange_saml_idp_attr2_name'] = array(
         '#type' => 'textfield',
         '#title' => t('Attribute Name 2'),
         '#default_value' => variable_get('miniorange_saml_idp_attr2_name', ''),
         '#disabled' => TRUE,
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Attribute Name'),
         '#required' => FALSE,
     );

     $form['miniorange_saml_idp_attr2_value'] = array(
         '#type' => 'select',
         '#title' => t('Attribute Value'),
         '#options' => array(
             '' => t('Select Attribute Value'),
         ),
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;'),
         '#disabled' => TRUE,
     );

     $form['miniorange_saml_idp_attr3_name'] = array(
         '#type' => 'textfield',
         '#title' => t('Attribute Name 3'),
         '#default_value' => variable_get('miniorange_saml_idp_attr3_name', ''),
         '#disabled' => TRUE,
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Attribute Name'),
         '#required' => FALSE,
     );

     $form['miniorange_saml_idp_attr3_value'] = array(
         '#type' => 'select',
         '#title' => t('Attribute Value'),
         '#options' => array(
             '' => t('Select Attribute Value'),
         ),
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;'),
         '#disabled' => TRUE,
     );

     $form['miniorange_saml_idp_attr4_name'] = array(
         '#type' => 'textfield',
         '#title' => t('Attribute Name 4'),
         '#default_value' => variable_get('miniorange_saml_idp_attr4_name', ''),
         '#disabled' => TRUE,
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Attribute Name'),
         '#required' => FALSE,
     );

     $form['miniorange_saml_idp_attr4_value'] = array(
         '#type' => 'select',
         '#title' => t('Attribute Value'),
         '#options' => array(
             '' => t('Select Attribute Value'),
         ),
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;'),
         '#disabled' => TRUE,
     );

     $form['markup_idp_attr_head_2'] = array(
         '#markup' => '</div>',
    );

     $form['markup_idp_user_attr_header'] = array(
         '#markup' => '<div id="mo_saml_vt_add_attr"><br/><h4>ADDITIONAL USER ATTRIBUTES (OPTIONAL)
                        <input type="button" class="btn btn-primary" style="width:3%;margin-left:2%;margin-right:2%;" value="+" onclick="add_user_attibute();" /> 
                        <input type="button" class="btn btn-danger" style="width:3%;" value="-" onclick="remove_user_attibute();" /></b><a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/licensing"> &nbsp;&nbsp;[Premium]</a></h4>',
         '#disabled' => TRUE,
         );

     $form['markup_idp_user_attr_note'] = array(
         '#markup' => '<div class="messages status">User Profile Attribute Name: It is the name which you want to send to your SP. It should be unique.
                        <br />User Profile Attribute Value: It is the user attribute (machine name) whose value you want to send to SP.</div></div><br/><br/>',
     );

  $form['miniorange_saml_idp_attr_map_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#submit' => array('miniorange_saml_idp_save_attribute_mapping'),
      '#attributes' => array('style' => 'border-radius:4px;background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;display:block;margin-left:auto;margin-right:auto;'),
  );

  $form['markup_idp_attr_header_top_div_close'] = array('#markup' => '</div>',);

    $form['miniorange_idp_guide_link'] = array(
        '#markup' => '<div class="mo_saml_table_layout mo_saml_container_2">',
    );

    $form['miniorange_idp_guide_link1'] = array(
        '#markup' => '<div style="font-size: 15px;"><b>To see detailed documentation of how to configure Drupal 7 SAML IdP with any Service Provider</b></div></br>',
    );

    Utilities::spConfigGuide($form, $form_state);


    $form['miniorange_idp_guide_link_end'] = array(
        '#markup' => '</div>',
    );

  Utilities::AddSupportButton($form, $form_state);

  return $form;

 }
 
 /**
 * Configure IdP.
 */
 function miniorange_saml_idp_save_attribute_mapping($form, &$form_state)
 {
      global $base_url;
      $nameid_attr = $form['miniorange_saml_idp_nameid_attr_map']['#value'];
      if($nameid_attr == ''){
        $nameid_attr = 'emailAddress';
      }
      variable_set('miniorange_saml_idp_nameid_attr_map', $nameid_attr);
      drupal_set_message(t('Your settings are saved successfully.'));
 }

function send_support_query(&$form, $form_state)
{
    $email = $form['miniorange_saml_email_address_support']['#value'];
    $phone = $form['miniorange_saml_phone_number_support']['#value'];
    $query = $form['miniorange_saml_support_query_support']['#value'];
    Utilities::send_query($email, $phone, $query);
}