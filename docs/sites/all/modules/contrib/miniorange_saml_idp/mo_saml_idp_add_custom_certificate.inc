<?php
include "mo_saml_visualTour.php";
function mo_saml_idp_add_custom_certificate($form, &$form_state)
{
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));


    $form['miniorange_saml_generate_metadata'] = array(
        '#markup' => '<div class="mo_saml_table_layout_1"><div class="mo_saml_table_layout mo_saml_container">',
    );

    $form['mo_saml_metadata'] = array(
        '#markup' =>'<div border="1" id="generate_certificate_form" style="background-color:#FFFFFF; border:0px solid #CCCCCC; padding:1px 1px 1px 10px; display:none;" >
                    <table style="width:100%;">
                        <h4>GENERATE CUSTOM CERTIFICATE<span style="float:right;margin-right:25px;">
            				<input type="button" class="btn btn-sm btn-danger" value="Back" onclick = "hide_gen_cert_form()"/></a><br>
           				    </span><br><br><b><hr></b><br></h4>',
    );

    $form['mo_saml_certificate_enterprise_version'] = array(
        '#markup' => '<div class="mo_saml_highlight_background_note" style="width: 85%"><b>Note:</b> Available in <a href="' . Utilities::getLicensingPageURL() . '">  Premium</a> version of the module.</div><br>',
    );

    $form['mo_saml_country_code_text'] = array(
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width:80%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Country code:'),
        '#description' => t('<b>Note: </b>Check your country code <a href="https://www.digicert.com/ssl-certificate-country-codes.htm" target="_blank">here.</a>'),
        '#disabled' => TRUE,
        '#prefix' => '<tr>
                        <td width="20%"><h4 style="margin-left: 44px;">Country code :</h4></td><td width="10%">'
    );

    $form['mo_saml_certificate_state_name'] = array(
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width:80%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'State Name:'),
        '#disabled' => TRUE,
        '#prefix' => '<tr>
                        <td width="20%"><h4 style="margin-left: 44px;">State :</h4></td>
                        <td width="30%">',
    );

    $form['mo_saml_certificate_company_name'] = array(
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width:80%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Company Name:'),
        '#disabled' => TRUE,
        '#prefix' => '<tr>
                        <td width="20%"><h4 style="margin-left: 44px;">Company :</h4></td><td width="10%">',
    );

    $form['miniorange_saml_unit_name'] = array(
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width:80%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Unit name'),
        '#disabled' => TRUE,
        '#prefix' => '<tr>
                        <td width="20%"><b style="margin-left: 44px;font-size: 14px;">Unit :</b></td><td width="10%">',
    );

    $form['mo_saml_certificate_common_name'] = array(
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width:80%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Common Name:'),
        '#disabled' => TRUE,
        '#prefix' => '<tr>
                        <td width="20%"><b style="margin-left: 44px;font-size: 14px;">Common :</b></td>
                        <td width="10%">'
    );

    $form['mo_saml_select_digest_algo'] = array(
        '#type' => 'select',
        '#options' => array(
            'sha512' => t('SHA512'),
        ),
        '#attributes' => array('style' => 'width:80%'),
        '#disabled' => FALSE,
        '#prefix' => '<tr>
                        <td width="20%"><h4 style="margin-left: 44px;">Digest Algorithm :</h4></td>
                        <td width="10%">',
    );

    $form['mo_saml_select_private_key_bit'] = array(
        '#type' => 'select',
        '#options' => array(
            '2048' => t('2048 bits'),
        ),
        '#attributes' => array('style' => 'width:80%'),
        '#disabled' => FALSE,
        '#prefix' => '<tr>
                        <td width="20%"><h4 style="margin-left: 44px;">Bits to generate the private key :</h4></td>
                        <td width="10%">'
    );

    $form['mo_saml_select_valid_days'] = array(
        '#type' => 'select',
        '#options' => array(
            '365' => t('365 days'),
        ),
        '#attributes' => array('style' => 'width:80%'),
        '#disabled' => FALSE,
        '#prefix' => '<tr>
                        <td width="20%"><h4 style="margin-left: 44px;">Valid Days :</h4></td>
                        <td width="10%">'
    );

    $form['metadata_5'] = array(
        '#markup' => '</td></tr></table>',
    );

    $form['generate_config_elements'] = array(
        '#type' => 'submit',
        '#value' => t('Generate Self-Signed Certs'),
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#submit' => array('miniorange_saml_generate_custom_certificate'),
        '#attributes' => array('style' => 'display:block;margin-left:auto;margin-right:auto;border-radius:4px;background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;'),
        '#disabled' => TRUE,
    );

    $form['metadata_i'] = array(
        '#markup' => '</div><div id="mo_gen_cert">',
    );

    $form['markup_1'] = array(
        '#markup' => '<h3>ADD CUSTOM CERTIFICATE </h3><a class="btn btn-primary btn-large" id="mosaml_upload" style="background-color:#12b233;box-shadow: 0 1px 0 #12b233;text-decoration: none;border-color: #00ba26;
            text-shadow: 0 -1px 1px #12b233, 1px 0 1px #12b233, 0 1px 1px #12b233, -1px 0 1px #12b233;padding:5px 10px;float: right;margin-top: -37px;" onclick="show_gen_cert_form()">'
            . 'Generate</a><hr><br/>',
    );

    $form['miniorange_saml_idp_custom_cert_settings'] = array(
        '#markup' => '<div class = mo_saml_highlight_background_note style="width: 75%"><b>NOTE: </b>This tab will help you to save and generate new certificates.
                        <br><br>You can perform following operations:<br><b> 1.</b> Copy your own certificates and save them to use it for SSO.<br>
                        <b>2.</b> You can generate new certificates and use them for SSO.<br><b> 3.</b> Reset setting to get back default settings.</div><br><br>',
    );

    $form['miniorange_saml_idp_private_certificate'] = array(
        '#type' => 'textarea',
        '#title' => t('X.509 Private Certificate.<b><a href="' . Utilities::getLicensingPageURL() . '"> [Premium]</a></b>'),
        '#cols' => 5,
        '#rows' => 5,
        '#attributes' => array('style' => 'width:77%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' =>"Copy and Paste the content from the downloaded certificate or copy the content enclosed in X509Certificate tag (has parent tag KeyDescriptor use=signing) in IdP-Metadata XML file."),
        '#resizable' => False,
        '#disabled' => TRUE,
        '#description' => '<strong>NOTE :</strong> Format of the certificate:<br /><strong>-----BEGIN CERTIFICATE-----<br />'
            . 'XXXXXXXXXXXXXXXXXXXXXXXXXXX<br />-----END CERTIFICATE-----</strong><br /><br /><br /><br />'
    );

    $form['miniorange_saml_idp_publ_certificate'] = array(
        '#type' => 'textarea',
        '#title' => t('X.509 Public Certificate <b><a href="' . Utilities::getLicensingPageURL() . '"> [Premium]</a></b>'),
        '#cols' => 5,
        '#rows' => 5,
        '#attributes' => array('style' => 'width:77%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' =>"Copy and Paste the content from the downloaded certificate or copy the content enclosed in X509Certificate tag (has parent tag KeyDescriptor use=signing) in IdP-Metadata XML file."),
        '#resizable' => False,
        '#disabled' => TRUE,
        '#description' => '<strong>NOTE :</strong> Format of the certificate:<br /><strong>-----BEGIN CERTIFICATE-----<br />'
            . 'XXXXXXXXXXXXXXXXXXXXXXXXXXX<br />-----END CERTIFICATE-----</strong><br /><br /><br /><br />'
    );

    $form['save_config_elements'] = array(
        '#type' => 'submit',
        '#name'=>'submit',
        '#value' => t('Upload'),
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#disabled' => TRUE,
        //'#attributes' => array('style' => 'width:100px;background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;'),
    );

    $form['save_config_elements1'] = array(
        '#type' => 'submit',
        '#value' => t('Reset'),
        '#disabled' => TRUE,
        //'#attributes' => array('style' => 'width:100px;background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;'),
        '#prefix' => '<td>',
        '#suffix' => '</td></div></div>',
    );

    Utilities::spConfigGuide($form, $form_state);

    Utilities::AddSupportButton($form, $form_state);

    return $form;
}

