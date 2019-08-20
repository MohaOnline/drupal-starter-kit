<?php
/**
 * @file
 * Contains Service Provider information for miniOrange SAML Login Module.
 */

 /**
 * Showing Service Provider information.
 */
include "mo_saml_visualTour.php";
function miniorange_sp_information($form, &$form_state) {
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

  global $base_url;

  $issue1 = variable_get('miniorange_saml_idp_issuerID','');
  $issuer = $issue1 != '' ? $issue1 : $base_url . '/?q=admin/config/people/miniorange_saml_idp/';
 
  $url = $base_url . '/';
  $login_url = $url . '?q=initiatelogon';
  $module_path = drupal_get_path('module', 'miniorange_saml_idp');

  variable_set('miniorange_saml_issuer_id', $issuer);
  variable_set('miniorange_saml_login_url', $login_url);

  $form['markup_idp_header_1'] = array(
      '#markup' => '<div class="mo_saml_table_layout_1"><div class="mo_saml_table_layout mo_saml_container"><b><span style="font-size: 17px;">IDP METADATA</span></b>
                    <a class="btn btn-primary btn-large restart_button" id="restart_tour_button">'
                    . 'Take a Tour</a><br><br><hr><br/>',
  );

    $form['mo_saml_metadata_option'] = array(
        '#markup' => '<div style="color: #d9534f;"><b>Provide this module information to your Service Provider team. You can choose any one of the below options:</b></div>',
    );

    $form['markup_idp_sp_2'] = array(
        '#markup' => '<div id="mo_saml_idp_vt_metadata">
                        <br><b>a) Provide this metadata URL to your Service Provider:</b><br>
                        <div class="mo_saml_highlight_background_url_note" >
                            <code><b>
                                <span id="idp_metadata_url">
                                    <a target="_blank" href="' . $base_url . '/?q=moidp_metadata">' . $base_url . '/?q=moidp_metadata</a>
                                </span></b>
                            </code>
                        </div>
                        <img class ="fa fa-fw fa-lg fa-copy mo_copy" style="margin-left: 10px;" onclick="copyToClipboard(\'#idp_metadata_url\');" src="'. $base_url.'/'.$module_path . '/includes/images/copy-regular.svg"></div>',
    );

    $form['mo_saml_download_btn_title'] = array(
        '#markup' => '<br><div id="visual_tour_download_metadataXMLFile"><b>b) Download the Module XML metadata and upload it on your Service Provider : </b>
                        <span><a href="' . $base_url . '/?q=moidp_download_metadata"  class="btn btn-primary btn-large mo_saml_btn" style="padding: 4px 10px;">Download XML Metadata</a></span></div>',
    );

    $form['header'] = array(
        '#markup' => '<br><br><div><b>c) Provide the following information to your Service Provider. Copy it and keep it handy.</b></div><br>',
    );

    $form['mo_saml_attrs_list_idp'] = array(
        '#markup' => '<div class="table-responsive" style="font-family: sans-serif;font-size: 12px;" id="mo_idp_vt_conf_table">   
                <table class="mo_guide_table mo_guide_table-striped mo_guide_table-bordered" style="border: 1px solid #ddd;max-width: 100%;border-collapse: collapse;">
                    <thead>
                        <tr>
                            <th class="mo_guide_text-center mo_td_values">ATTRIBUTE</th>
                            <th class="mo_guide_text-center mo_td_values">VALUE</th>   
                            <tbody style="font-size: 12px;color:gray;">
                                <tr>
                                    <td style="font-weight:bold;padding: 15px;">IDP-Entity ID / Issuer</td>
                                    <td>						               
						               <span id="issuer_id">' . $issuer . '</span>
                                       <img class ="fa fa-fw fa-pull-right fa-lg fa-copy mo_copy" onclick="copyToClipboard(\'#issuer_id\');" src="'. $base_url.'/'.$module_path . '/includes/images/copy-regular.svg">			
						            </td>                                    
                                </tr>
                                <tr>
                                    <td style="font-weight:bold;padding: 15px;">SAML Login URL</td>
                                    <td>						               
						               <span id="acs_url">' . $login_url . '</span>
						               <img class ="fa fa-fw fa-pull-right fa-lg fa-copy mo_copy" onclick="copyToClipboard(\'#acs_url\');" src="'. $base_url.'/'.$module_path . '/includes/images/copy-regular.svg">       
						            </td>                                    
                                </tr>
                                <tr>
                                    <td style="font-weight:bold;padding: 15px;">SAML Logout URL</td>
                                    <td>						               
						               <span id="base_url">Available in </b><a href="' . Utilities::getLicensingPageURL() . '"> Premium</a> version of the module.</span>						                      
						            </td>                                  
                                </tr>
                                <tr>
                                    <td style="font-weight:bold;padding: 15px;">Certificate (Optional)</td>
                                     <td>						               
						               <span id="bacs_url"><a href="' . $base_url . '/' . $module_path . '/resources/idp-signing.crt" download="idp-signing.crt">Download</a> X509 Certificate.</span>
						                      
						            </td>                                    
                                </tr><tr>
                                    <td style="font-weight:bold;padding: 15px;">Response Signed</td>
                                    <td>						               
						               <span id="base_url">Available in </b><a href="' . Utilities::getLicensingPageURL() . '"> Premium</a> version of the module.</span>						                      
						            </td>   
                                </tr>
                                <tr>
                                    <td style="font-weight:bold;padding: 15px;">Assertion Signed</td>
                                    <td style="padding: 15px;">You can choose to sign your response in <a href="' . $base_url . '/?q=admin/config/people/miniorange_saml_idp/idp_setup">Service Provider</a> tab</td>
                                </tr>                              
                            </tbody>                                                 
                        </tr>
                    </thead>
                    </table>
          <script>               
              function copyToClipboard(element) {
                jQuery(".selected-text").removeClass("selected-text");
                var temp = jQuery("<input>");
                jQuery("body").append(temp);
                jQuery(element).addClass("selected-text");
                temp.val(jQuery(element).text().trim()).select();
                document.execCommand("copy");
                temp.remove();
            }
              jQuery(window).click(function(e) {
                  console.log(e.target.className);
                  if( e.target.className == undefined || e.target.className.indexOf("mo_copy") == -1)
                   jQuery(".selected-text").removeClass("selected-text");
              });
          </script>'
    );

    $form['miniorange_idp_guide_link'] = array(
        '#markup' => '</div></div>',
    );

    Utilities::spConfigGuide($form, $form_state);

    Utilities::AddSupportButton($form, $form_state);

    return $form;
 }

