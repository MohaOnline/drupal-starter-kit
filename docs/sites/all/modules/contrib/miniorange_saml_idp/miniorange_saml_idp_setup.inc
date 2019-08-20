<?php
/**
 * @file
 * Set up Identity Provide for miniOrange SAML IDP Module.
 */

 /**
 * Showing IdP Setup form info.
 */
include "mo_saml_visualTour.php";
 function miniorange_saml_idp_setup($form, &$form_state)
 {
     drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
     drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

     global $base_url;

     $disable = Utilities::isModuleConfigured();

     $form['miniorange_saml_IDP_tab'] = array(
         '#markup' => '<div class="mo_saml_table_layout_1"><div class="mo_saml_table_layout mo_saml_container">
                    <div id="tabhead"><h5> Enter the information gathered from your Service Provider &nbsp; OR &nbsp;<a class="btn btn-primary btn-large" 
                  id="mosaml_upload" style="padding:5px 10px;margin-left: 0px;box-shadow: 0 1px 0 #006799;" onclick="show_metadata_form()">Upload SP Metadata</a>
                 <a class="btn btn-primary btn-large restart_button" id="restart_tour_button" style="margin-right: 0px;" >Take a Tour</a></h5></div><hr>',
     );

     $form['metadata_1'] = array(
         '#markup' =>'<div border="1" id="upload_metadata_form" style="background-color:#FFFFFF; border:2px solid #CCCCCC; padding:1px 1px 1px 10px; display:none;" >'
             . '		<table style="width:100%; border: none">'
             .'		        <tr> <td colspan="3">'
             .'				<h3>UPLOAD SP METADATA'
             .'					<span style="float:right;margin-right:25px;">'
             .'						<input type="button" class="btn btn-sm btn-danger" value="Cancel" onclick = "hide_metadata_form()"/></a><br>'
             .'					</span>',
     );
     $form['metadata_2'] = array(
         '#markup' => '</span><br><br></h1></td></tr><tr>
                        <td><h4>Upload Metadata  :</h4><td colspan="2">',
     );

     $form['metadata_file'] = array(
         '#type' => 'file',
     );

     $form['metadata_upload'] = array(
         '#type' => 'submit',
         '#value' => t('Upload'),
         '#submit' => array('miniorange_saml_upload_file'),
         '#attributes' => array('style' => 'background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;border-radius: 4px;'),
     );

     $form['metadata_3'] = array(
         '#markup' =>'</td></td></tr><tr><td colspan="3" ><p style="font-size:13pt;text-align:center;"><b>OR</b></p></td></tr><tr>
                        <td width="20%"><h4>Enter metadata URL:</h4></td><td width="10%">',
     );

     $form['metadata_URL'] = array(
         '#type' => 'textfield',
         '#attributes' => array('placeholder' => 'Enter metadata URL of your SP.'),
     );

     $form['metadata_4'] = array(
         '#markup' =>'</td><td>',
     );

     $form['metadata_fetch'] = array(
         '#type' => 'submit',
         '#value' => t('Fetch Metadata'),
         '#submit' => array('miniorange_saml_fetch_metadata'),
         '#attributes' => array('style' => 'background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;border-radius: 4px;'),
     );

     $form['metadata_4'] = array(
         '#markup' =>'</td></tr><tr><td></td><td>',
     );


     $form['metadata_5'] = array(
         '#markup' => '</td></tr></table></div>',
     );

     $form['markup_idp_note'] = array(
        '#markup' => '<div id="idpdata"><br><div>Please note down the following information from your Service Provider'
	    . ' and keep it handy to configure your Identity Provider.</div>',
     );

     $form['markup_idp_list'] = array(
        '#markup' => '<b><ol><li>SP Entity ID / Issuer</li>'
	    . ' <li>ACS URL</li>'
	    . ' <li>X.509 Certificate for Signing if you are using HTTP-POST Binding. [This is a'
	    . ' <a href="' . Utilities::getLicensingPageURL() . '">premium</a> feature]</li>'
	    . ' <li>X.509 Certificate for Encryption. [This is a'
	    . ' <a href="' . Utilities::getLicensingPageURL() . '">premium</a> feature]</li>'
	    . ' <li>NameID Format</li></ol></b><br />',
     );

     $form['miniorange_saml_idp_sp_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Service Provider Name<span class="miniorange_class_color_red">*</span>'),
        '#default_value' => variable_get('miniorange_saml_idp_sp_name', ''),
        '#attributes' => array('style' => 'width:85%', 'placeholder' => 'Service Provider Name'),
     );

     $form['miniorange_saml_idp_sp_entity_id'] = array(
        '#type' => 'textfield',
        '#title' => t('SP Entity ID or Issuer<span class="miniorange_class_color_red">*</span>'),
        '#description' => t('<b>Note:</b> You can find the EntityID in Your SP-Metadata XML file enclosed in <code>EntityDescriptor</code> tag having attribute as <code>entityID</code>.'),
        '#default_value' => variable_get('miniorange_saml_idp_sp_entity_id', ''),
        '#attributes' => array('style' => 'width:85%', 'placeholder' => 'SP Entity ID or Issuer'),
     );

     $form['miniorange_saml_idp_nameid_format'] = array(
         '#type' => 'select',
         '#title' => t('NameID Format:'),
         '#options' => array(
             '1.1:nameid-format:emailAddress' => t('urn:oasis:names:tc:SAML:1.1:nameid-format:emailAddress'),
             '1.1:nameid-format:unspecified' => t('urn:oasis:names:tc:SAML:1.1:nameid-format:unspecified'),
             '2.0:nameid-format:transient' => t('urn:oasis:names:tc:SAML:1.1:nameid-format:transient'),
             '2.0:nameid-format:persistent' => t('urn:oasis:names:tc:SAML:1.1:nameid-format:persistent'),
         ),
        '#attributes' => array('style' => 'width:85%'),
        '#default_value' => variable_get('miniorange_saml_idp_nameid_format', ''),
        '#description' => t('<b>Note:</b> <code>urn:oasis:names:tc:SAML:1.1:nameid-format:emailAddress</code> is selected by default'),
     );

     $form['miniorange_saml_idp_acs_url'] = array(
        '#type' => 'textfield',
        '#title' => t('ACS URL<span class="miniorange_class_color_red">*</span>'),
        '#description' => t('<b>Note:</b> You can find the SAML Login URL in Your SP-Metadata XML file enclosed in <code>AssertionConsumerService </code> tag <br/>having attribute as <code>Location</code>.'),
        '#default_value' => variable_get('miniorange_saml_idp_acs_url', ''),
        '#attributes' => array('style' => 'width:85%','placeholder' => 'ACS URL'),
     );

     $form['miniorange_saml_idp_relay_state'] = array(
        '#type' => 'textfield',
        '#title' => t('Relay State'),
        '#description' => t('<b>Note:</b>It specifes the landing page at the service provider once SSO completes.'),
        '#attributes' => array('style' => 'width:85%','placeholder' => 'Relay State (optional)'),
        '#default_value' => variable_get('miniorange_saml_idp_relay_state', ''),
     );

     $form['miniorange_saml_idp_single_logout_url'] = array(
         '#type' => 'textfield',
         '#title' => t('Single Logot URL (optional):<a href="' . Utilities::getLicensingPageURL() . '"> [Premium]</a>'),
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Sinle Logout URL'),
         '#disabled' => TRUE,
     );

    $form['miniorange_saml_idp_x509_certificate_request'] = array(
         '#type' => 'textarea',
         '#title' => t('x.509 Certificate Value  <b>[Note: For Signed Request] </b><a href="' . Utilities::getLicensingPageURL() . '"> [Premium]</a>'),
         '#cols' => '10',
         '#rows' => '5',
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Copy and Paste the content from the downloaded certificate or copy the content enclosed in X509Certificate tag (has parent tag KeyDescriptor use=signing) in SP-Metadata XML file)'),
         '#disabled' => TRUE,
         '#resizable' => False,
        '#description' =>'<b>NOTE:</b> Format of the certificate:<br><b>-----BEGIN CERTIFICATE-----<br>'
            .'XXXXXXXXXXXXXXXXXXXXXXXXXXX<br>-----END CERTIFICATE-----</b><br><br>'
    );


    $form['miniorange_saml_idp_x509_certificate_assertion'] = array(
         '#type' => 'textarea',
        '#title' => t('x.509 Certificate Value <b>[Note: For Encrypted Assertion]</b> <a href="' . Utilities::getLicensingPageURL() . '"> [Premium]</a>'),
        '#cols' => '10',
        '#rows' => '5',
        '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Copy and Paste the content from the downloaded certificate or copy the content enclosed in X509Certificate tag (has parent tag KeyDescriptor use=encryption)'),
        '#disabled' => TRUE,
        '#resizable' => False,
        '#description' =>'<b>NOTE:</b> Format of the certificate:<br><b>-----BEGIN CERTIFICATE-----<br>'
            .'XXXXXXXXXXXXXXXXXXXXXXXXXXX<br>-----END CERTIFICATE-----</b><br><br><br>'
    );

     $form['miniorange_saml_idp_assertion_signed'] = array(
         '#type' => 'checkbox',
         '#title' => t('Assertion Signed (Check If you want to sign SAML Assertion)'),
         '#required'=>FALSE,
         '#default_value' => variable_get('miniorange_saml_idp_assertion_signed', FALSE),
         '#prefix' => '<div id="mo_saml_idps_vt_checkbox">',
         '#suffix' => '</div>',
     );

    $form['miniorange_saml_idp_response_signed'] = array(
        '#markup' => '<br>',
        '#type' => 'checkbox',
        '#title' => t('Response Signed'),
        '#disabled' => TRUE,
        '#description' => t('<b>Note: </b>This feature is available in the<a href="' . Utilities::getLicensingPageURL() . '"> Premium</a> version.'),
        '#prefix' => '<div class="mo_saml_highlight_background_note">',
    );

    $form['miniorange_saml_idp_encrypt_signed'] = array(
        '#type' => 'checkbox',
        '#title' => t('Encrypted Assertion'),
        '#disabled' => TRUE,
        '#description' => t('<b>Note: </b>This feature is available in the<a href="' . Utilities::getLicensingPageURL() . '"> Premium</a> version.'),
        '#suffix' => '</div><br><br><br>',
    );

     $form['miniorange_saml_idp_config_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Configuration'),
        '#submit' => array('miniorange_saml_idp_save_idp_config'),
        '#attributes' => array('style' => 'background: #337ab7;border-radius: 4px;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;'),
     );


     $form['miniorange_saml_idp_test_config_button'] = array(
        '#markup' => '<a class="btn btn-success btn-large" style="padding:3px 7px;  margin-right: 14px;" onclick="testIdpConfig(\'' . getTestUrl() . '\');">'
        . 'Test Configuration</a>'
     );

     $form['miniorange_saml_idp_config_delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete Configuration'),
        '#submit' => array('miniorange_saml_idp_delete_idp_config'),
        '#disabled' => $disable,
        '#attributes' => array('style' => 'background: #be4e34;border-radius: 4px;color: #ffffff;text-shadow: 0 -1px 1px #be4e34, 1px 0 1px #be4e34, 0 1px 1px #be4e34, -1px 0 1px #be4e34;box-shadow: 0 1px 0 #be4e34;border-color: #be4e34 #be4e34 #be4e34;'),
        '#suffix' => '</div></div>'
     );

     Utilities::spConfigGuide($form, $form_state);

     Utilities::AddSupportButton($form, $form_state);

     return $form;
 }

function miniorange_saml_upload_file($form, &$form_state) {
    $file_name = $_FILES['files']['tmp_name']['metadata_file'];
    $file      = file_get_contents( $file_name );
    Utilities::upload_metadata( $file );
}

function miniorange_saml_fetch_metadata($form, &$form_state) {
    $url=filter_var($form['metadata_URL']['#value'],FILTER_SANITIZE_URL);
    $arrContextOptions=array(
        "ssl"=>array(
            "verify_peer"=>false,
            "verify_peer_name"=>false,
        ),
    );
    $file = file_get_contents( $url, false, stream_context_create( $arrContextOptions ) );
    Utilities::upload_metadata( $file );
}

 function getTestUrl() {
  global $base_url;
  //$issuer = variable_get('miniorange_saml_idp_sp_entity_id');
  //$acs = variable_get('miniorange_saml_idp_acs_url');
  $testUrl = $base_url . '/?q=mo_testIdpConfig';
  return $testUrl;
 }

 /**
 * Configure IdP.
 */
 function miniorange_saml_idp_save_idp_config($form, &$form_state) {
  global $base_url;
  $sp_name = $form['miniorange_saml_idp_sp_name']['#value'];
  $issuer = $form['miniorange_saml_idp_sp_entity_id']['#value'];
  $acs_url = $form['miniorange_saml_idp_acs_url']['#value'];
  $relay_state = $form['miniorange_saml_idp_relay_state']['#value'];
  $nameid_format = $form['miniorange_saml_idp_nameid_format']['#value'];
  $is_assertion_signed = $form['miniorange_saml_idp_assertion_signed']['#value'] == 1 ? TRUE : FALSE;

  if( empty( $sp_name ) || empty( $issuer ) || empty( $acs_url ) ){
     drupal_set_message(t('The <b>Service Provider Name, SP Entity ID or Issuer, ACS URL</b> fields are required.'), 'error');
     return;
  }

  $idp_name = $base_url;
  variable_set('miniorange_saml_idp_issuer', $idp_name);
  variable_set('miniorange_saml_idp_sp_name', $sp_name);
  variable_set('miniorange_saml_idp_sp_entity_id', $issuer);
  variable_set('miniorange_saml_idp_acs_url', $acs_url);
  variable_set('miniorange_saml_idp_relay_state', $relay_state);
  variable_set('miniorange_saml_idp_nameid_format', $nameid_format);
  variable_set('miniorange_saml_idp_assertion_signed', $is_assertion_signed);

  drupal_set_message(t('Your Service Provider Configuration are successfully saved. You can click on Test Configuration button below to test these configurations.'));

 }

 function miniorange_saml_idp_delete_idp_config($form, &$form_state){
     variable_del('miniorange_saml_idp_sp_name');
     variable_del('miniorange_saml_idp_sp_entity_id');
     variable_del('miniorange_saml_idp_acs_url');
     variable_del('miniorange_saml_idp_relay_state');
     variable_del('miniorange_saml_idp_nameid_format');
     variable_del('miniorange_saml_idp_assertion_signed');

     drupal_set_message(t('Your Service Provider Configuration is successfully deleted.'));
 }
