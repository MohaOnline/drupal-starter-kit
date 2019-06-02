<?php

    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/mo-card.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_js(drupal_get_path('module', 'miniorange_saml_idp') . '/js/dru_visual_tour.js');

    $Tour_taken = variable_get('mo_saml_tourTaken_' . getPage_name(), false);

    drupal_add_js(array('moTour' => array(
        'pageID' => getPage_name(),
        'tourData' => getTourData(getPage_name(),$Tour_taken),
        'tourTaken' => $Tour_taken,
        'addID' => addID(),
        'pageURL' => $_SERVER['REQUEST_SCHEME'] . "://" . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'],

    )),array('type' => 'setting'));

if(isset($_POST['doneTour']) && isset($_POST['pageID']))
{
    variable_set('mo_saml_tourTaken_'.$_POST['pageID'], $_POST['doneTour']);
}

function getPage_name()
{
    $link = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on' ? "https" : "http") . "://" . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'];
    $exploded = explode('/', $link);
    $l_url = end($exploded);
    $l_url = multiexplode(array("?","%","#",":","="),$l_url);
    $f_url = $l_url[0];
    return $f_url;
}

function multiexplode ($delimiters,$string)
{
    $ready = str_replace($delimiters, $delimiters[0], $string);
    $launch = explode($delimiters[0], $ready);
    return  $launch;
}

function mo_visual_tour()
{
    $firstTour = true;
    echo '<div id="restart_tour_button" class="mo-otp-help-button static" style="margin-right:10px;z-index:10">
    <button class="button button-primary button-large">
    <span class="dashicons dashicons-controls-repeat" style="margin:5% 0 0 0;"></span>
        '.mo_("Restart Tour").'
    </button>
    </div>';
}

function addID()
{
    $idArray = array(

        array(
            'selector'  =>'.form-select',
            'newID'     =>'mo_idp_vt_dropdown',
        ),
        array(
            'selector'  =>'.form-item form-type-textarea form-item-miniorange-saml-idp-x509-certificate-request form-disabled',
            'newID'     =>'mo_idp_vt_cert',
        ),
        array(
            'selector'  =>'.sticky-enabled',
            'newID'     =>'mo_idp_vt_conf_table',
        ),
        array(
            'selector'  =>'.mo_saml_table_layout_support_1',
            'newID'     =>'mo_idp_vt_sp_support',
        ),
        array(
            'selector'  =>'.tabs li:nth-of-type(1)>a',
            'newID'     =>'mo_saml_idp_vt_idp',
        ),
        array(
            'selector'  =>'.tabs li:nth-of-type(2)>a',
            'newID'     =>'mo_saml_idp_vt_sp',
        ),
        array(
            'selector'  =>'.tabs li:nth-of-type(3)>a',
            'newID'     =>'mo_saml_idp_vt_mapp',
        ),
        array(
            'selector'  =>'.tabs li:nth-of-type(4)>a',
            'newID'     =>'mo_saml_idp_vt_import',
        ),
        array(
            'selector'  =>'.tabs li:nth-of-type(5)>a',
            'newID'     =>'mo_saml_idp_vt_custom_cert',
        ),
        array(
            'selector'  =>'.tabs li:nth-of-type(6)>a',
            'newID'     =>'mo_saml_idp_vt_license',
        ),array(
            'selector'  =>'.tabs li:nth-of-type(7)>a',
            'newID'     =>'mo_saml_idp_vt_acnt',
        ),
        array(
            'selector'  =>'.mo_saml_table_layout',
            'newID'     =>'mo_saml_idp_vt_signup',
        ),

    );
    return $idArray;
}


function getTourData($pageID,$Tour_Taken)
{
    $tourData = array();

    if($Tour_Taken == FALSE)
        $tab_index = 'miniorange_saml_idp';
    else $tab_index = 'idp_tab';

    $tourData['miniorange_saml_idp'] = array(
        0 =>    array(
            'targetE'       =>  'mo_idp_vt_conf_table',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>Identity Provider metadata URLs</h1>',
            'contentHTML'   =>  'You can manually configure your Service Provider using the information given here.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
        ),
        1 =>    array(
            'targetE'       =>  'mo_saml_idp_vt_metadata',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>Identity Provider metadata URL</h1>',
            'contentHTML'   =>  'Provide this Metadata URL to configure your Service Provider.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
        ),
        2 =>    array(
            'targetE'       =>  'mosaml-feedback-form',
            'pointToSide'   =>  'right',
            'titleHTML'     =>  '<h1>Need Help ?</h1>',
            'contentHTML'   =>  'If you need any help, you can just send us a query so we can help you.',
            'ifNext'        =>  true,
            'buttonText'    =>  'End Tour',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
            'ifskip'        =>  'hidden',
        ),
    );

    $tourData[$tab_index] = array(
        0 =>    array(
            'targetE'       =>  'mosaml-feedback-form',
            'pointToSide'   =>  'right',
            'titleHTML'     =>  '<h1>Need help?</h1>',
            'contentHTML'   =>  'Get in touch with us and we will help you setup the plugin in no time.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
        ),
        1 =>    array(
            'targetE'       =>  'mo_saml_idp_vt_idp',
            'pointToSide'   =>  'up',
            'titleHTML'     =>  '<h1>Identity Provider Tab</h1>',
            'contentHTML'   =>  'This tab provides details to configure your <b>Service Provider</b>.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
        ),
        2 =>    array(
            'targetE'       =>  'mo_saml_idp_vt_sp',
            'pointToSide'   =>  'up',
            'titleHTML'     =>  '<h1>Service Provider Tab</h1>',
            'contentHTML'   =>  'Configure this tab using <b>Service Provider</b> information which you get from <b>SP-Metadata XML</b>',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
        ),
        3 =>    array(
            'targetE'       =>  'mo_saml_idp_vt_mapp',
            'pointToSide'   =>  'up',
            'titleHTML'     =>  '<h1>Attribute Mapping</h1>',
            'contentHTML'   =>  'In this tab you can find <b>attribute mapping</b>. This attribute value will be send in <b>SAML Response</b>',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
        ),
        4 =>    array(
            'targetE'       =>  'mo_saml_idp_vt_import',
            'pointToSide'   =>  'up',
            'titleHTML'     =>  '<h1>Import/Export</h1>',
            'contentHTML'   =>  'This tab will help you to transfer your plugin configurations when you change your Drupal instance.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
        ),
        5 =>    array(
            'targetE'       =>  'mo_saml_idp_vt_license',
            'pointToSide'   =>  'up',
            'titleHTML'     =>  '<h1>Upgrade here</h1>',
            'contentHTML'   =>  'You can find <b>Premium features</b> and could upgrade to our <b>Premium plans</b>.',
            'ifNext'        =>  true,
            'buttonText'    =>  'End Tour',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
        ),
        6 =>    array(
            'targetE'       =>  'mo_idp_vt_conf_table',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>Identity Provider metadata URLs</h1>',
            'contentHTML'   =>  'You can manually configure your Service Provider using the information given here.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
        ),
        7 =>    array(
            'targetE'       =>  'mo_saml_idp_vt_metadata',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>Identity Provider metadata URL</h1>',
            'contentHTML'   =>  'Provide this Metadata URL to configure your Service Provider.',
            'ifNext'        =>  true,
            'buttonText'    =>  'End Tour',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
            'ifskip'        =>  'hidden',
        ),

    );

    $tourData['idp_setup'] = array(
        0 =>    array(
            'targetE'       =>  'mosaml_upload',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>Upload Your Metadata</h1>',
            'contentHTML'   =>  'If you have a <b>metadata URL</b> or <b>file</b> provided by your SP, click on the button or you can configure the module manually also.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
        ),
        1 =>    array(
            'targetE'       =>  'edit-miniorange-saml-idp-sp-name',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>Service Provider name</h1>',
            'contentHTML'   =>  'Enter your service provider name.(you can enter any name)',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
        ),
        2 =>    array(
            'targetE'       =>  'edit-miniorange-saml-idp-sp-entity-id',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>SP Entity ID or Issuer</h1>',
            'contentHTML'   =>  'You can find the EntityID in Your SP-Metadata XML file enclosed in EntityDescriptor tag having attribute as entityID.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
        ),
        3 =>    array(
            'targetE'       =>  'edit-miniorange-saml-idp-acs-url',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>ACS URL</h1>',
            'contentHTML'   =>  'You can find the SAML Login URL in Your SP-Metadata XML file enclosed in AssertionConsumerService tag having attribute as Location.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
        ),
        4 =>    array(
            'targetE'       =>  'edit-miniorange-saml-idp-relay-state',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>Relay State</h1>',
            'contentHTML'   =>  'It specifes the landing page at the service provider once SSO completes.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
        ),
        5 =>    array(
            'targetE'       =>  'mo_saml_idps_vt_checkbox',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>Assertion Signed</h1>',
            'contentHTML'   =>  'Enable the checkbox if you want to sign SAML Assertion.',
            'ifNext'        =>  true,
            'buttonText'    =>  'Next',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
        ),
        6 =>    array(
            'targetE'       =>  'mosaml-feedback-form',
            'pointToSide'   =>  'right',
            'titleHTML'     =>  '<h1>Need help?</h1>',
            'contentHTML'   =>  'Get in touch with us and we will help you setup the plugin in no time.',
            'ifNext'        =>  true,
            'buttonText'    =>  'End Tour',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
            'ifskip'        =>  'hidden',
        ),
    );

    $tourData['attr_mapping'] = array(
        0 =>    array(
            'targetE'       =>  'mo_idp_vt_dropdown',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>NameID Attribute</h1>',
            'contentHTML'   =>  'This attribute value will be send in SAML Response. Users in your Service Provider will be searched or created based on this attribute. Use EmailAddress by default.',
            'ifNext'        =>  true,
            'buttonText'    =>  'End Tour',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
            'ifskip'        =>  'hidden',
        ),
    );

    $tourData['export_config'] = array(
        0 =>    array(
            'targetE'       =>  'mosaml_vt_impexp',
            'pointToSide'   =>  'left',
            'titleHTML'     =>  '<h1>Download Configuration</h1>',
            'contentHTML'   =>  'If you are having trouble setting up the plugin, Export the configurations and mail us at info@miniorange.com.',
            'ifNext'        =>  true,
            'buttonText'    =>  'End Tour',
            'img'           =>  array(),
            'cardSize'      =>  'largemedium',
            'action'        =>  '',
            'ifskip'        =>  'hidden',
        ),
    );

    $tourData['customer_setup'] = array(

        0 =>    array(
            'targetE'       =>  'mosaml-feedback-form',
            'pointToSide'   =>  'right',
            'titleHTML'     =>  '<h1>Need help?</h1>',
            'contentHTML'   =>  'Get in touch with us and we will help you setup the plugin in no time.',
            'ifNext'        =>  true,
            'buttonText'    =>  'End Tour',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
            'ifskip'        =>  'hidden',
        ),

    );

    $tourData['custom_certificate'] = array(
        0 =>    array(
            'targetE'       =>  'mosaml-feedback-form',
            'pointToSide'   =>  'right',
            'titleHTML'     =>  '<h1>Need help?</h1>',
            'contentHTML'   =>  'Get in touch with us and we will help you setup the plugin in no time.',
            'ifNext'        =>  true,
            'buttonText'    =>  'End Tour',
            'img'           =>  array(),
            'cardSize'      =>  'medium',
            'action'        =>  '',
            'ifskip'        =>  'hidden',
        ),

    );
    return $tourData[$pageID];
}

/*
                            ********************************
                                    array terms :
                            ********************************
pageID              -   your Page ID, contains array of popups
0                   -   Popup/card number, goes from zero to n. For next Tab card use 'nextCard' instead of number
targetE             -   Element to target to. Has to be element ID without #. If no ID, add one. Empty For none, shows in centre of screen if empty
pointToSide         -   Direction of arrow to point to (up,down,left,right), for no arrow-keep empty (places at center keep targetE empty) //look at this fix
titleHTML           -   Title of card, can be HTML code
contentHTML         -   Content of card, can be HTML code
ifNext              -   if to show(true) Next Button or not(false), Keep False for Card Number('nextTab')
buttonText          -   Next Button Text
img                 -   image(icon) attributes ('src' should not be 'empty' with 'visible' true)
                        src     -   url of image(best for ico/transparent png) icon(https://visualpharm.com/assets/262/Comments-595b40b65ba036ed117d3e48.svg)
                        visible -   to show image or not, true or false
cardSize            -   Card has 3 difined sizes- big, medium and small. Recomended not to use image with small
nextTab             -   This is special card used if you want user to move to next tab during tour, disabled during restart tour

 */