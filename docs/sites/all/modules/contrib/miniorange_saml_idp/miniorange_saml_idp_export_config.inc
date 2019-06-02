<?php
/**
 * @file
 * Contains support form for miniOrange SAML Login Module.
 */
include dirname(__FILE__) . "/includes/miniorange_saml_idp_enum.php";
include "mo_saml_visualTour.php";

define( "Tab_Class_Names", serialize( array(
    "Identity_Provider" => 'mo_options_enum_identity_provider',
    "Service_Provider"  => 'mo_options_enum_service_provider',
) ) );
/**
 * Showing Support form info.
 */
function miniorange_saml_idp_export_config($form, &$form_state)
{
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

    global $base_url;

    $form['header_top_style_1'] = array('#markup' => '<div class="mo_saml_table_layout_1">');

    $form['header_top_style_tops'] = array('#markup' => '<div class="mo_saml_table_layout mo_saml_container">');

    $form['markup_top_head'] = array(
        '#markup' => '<b><span style="font-size: 17px;">IMPORT/EXPORT CONFIGURATIONS</span></b><a class="btn btn-primary btn-large restart_button" id="restart_tour_button">'
            . 'Take a Tour</a><br><br><hr><br/>'
    );

    $form['markup_1'] = array(
        '#markup' => '<div class="mo_saml_highlight_background_note" style="width: 97%"><p><b>NOTE: </b>This tab will help you to transfer your plugin configurations when you change your Drupal instance. 
                        <br>Example: When you switch from test environment to production.<br>Follow these 3 simple steps to do that:<br>
                        <br>1. Download plugin configuration file by clicking on the Download Plugin Configuration button given below.
                        <br>2. Install the plugin on new Drupal instance.<br>3. Upload the configuration file in Import Plugin Configurations section.<br>
                        <br><b>Choose "json" Extened plugin configuration file and upload by clicking on the button given below.</b><br>
                        And just like that, all your plugin configurations will be transferred! You can also send us this file along with your support query.</p></div>',
    );

    $form['markup_div_imp1']=array('#markup'=>'<div id="mosaml_vt_import">');

    $form['miniorange_idp_guide_link_container'] = array(
        '#markup' => '<br><br><div class="mo_saml_tadble_layout mo_saml_container_3" style="text-align: center;float: left;margin-right: 20px;border: solid 1px #00000024;padding-bottom: 50px;">',
    );

    $form['mo_markup_div']=array('#markup'=>'<div id="mosaml_vt_impexp">');

    $form['mo_markup_top'] = array(
        '#markup' => '<b><span style="font-size: 17px;">EXPORT CONFIGURATION</span></b><br><br><hr><br/><br>'
    );

    if(miniorange_saml_is_sp_configured()) {
        $form['miniorange_saml_idp_export'] = array(
            '#type' => 'submit',
            '#value' => t('Download Plugin Configuration'),
            '#prefix' => '<td>',
            '#suffix' => '</td>',
            '#submit' => array('miniorange_import_export'),
            '#attributes' => array('id'=>'mosaml_vt_export','style' => 'width:77%;background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;'),
        );
    }else {
        $form['miniorange_saml_idp_export'] = array(
            '#markup' => '<div style="display:block; margin-top:10px; color:red; background-color:rgba(251, 232, 0, 0.15); font-size: medium;padding:5px;border:solid 1px rgba(255, 0, 9, 0.36);">
	                            Please <a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/idp_setup">configure the plugin</a> first to export the configurations.
                          </div>',
        );
    }

    $form['mo_markup_test_div']=array('#markup'=>'</div></div>');

    $form['miniorange_idp_guide_link_cont'] = array(
        '#markup' => '<div class="mo_saml_tadble_layout mo_saml_container_3" style="float: left;text-align:center;padding-bottom: 20px;border: solid 1px #00000024;;">',
    );

    $form['mo_markup_import'] = array(
        '#markup' => '<b><span style="font-size: 17px;">IMPORT CONFIGURATION <b><a href="' . $base_url . '/admin/config/people/miniorange_saml_idp/licensing"> [Premium]</a></b></span></b><br><br><hr><br>',
    );

    $form['mo_import_Config_file'] = array(
        '#type' => 'file',
        '#disabled' => TRUE,
        '#attributes' => array('id'=>'mosaml_vt_import','style'=>'width: 175px;'),
    );

    $form['miniorange_saml_idp_import_config'] = array(
        '#type' => 'submit',
        '#value' => t('Upload'),
        '#submit' => array('miniorange_import_Config'),
        '#disabled' => TRUE,
        '#attributes' => array('id'=>'mosaml_vt_export','style' => 'width:100px;background: #337ab7;color: #ffffff;text-shadow: 0 -1px 1px #337ab7, 1px 0 1px #337ab7, 0 1px 1px #337ab7, -1px 0 1px #337ab7;box-shadow: 0 1px 0 #337ab7;border-color: #337ab7 #337ab7 #337ab7;;margin-top: -47px;'),
    );

    $form['mo_markusp_test_div']=array('#markup'=>'</div>');

    $form['mo_markup_idp_attr_header_top_div_close'] = array('#markup' => '</div></div>',);

    $form['miniorange_idp_setup_guide_link'] = array(
        '#markup' => '<div class="mo_saml_table_layout mo_saml_container_2">',
    );

    $form['miniorange_idp_guide_link1'] = array(
        '#markup' => '<div style="font-size: 15px;"><b>To see detailed documentation of how to configure Drupal 7 SAML IdP with any Service Provider</b></div></br>',
    );

    Utilities::spConfigGuide($form, $form_state);

    $form['mo_markup_div_imp']=array('#markup'=>'</div>');

    Utilities::faq($form, $form_state);

    Utilities::AddSupportButton($form, $form_state);

    return $form;
}

function miniorange_saml_is_sp_configured() {
    $saml_login_url  = variable_get( 'miniorange_saml_idp_acs_url' );
    $saml_idp_issuer = variable_get( 'miniorange_saml_idp_sp_entity_id' );
    if ( ! empty( $saml_login_url ) && ! empty( $saml_idp_issuer ) ) {
        return 1;
    } else {
        return 0;
    }
}

function miniorange_import_export() {

    $tab_class_name      = unserialize( Tab_Class_Names );
    $configuration_array = array();
    foreach ( $tab_class_name as $key => $value ) {
        $configuration_array[ $key ] = mo_get_configuration_array( $value );
    }
    $configuration_array["Version_dependencies"]=mo_get_version_informations();
    header( "Content-Disposition: attachment; filename=miniorange-saml-idp-config.json" );
    echo( json_encode( $configuration_array, JSON_PRETTY_PRINT|JSON_UNESCAPED_SLASHES) );
    exit;
}

function mo_get_configuration_array( $class_name ) {
    $class_object = call_user_func( $class_name . '::getConstants' );
    $mo_array = array();
    foreach ( $class_object as $key => $value ) {

        $mo_option_exists=variable_get($value);
        if($mo_option_exists){
            if(@unserialize($mo_option_exists)!==false){
                $mo_option_exists = unserialize($mo_option_exists);
            }
            $mo_array[ $key ] = $mo_option_exists;
        }
    }
    return $mo_array;
}

function mo_get_version_informations(){
    $array_version = array();
    $array_version["PHP_version"] = phpversion();
    $array_version["Drupal_version"] = VERSION;
    $array_version["OPEN_SSL"] = mo_saml_is_openssl_installed();
    $array_version["CURL"] = mo_saml_is_curl_installed();
    $array_version["ICONV"] = mo_saml_is_iconv_installed();
    $array_version["DOM"] = mo_saml_is_dom_installed();

    return $array_version;
}

function mo_saml_is_openssl_installed() {
    if ( in_array( 'openssl', get_loaded_extensions() ) ) {
        return 1;
    } else {
        return 0;
    }
}

function mo_saml_is_curl_installed() {
    if ( in_array( 'curl', get_loaded_extensions() ) ) {
        return 1;
    } else {
        return 0;
    }
}

function mo_saml_is_iconv_installed(){

    if ( in_array( 'iconv', get_loaded_extensions() ) ) {
        return 1;
    } else {
        return 0;
    }
}

function mo_saml_is_dom_installed(){

    if ( in_array( 'dom', get_loaded_extensions() ) ) {
        return 1;
    } else {
        return 0;
    }
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