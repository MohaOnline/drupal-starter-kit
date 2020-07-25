<?php
    include "mo_saml_visualTour.php";
    /**
     * Attribute & Role Mapping tab form
     */
    function miniorange_attr_mapping($form, $form_state)
    {
        global $base_url;
        drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
        drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

        $email_attr = variable_get('miniorange_oauth_client_email_attr_val','');
        $name_attr = variable_get('miniorange_oauth_client_name_attr_val','');
        $form['header_top_style_1'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

        $form['markup_top'] = array(
            '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
        );

        $form['markup_top_vt_start'] = array(
            '#markup' => '<b><span style="font-size: 17px;">ATTRIBUTE MAPPING</span></b><a class="btn btn-primary btn-large restart_button" id="restart_tour_button">'
                . 'Restart Tour</a><br><br><hr><br/><b>Note: </b>Please copy the attribute name with <b>email</b> and <b>username</b> from the <b>test configuration</b> window for successful SSO.<br><br>'
        );

        $form['miniorange_oauth_client_email_attr'] = array(
            '#type' => 'textfield',
            '#title' => t('Email Attribute: '),
            '#default_value' => $email_attr,
            '#required' => TRUE,
            '#attributes' => array('id'=>'mo_oauth_vt_attrn','style' => 'width:73%;','placeholder' => 'Enter Email Attribute Name'),
        );

        $form['miniorange_oauth_client_name_attr'] = array(
            '#type' => 'textfield',
            '#title' => t('Username Attribute: '),
            '#default_value' => $name_attr,
            '#description' => t("<b>Note:</b> If this text field is empty, then by default email id will be the user's username"),
            '#attributes' => array('id'=>'mo_oauth_vt_attre','style' => 'width:73%;','placeholder' => 'Enter Username Attribute Name'),
        );

        $form['miniorange_oauth_client_attr1_setup_button'] = array(
            '#type' => 'submit',
            '#id' => 'button_config_center',
            '#value' => t('Save Configuration'),
            '#submit' => array('miniorange_oauth_client_attr_setup_submit'),
        );

        $form['markup_cam'] = array(
            '#markup' => '<br><h3>Custom Attribute Mapping <a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Standard, Premium, Enterprise]</b></a></h3>
                          <div class="mo_saml_highlight_background_note_1">Add the Drupal field attributes in the Attribute Name textfield and add the OAuth Server attributes that you need to map with the drupal attributes in the OAuth Server Attribute Name textfield. Drupal Field Attributes will be of type text. Add the machine name of the attribute in the Drupal Attribute textfield. 
                          <br><b>For example:</b> If the attribute name in the drupal is name then its machine name will be field_name.</div><br>',
        );

        $form['miniorange_oauth_attr_name'] = array(
            '#type' => 'textfield',
            '#prefix' => '<table style="border: unset"><tr><td>',
            '#suffix' => '</td>',
            '#id' => 'text_field',
            '#title' => t('OAuth Server Attribute Name 1'),
            '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter OAuth Server Attribute Name'),
            '#required' => FALSE,
            '#disabled' => TRUE,
        );
        $form['miniorange_oauth_server_name'] = array(
            '#type' => 'textfield',
            '#id' => 'text_field',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
            '#title' => t('Attribute Name 1'),
            '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Attribute Name'),
            '#required' => FALSE,
            '#disabled' => TRUE,
        );
        $form['miniorange_oauth_add_name'] = array(
            '#prefix' => '<td>',
            '#suffix' => '</td>',
            '#type' => 'button',
            '#disabled' => 'true',
            '#attributes' => array('style' => 'background-color: lightgreen;color:white'),
            '#value' => '+',
        );
        $form['miniorange_oauth_sub_name'] = array(
            '#prefix' => '<td style="border: unset">',
            '#suffix' => '</td></tr></table>',
            '#type' => 'button',
            '#disabled' => 'true',
            '#attributes' => array('style' => 'background-color: red;color:white'),
            '#value' => '-',
        );

        $form['markup_role'] = array(
            '#markup' => '<br><h3>Custom Role Mapping <a href="' . $base_url . '/admin/config/people/oauth_login_oauth2/licensing"><b>[Premium, Enterprise]</b></a></h3><hr><br>',
        );

        $form['miniorange_disable_attribute'] = array(
            '#type' => 'checkbox',
            '#title' => t('Do not update existing user&#39;s role'),
	        '#disabled' => TRUE,
        );

        $form['miniorange_oauth_disable_role_update'] = array(
            '#type' => 'checkbox',
            '#title' => t('Check this option if you do not want to update user role if roles not mapped.'),
	        '#disabled' => TRUE,
        );

        $form['miniorange_oauth_disable_autocreate_users'] = array(
            '#type' => 'checkbox',
            '#title' => t('Check this option if you want to disable <b>auto creation</b> of users if user does not exist.'),
	        '#disabled' => TRUE,
        );

	    $mrole= user_roles($membersonly = TRUE);
	    $drole = array_search(variable_get('miniorange_oauth_default_role',''),$mrole);

        $form['miniorange_oauth_default_mapping'] = array(
            '#type' => 'select',
            '#id' => 'miniorange_oauth_client_app',
	        '#title' => t('Select default group for the new users'),
	        '#options' => $mrole,
	        '#default_value' => $drole,
            '#attributes' => array('style' => 'width:73%;'),
            '#disabled' => TRUE,
        );

	    foreach($mrole as $roles) {
            $rolelabel = str_replace(' ','',$roles);
            $form['miniorange_oauth_role_' . $rolelabel] = array(
	            '#type' => 'textfield',
	            '#title' => t($roles),
	            '#default_value' => variable_get('miniorange_oauth_role_' . $rolelabel, ''),
                '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Semi-colon(;) separated Group/Role value for ' . $roles),
	            '#disabled' => TRUE,
            );
        }

        $form['markup_role_signin'] = array(
            '#markup' => '<br><h3>Custom Login/Logout (Optional)</h3><hr>'
        );

        $form['miniorange_oauth_client_login_url'] = array(
            '#type' => 'textfield',
            '#id' => 'text_field',
            '#required' => FALSE,
            '#disabled' => TRUE,
            '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Login URL'),
        );
        $form['miniorange_oauth_client_logout_url'] = array(
            '#type' => 'textfield',
            '#id' => 'text_field',
            '#required' => FALSE,
            '#disabled' => TRUE,
            '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Logout URL'),
        );

        $form['miniorange_oauth_client_attr_setup_button'] = array(
            '#type' => 'submit',
            '#id' => 'button_config_center',
            '#value' => t('Save Configuration'),
            '#disabled' => TRUE,
            '#submit' => array('miniorange_oauth_client_attr_setup_submit'),
        );
        $form['mo_header_style_end'] = array('#markup' => '</div>');
        Utilities::show_attr_list_from_idp($form, $form_state);
        $form['mo_markup_div_imp']=array('#markup'=>'</div>');
        Utilities::AddSupportButton($form, $form_state);
        return $form;
    }
    /*
     * Attribute mapping save settings handler
     */
    function miniorange_oauth_client_attr_setup_submit($form,$form_state)
    {
        $email_attr = trim($form['miniorange_oauth_client_email_attr']['#value']);
        $name_attr = trim($form['miniorange_oauth_client_name_attr']['#value']);

        if(empty($email_attr)){
            drupal_set_message(t('The <b>Email Attribute</b> field is required.'), 'error');
            return;
        }

        variable_set('miniorange_oauth_client_email_attr_val',$email_attr);
        variable_set('miniorange_oauth_client_name_attr_val',$name_attr);
        $app_values = variable_get('miniorange_oauth_client_appval','');

        $app_values['miniorange_oauth_client_email_attr'] = $email_attr;
        $app_values['miniorange_oauth_client_name_attr'] = $name_attr;
        variable_set('miniorange_oauth_client_appval', $app_values);
        drupal_set_message(t('Attribute Mapping saved successfully.'));
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

    function clear_attr_list(&$form,$form_state){
        variable_del('miniorange_oauth_client_attr_list_from_server');
        Utilities::show_attr_list_from_idp($form, $form_state);
    }
?>

