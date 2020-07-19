<?php
/**
 * @file
 * Contains Licensing information for miniOrange SAML IDP Module.
 */

/**
 * Showing Licensing form info.
 */
function miniorange_saml_idp_licensing() {

  drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  drupal_add_css( drupal_get_path('module', 'miniorange_saml_idp'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  drupal_add_js( drupal_get_path('module', 'miniorange_saml_idp'). '/js/dru_visual_tour.js' , array('group' => JS_DEFAULT, 'every_page' => FALSE));

  $form['markup_0'] = array(
    '#markup' =>'<div class="mo_saml_table_layout_1"><div class="mo_saml_table_layout" style="margin-right: -5px;"><br><h3>&emsp;'.t('UPGRADE PLANS').'  </h3><hr>'
  );

  $form['markup_free'] = array(
    '#markup' => t('<html lang="en">
        <head>
            <meta charset="utf-8">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <!-- Main Style -->
        </head>
        <body>
        <!-- Pricing Table Section -->
        <section id="mo_idp_pricing-table">
            <div class="mo_idp_container_1">
                <div class="row">
                    <div class="mo_idp_pricing">
                        
                        <div class="mo_idp_pricing-table mo_idp_class_inline">
                            <div class="mo_idp_pricing-header">
                                <p class="mo_idp_pricing-title">Drupal Lite Plan<br></p>
                                <p></p><br>
                                <p class="mo_idp_pricing-rate">[Users hosted in miniOrange]<br></p><br>
                                <p></p>
                            </div>  
                            <div class="mo_idp_pricing-list">
                                <ul>
                                    <li>Keep users in miniOrange Database<br><br></li>
                                    <li>Passwords will be hosted in miniOrange<br><br></li>
                                    <li>Cross-Protocol SSO Support <br>SAML <br>OAuth <br>OpenId Connect <br>JWT</li>
                                    <li>Sign-Up via miniOrange login page</li>
                                    <li>Embed miniOrange login widget on your Drupal site <br>OR <br>Use login page hosted on miniOrange</li>
                                    <li>miniOrange Sub-domain will be provided</li>
                                    <li>Social Providers Included <br>(Facebook, Twitter, Google+, etc)</li>
                                    <li>Multi-Factor Authentication Not included</li>
                                    <li>User Provisioning Not included</li>
                                    <li class="mo_idp_pricing-header_2">
                                         <table id="miniOrange_IDP_Pricing">
                                              <tr>
                                                <th style="text-align:left;">User Slabs</th>
                                                <th>Monthly Pricing</th>
                                                <th>Yearly Pricing</th>
                                              </tr>
                                              <tr>
                                                <td></td>
                                                <td><a class="mo_saml_btn mo_saml_btn-danger mo_saml_btn-sm" href="https://www.miniorange.com/contact" target="_blank">Upgrade Now</a></td>
                                                <td><a class="mo_saml_btn mo_saml_btn-danger mo_saml_btn-sm" href="https://www.miniorange.com/contact" target="_blank">Upgrade Now</a></td>
                                              </tr>
                                         </table>
                                    </li>                             
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">1 - 100</span></td><td>$15</td><td>$165</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">100 - 200</span></td><td>$16</td><td>$176</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">200 - 300</span></td><td>$17</td><td>$187</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">300 - 400</span></td><td>$18</td><td>$198</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">400 - 500</span></td><td>$19</td><td>$209</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">500 - 1000</span></td><td>$22</td><td>$242</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">1000 - 2000</span></td><td>$44</td><td>$484</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">2000 - 3000</span></td><td>$66</td><td>$726</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">3000 - 4000</span></td><td>$88</td><td>$968</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">4000 - 5000</span></td><td>$110</td><td>$1155</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td><span style="color:red;">5000 +</span></td><td colspan="2">&emsp;&emsp;&emsp;<a href="https://www.miniorange.com/contact" target="_blank">Request a quote</a></td></table></li>
                                </ul>
                            </div>
                        </div>
                        
                        <div class="mo_idp_pricing-table mo_idp_class_inline">
                            <div class="mo_idp_pricing-header">
                                <p class="mo_idp_pricing-title">Drupal Premium Plan<br></p>
                                <p></p><br>
                                <p class="mo_idp_pricing-rate">[Users stored in your own Drupal database]<br></p><br>
                                <p></p>
                            </div>
                            
                            <div class="mo_idp_pricing-list">
                                <ul>
                                    <li>Keep users in Drupal Database<br><br></li>
                                    <li>Passwords will be stored in your Drupal site<br>Database</li>
                                    <li>SingleProtocol SSO Support <br>SAML<br><br><br><br></li>
                                    <li>User your own Existing Drupal Sign-Up form</li>
                                    <li>User your own Existing Drupal Site Login Page<br><br><br></li>
                                    <li>User your own Drupal Domain</li>
                                    <li>Social Providers Not Included<br><br></li>
                                    <li>Click <a target= _blank href="https://plugins.miniorange.com/drupal-two-factor-authentication-2fa">here</a> to purchase Multi-Factor module separately</li>
                                    <li>User Provisioning Not included</li>
                                    <li class="mo_idp_pricing-header_2">
                                        <table id="miniOrange_IDP_Pricing"">
                                              <tr>
                                                <th>Yearly Pricing</th>
                                              </tr>
                                              <tr>
                                                <td><a class="mo_saml_btn mo_saml_btn-danger mo_saml_btn-sm" onclick="click_to_upgrade_or_register(\'' . miniorange_saml_premium_plan_button() . '\');" >Upgrade Now</a></td>
                                              </tr>
                                              <tr>
                                                <td style="font-size: smaller">[50% from second year onwards]</td>
                                              </tr>
                                         </table> 
                                    </li>                             
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$450</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$550</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$650</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$750</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$850</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$1250</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$1600</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$1900</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$2150</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">$2400</td></table></li>
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;"><a href="https://www.miniorange.com/contact" target="_blank">Request a quote</a></td></table></li>
                                </ul>
                            </div>
                        </div>
                        
                        <div class="mo_idp_pricing-table mo_idp_class_inline">
                            <div class="mo_idp_pricing-header">
                                <p class="mo_idp_pricing-title">Drupal All Inclusive Plan<br></p>
                                <p></p><br>
                                <p class="mo_idp_pricing-rate">[Users hosted in miniOrange or Enterprise Directory or any 3rd party identity provider]<br></p>
                                <p></p>
                            </div>
                            
                            <div class="mo_idp_pricing-list">
                                <ul>
                                    <li>Keep users hosted in miniOrange or Enterprise <br>Directory or any 3rd party identity provider</li>
                                    <li>Passwords can be managed by miniOrange or <br>by the 3rd party providers.</li>
                                    <li>CrossProtocol SSO Support <br>SAML <br>OAuth <br>OpenId Connect <br>JWT</li>
                                    <li>Sign-Up via miniOrange login page</li>
                                    <li>Fully Customized miniOrange Login Page<br><br><br></li>
                                    <li>Fully Custom Domain is provided </li>
                                    <li>Social Providers Included <br>(Facebook, Twitter, Google+, etc)</li>
                                    <li>Multi-Factor Authentication Included<br></li>
                                    <li>User Provisioning Included</li>
                                    <li class="mo_idp_pricing-header_2">                
                                        <table id="miniOrange_IDP_Pricing"">
                                              <tr>
                                                <th>Monthly / Yearly Pricing</th>
                                              </tr>
                                              <tr>
                                                <td><a class="mo_saml_btn mo_saml_btn-danger mo_saml_btn-sm" href="https://www.miniorange.com/contact" target="_blank">Upgrade Now</a></td>
                                              </tr>
                                         </table> 
                                    </li>                             
                                    <li><table class="miniOrange_pricing_table"><td style="text-align:center;">Starts from $0.5 per user per month</td></table></li>
                                    <li style="background-color: #EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;"><a href="https://www.miniorange.com/contact" target="_blank">Request a quote</a></td></table></li>
                                    <li style="color:#EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;">&nbsp;</td></table></li>
                                    <li style="background-color: #EAECEB;color:#EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;">&nbsp;</td></table></li>
                                    <li style="color:#EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;">&nbsp;</td></table></li>
                                    <li style="background-color: #EAECEB; color:#EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;">&nbsp;</td></table></li>
                                    <li style="color:#EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;">&nbsp;</td></table></li>
                                    <li style="background-color: #EAECEB;color:#EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;">&nbsp;</td></table></li>
                                    <li style="color:#EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;">&nbsp;</td></table></li>
                                    <li style="background-color: #EAECEB;color:#EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;">&nbsp;</td></table></li>
                                    <li style="color:#EAECEB;"><table class="miniOrange_pricing_table"><td style="text-align:center;">&nbsp;</td></table></li>
                                </ul>
                            </div>
                        
                    </div>
                </div>
            </div>
        </div>
    </section>
    <!-- Pricing Table Section End -->
    </body>
    </html>'),
  );
  $form['markup_4'] = array(
    '#markup' => '<div class="mo_saml_space"><h3>Service Providers Supported</h3>'
      . 'We support all SAML compliant Service Providers like Tableau, Zendesk, Owncloud, Inkling, Workplace by Facebook, AWS appstream and many more.</div>'
  );

    $form['markup_6'] = array(
        '#markup' => '<br><div class="mo_saml_space"><h3>Steps to Upgrade to Premium Module</h3>'
            . '<ol>
                <li>Click on the <strong>Upgrade Now</strong> button. You will be redirected to miniOrange Login Console. Enter your password with which you created an account with us. After that you will be redirected to payment page.</li>'
            . '<li>Enter your card details and complete the payment. On successful payment completion, you will see the link to download the premium module.</li>'
            . '<li>Once you download the premium module, just unzip it and replace the folder with existing module. Clear Drupal Cache.</li></ol>'
    );
  $form['markup_5'] = array(
      '#markup' => '<br><h3>* Multiple SPs</h3>'
          . 'Additional charges will be applicable based on the number of Service Providers you wish to configure.<br>'
  );
  $form['markup_9']=array(
    '#markup'=>"<br><h3>10 Days Return Policy -</h3>
At miniOrange, we want to ensure you are 100% happy with your purchase. If the module you purchased is not working as advertised and you've attempted to resolve any issues with our support team, which couldn't get resolved, we will refund the whole amount given that you have a raised a refund request within the first 10 days of the purchase. Please email us at info@xecurify.com for any queries regarding the return policy.
    "
  );

  $form['markup_7'] = array(
    '#markup' => '<br><br><h3>** End to End Identity Provider Integration (additional charges applicable)</h3>'
      . ' We will setup a Conference Call / Goto meeting and do end to end configuration for you to setup drupal as IDP.'
      . ' We provide services to do the configuration on your behalf. (Extra charges applicable at $60/hr)<br />'
      . ' If you have any doubts regarding the licensing plans, you can mail us at <a href="mailto:drupalsupport@xecurify.com"><i>drupalsupport@xecurify.com</i></a> or submit a query using the support form <b>(support form available on each tab)</b>.<br><br></div></div>'
  );
  Utilities::AddrfdButton($form,$form_state);

  $form['markup_8'] = array(
    '#markup' => '</div></div>'
  );

  return $form;
}



function miniorange_saml_premium_plan_button(){
    $admin_email =  variable_get('miniorange_saml_idp_customer_admin_email');
    $admin_email = (isset($admin_email) && !empty($admin_email)) ? $admin_email : 'none';
    $URL_Redirect_std = "https://login.xecurify.com/moas/login?username=$admin_email&redirectUrl=https://login.xecurify.com/moas/initializepayment&requestOrigin=drupal_saml_idp_premium_plan";
    variable_set('redirect_plan_after_registration_premium',$URL_Redirect_std);
    return return_url($URL_Redirect_std, 'premium');
}

function return_url($url, $payment_plan){
    if(!Utilities::isCustomerRegistered()){
        return $url;
    }else{
        global $base_url;
        $SAMLrequestUrl = $base_url . '/?q=register_user&payment_plan=' . $payment_plan;
        return $SAMLrequestUrl;
    }
}
// send demo request
function send_rfd_query(&$form, $form_state) {
    $email = trim($form['customer_email']['#value']);
    $demo_plan = 'Drupal 7 IdP Premium Module';
    $description_doubt = trim($form['description_doubt']['#value']);
    $query = $demo_plan.' ->'.$description_doubt;

    if(valid_email_address($email))
        Utilities::send_demo_query($email, $query, $description_doubt);
    else {
        drupal_set_message('Please enter valid Email ID', 'error');
        return;
    }
}
