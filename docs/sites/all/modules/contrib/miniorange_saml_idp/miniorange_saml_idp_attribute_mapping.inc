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

    $form['markup_idp_attr_header'] = array(
        '#markup' => '<div class="mo_saml_table_layout_1">
                        <div class="mo_saml_table_layout mo_saml_container"><b><span style="font-size: 17px;">ATTRIBUTE MAPPING (OPTIONAL)</span></b>
                        <a class="mo_saml_btn mo_saml_btn-primary mo_saml_btn-large mo_saml_restart_button" id="mo_saml_restart_tour_button" onclick="show_visual_tour_form()">Take a Tour</a>
                        <br><br><hr><br/>',
    );

    $form['markup_idp_nameid_attr_map_note'] = array(
        '#markup' => '<div style="background-color: #e0e0e0; border-radius: 4px; padding: 0.55em; width: 78%">'
            . '<span class="miniorange_class_color_red"><b>Note:</b></span> NameID attribute value is sent in SAML Response.
            Users in your Service Provider will be searched (existing users) or created (new users) based on this attribute.
            Use <b><code><u>EmailAddress</u></code></b> by default.</b></div>',
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

    $form['markup_idp_attr_header2'] = array(
        '#markup' => '<br><br><div id="mo_saml_idp_vt_attrmapp"><h4>ATTRIBUTE STATEMENT (OPTIONAL)<a href="' . Utilities::getLicensingPageURL() . '"> [Premium]</a></h4><hr><br>',
    );

    $form['miniorange_saml_idp_attr1_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Attribute Name'),
        '#attributes' => array('style' => 'width:80%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Attribute Name'),
        '#disabled' => TRUE,
    );

    $form['miniorange_saml_idp_attr1_value'] = array(
        '#type' => 'select',
        '#title' => t('Attribute Value'),
        '#options' => array(
            '' => t('Select Attribute Value'),
        ),
        '#attributes' => array('style' => 'width:80%;background-color: hsla(0,0%,0%,0.08) !important;'),
        '#disabled' => TRUE,
    );

     $form['markup_idp_user_attr_header'] = array(
         '#markup' => '</div><div id="mo_saml_vt_add_attr"><br/><h4>ADDITIONAL USER ATTRIBUTES (OPTIONAL)
                        <input type="button" class="mo_saml_btn mo_saml_btn-primary" style="width:3%;margin-left:2%;margin-right:2%;" value="+" onclick="add_user_attibute();" />
                        <input type="button" class="mo_saml_btn mo_saml_btn-danger" style="width:3%;" value="-" onclick="remove_user_attibute();" /></b><a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/licensing"> &nbsp;&nbsp;[Premium]</a></h4>',
         '#disabled' => TRUE,
         );

     $form['markup_idp_user_attr_note'] = array(
         '#markup' => '<div class="messages status" style="width: 73%"><b>User Profile Attribute Name:</b> It is the name which you want to send to your SP. It should be unique.
                        <br /><b>User Profile Attribute Value:</b> It is the user attribute (machine name) whose value you want to send to SP.</div></div><br/><br/>',
     );

  $form['miniorange_saml_idp_attr_map_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#submit' => array('miniorange_saml_idp_save_attribute_mapping'),
    //'#attributes' => array('style' => 'border-radius:4px; background: #337ab7; color: #ffffff; text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;display:block;margin-left:auto;  margin-right:auto;'),
    '#suffix' => '</div>',
  );

    Utilities::spConfigGuide($form, $form_state,'AD');

    Utilities::AddSupportButton($form, $form_state);

    return $form;

 }

 /**
 * Configure IdP.
 */
 function miniorange_saml_idp_save_attribute_mapping($form, &$form_state){
     $nameid_attr = $form['miniorange_saml_idp_nameid_attr_map']['#value'];
     $nameid_attr_value = $nameid_attr == '' ? 'emailAddress' : $nameid_attr;
     variable_set('miniorange_saml_idp_nameid_attr_map', $nameid_attr_value);
     drupal_set_message(t('Your settings are saved successfully.'));
 }
