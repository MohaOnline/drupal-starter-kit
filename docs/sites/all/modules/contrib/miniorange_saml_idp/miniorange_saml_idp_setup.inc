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

     $form['header_top_style_1'] = array('#markup' => '<div class="mo_saml_table_layout_1">',);

     $form['markup_top_btn'] = array(
         '#markup' => '<div class="mo_saml_table_layout mo_saml_container">',
     );

     $form['miniorange_saml_IDP_tab'] = array(
         '#markup' => '<div id="tabhead"><h5> Enter the information gathered from your Service Provider &nbsp; OR &nbsp;<a class="btn btn-primary btn-large" 
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

     $form['metadata_idp_name'] = array(
         '#markup' => '<br><tr><td colspan="3" ></td></tr>'
             .'		    <tr>'
             .'			<td width="20%"><h4>Service Provider name:</h4></td><td width="10%">',
     );

     $form['miniorange_saml_idp_name_3'] = array(
         '#type' => 'textfield',
         '#attributes' => array('style' => 'width:65%;margin-top: 55px;','placeholder' => 'Enter Service Provider name'),
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
         '#markup' => '</td></tr></table></div><div id="idpdata">',
     );

     $form['markup_idp_note'] = array(
        '#markup' => '<br><div>Please note down the following information from your Service Provider'
	    . ' and keep it handy to configure your Identity Provider.</div>',
     );
  
     $form['markup_idp_list'] = array(
        '#markup' => '<b><ol><li>SP Entity ID / Issuer</li>'
	    . ' <li>ACS URL</li>'
	    . ' <li>X.509 Certificate for Signing if you are using HTTP-POST Binding. [This is a'
	    . ' <a href="' . $base_url . '/?q=admin/config/people/miniorange_saml_idp/licensing">premium</a> feature]</li>'
	    . ' <li>X.509 Certificate for Encryption. [This is a'
	    . ' <a href="' . $base_url . '/?q=admin/config/people/miniorange_saml_idp/licensing">premium</a> feature]</li>'
	    . ' <li>NameID Format</li></ol></b><br />',
     );

     $form['miniorange_saml_idp_sp_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Service Provider Name'),
        '#default_value' => variable_get('miniorange_saml_idp_sp_name', ''),
        '#attributes' => array('style' => 'width:85%','placeholder' => 'Service Provider Name'),
     );

     $form['miniorange_saml_idp_sp_entity_id'] = array(
        '#type' => 'textfield',
        '#title' => t('SP Entity ID or Issuer'),
        '#description' => t('<b>Note:</b> You can find the EntityID in Your SP-Metadata XML file enclosed in <code>EntityDescriptor</code> tag having attribute as <code>entityID</code>.'),
        '#default_value' => variable_get('miniorange_saml_idp_sp_entity_id', ''),
        '#attributes' => array('style' => 'width:85%','placeholder' => 'SP Entity ID or Issuer'),
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
        '#description' => t('<b>Note:</b> urn:oasis:names:tc:SAML:1.1:nameid-format:emailAddress is selected by default'),
     );

     $form['miniorange_saml_idp_acs_url'] = array(
        '#type' => 'textfield',
        '#title' => t('ACS URL'),
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
        '#required' => FALSE,
     );

     $form['miniorange_saml_idp_single_logout_url'] = array(
         '#type' => 'textfield',
         '#title' => t('Single Logot URL (optional):<a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/licensing"> [Premium]</a>'),
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Sinle Logout URL'),
         '#disabled' => TRUE,
     );

    $form['miniorange_saml_idp_x509_certificate_request'] = array(
         '#type' => 'textarea',
         '#title' => t('x.509 Certificate Value  <b>[Note: For Signed Request] </b><a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/licensing"> [Premium]</a>'),
         '#cols' => '10',
         '#rows' => '5',
         '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Copy and Paste the content from the downloaded certificate or copy the content enclosed in X509Certificate tag (has parent tag KeyDescriptor use=signing) in SP-Metadata XML file)'),
         '#disabled' => TRUE,
         '#resizable' => False,
    );

    $form['markup_1'] = array(
         '#markup' => '<b>NOTE:</b> Format of the certificate:<br><b>-----BEGIN CERTIFICATE-----<br>'
         .'XXXXXXXXXXXXXXXXXXXXXXXXXXX<br>-----END CERTIFICATE-----</b><br><br>'
    );

    $form['miniorange_saml_idp_x509_certificate_assertion'] = array(
         '#type' => 'textarea',
        '#title' => t('x.509 Certificate Value <b>[Note: For Encrypted Assertion]</b> <a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/licensing"> [Premium]</a>'),
        '#cols' => '10',
        '#rows' => '5',
        '#attributes' => array('style' => 'width:85%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Copy and Paste the content from the downloaded certificate or copy the content enclosed in X509Certificate tag (has parent tag KeyDescriptor use=encryption)'),
        '#disabled' => TRUE,
        '#resizable' => False,
    );

    $form['markup_2'] = array(
         '#markup' => '<b>NOTE:</b> Format of the certificate:<br><b>-----BEGIN CERTIFICATE-----<br>'
         .'XXXXXXXXXXXXXXXXXXXXXXXXXXX<br>-----END CERTIFICATE-----</b><br><br><br>'
    );

     $form['markup_idp_back_color'] = array(
         '#markup' => '<div class="mo_saml_highlight_background_note">',
     );

    $form['miniorange_saml_idp_response_signed'] = array(
         '#markup' => '<br>',
        '#type' => 'checkbox',
        '#title' => t('Response Signed <b>[This is a Premium feature]</b>'
         . ' Check <a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/licensing"><b>Licensing</b></a>'
         . ' Tab to learn more.'),
        '#disabled' => TRUE,
    );

    $form['miniorange_saml_idp_encrypt_signed'] = array(
         '#type' => 'checkbox',
        '#title' => t('Encrypted Assertion <b>[This is a Premium feature]</b>'
         . ' Check <a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/licensing"><b>Licensing</b></a>'
         . ' Tab to learn more.'),
         '#disabled' => TRUE,
    );

     $form['markup_idp_div_vt'] = array(
         '#markup' => '</div><br><div id="mo_saml_idps_vt_checkbox">',
     );

     $form['miniorange_saml_idp_assertion_signed'] = array(
        '#type' => 'checkbox',
        '#title' => t('Assertion Signed (Check If you want to sign SAML Assertion)'),
        '#required'=>FALSE,
        '#default_value' => variable_get('miniorange_saml_idp_assertion_signed', FALSE),
     );

     $form['markup_idp_attr_br'] = array(
        '#markup' => '</div><br><br>',
     );

     $disable_true="";
     $disableval = False;
     $entityid = variable_get('miniorange_saml_idp_sp_entity_id', FALSE);
     $acsurl = variable_get('miniorange_saml_idp_acs_url', FALSE);
     if(empty($entityid) || empty($acsurl)){
         $disable_true = 'disabled="True"';
         $disableval = TRUE;
     }

     $form['miniorange_saml_idp_config_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Configuration'),
        '#submit' => array('miniorange_saml_idp_save_idp_config'),
        '#attributes' => array('style' => 'background: #337ab7;border-radius: 4px;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;'),
     );


     $form['miniorange_saml_idp_test_config_button'] = array(
        '#markup' => '<a '.$disable_true.' class="btn btn-primary btn-large" style="padding:3px 7px;  margin-right: 14px;" onclick="testIdpConfig(\'' . getTestUrl() . '\');">'
        . 'Test Configuration</a>'
     );

     $acs_url = variable_get('miniorange_saml_idp_acs_url', '');
     $entity_id = variable_get('miniorange_saml_idp_sp_entity_id', '');
     $disable_val = ($acs_url == '' || is_null($acs_url) || $entity_id == '' || is_null($entity_id))? TRUE : FALSE;

     $form['miniorange_saml_idp_config_delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete Configuration'),
        '#submit' => array('miniorange_saml_idp_delete_idp_config'),
        '#disabled' => $disableval,
        '#attributes' => array('style' => 'background: #337ab7;border-radius: 4px;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;'),
     );

     $form['markup_idp_attr_header_top_div_close'] = array('#markup' => '</div></div>',);

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

function miniorange_saml_upload_file($form, &$form_state) {
    $sp_name = $form['miniorange_saml_idp_name_3']['#value'];
    if(empty($sp_name) && empty(variable_get('miniorange_saml_idp_sp_name'))){
        drupal_set_message(t('Service Provider Name is required.'),'error');
        return;
    }
    $file_name = $_FILES['files']['tmp_name']['metadata_file'];
    $file = file_get_contents($file_name);
    upload_metadata($file,$sp_name);
}

function upload_metadata($file,$sp_name)
{
    global $base_url;
    require_once drupal_get_path('module', 'miniorange_saml_idp') . DIRECTORY_SEPARATOR . 'includes' . DIRECTORY_SEPARATOR . 'MetadataReader.php';
    $document = new DOMDocument();
    $document->loadXML($file);
    restore_error_handler();
    $first_child = $document->firstChild;

    if(!empty($first_child))
    {
        $metadata = new MetadataReader($document);
        $service_providers = $metadata->getServiceProviders();
        if(empty($service_providers))
        {
            drupal_set_message(t('<b>Please provide a valid metadata file.</b>'),'error');
            return;
        }
        foreach($service_providers as $key => $sp)
        {
            $entityID_issuer = $sp->getEntityID();
            $acs_url = $sp->getAcsURL();
            $is_assertion_signed = $sp->getAssertionsSigned() == 'true' ? TRUE : FALSE;

            if(!empty(variable_get('miniorange_saml_idp_sp_name'))&& empty($sp_name)){
                $sp_name = variable_get('miniorange_saml_idp_sp_name');
                variable_set('miniorange_saml_idp_sp_name', $sp_name);
            }
            else{
                variable_set('miniorange_saml_idp_sp_name', $sp_name);
            }

            variable_set('miniorange_saml_idp_sp_entity_id', $entityID_issuer);
            variable_set('miniorange_saml_idp_acs_url', $acs_url);
            variable_set('miniorange_saml_idp_assertion_signed', $is_assertion_signed);
        }
        drupal_set_message(t('Identity Provider Configuration successfully saved.'));
        return;
    }
    else {
        drupal_set_message(t('<b>Please provide a valid metadata file.</b>'),'error');
        return;
    }
}

function miniorange_saml_fetch_metadata($form, &$form_state) {
    $sp_name = $form['miniorange_saml_idp_name_3']['#value'];
    if(empty($sp_name) && empty(variable_get('miniorange_saml_idp_sp_name'))){
        drupal_set_message(t('Service Provider Name is required.'),'error');
        return;
    }
    $url=filter_var($form['metadata_URL']['#value'],FILTER_SANITIZE_URL);
    $arrContextOptions=array(
        "ssl"=>array(
            "verify_peer"=>false,
            "verify_peer_name"=>false,
        ),
    );
    $file = file_get_contents($url, false, stream_context_create($arrContextOptions));
    upload_metadata($file,$sp_name);
}

 function getTestUrl() {
  global $base_url;
  $issuer = variable_get('miniorange_saml_idp_sp_entity_id');
  $acs = variable_get('miniorange_saml_idp_acs_url');
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

     if(empty($sp_name)||empty($issuer)||empty($acs_url)){
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
  variable_set('miniorange_saml_idp_sp_name', '');
  variable_set('miniorange_saml_idp_sp_entity_id', '');
  variable_set('miniorange_saml_idp_acs_url', '');
  variable_set('miniorange_saml_idp_relay_state', '');
  variable_set('miniorange_saml_idp_nameid_format', '');
  variable_set('miniorange_saml_idp_assertion_signed', '');
  
  drupal_set_message(t('Your Service Provider Configuration is successfully deleted.'));
  }

/**
 * Send support query.
 */
function send_support_query(&$form, $form_state)
{
    $email = $form['miniorange_saml_email_address_support']['#value'];
    $phone = $form['miniorange_saml_phone_number_support']['#value'];
    $query = $form['miniorange_saml_support_query_support']['#value'];
    Utilities::send_query($email, $phone, $query);
}