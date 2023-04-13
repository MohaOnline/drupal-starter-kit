<?php

include_once('includes/Utilities.php');
include "mo_saml_visualTour.php";

function miniorange_oauth_client_license($form, $form_state)
{
    global $base_url;

    drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_js( drupal_get_path('module', 'oauth_login_oauth2') . '/js/slide_support_button.js');


      $form['header_top_style_2'] = array(
          '#markup' => '<div class="mo_saml_table_layout_1"><div class="mo_saml_table_layout">'
      );

    $form['markup_1'] = array(
        '#markup' =>'<br><h3>&nbsp;&nbsp;&nbsp;UPGRADE PLANS <a style="margin-right: 20px;" class="btn btn-primary btn-large restart_button" id="restart_tour_button">'
            . 'Restart Tour</a></h3><br><hr>'
    );

    if(!Utilities::isCustomerRegistered())
    {
        $username = variable_get('miniorange_oauth_client_customer_admin_email');
        $URL_Redirect_std = "https://login.xecurify.com/moas/login?username=".$username."&redirectUrl=https://login.xecurify.com/moas/initializepayment&requestOrigin=drupal_oauth_client_standard_plan";
        $URL_Redirect_pre = "https://login.xecurify.com/moas/login?username=".$username."&redirectUrl=https://login.xecurify.com/moas/initializepayment&requestOrigin=drupal_oauth_client_premium_plan";
        $URL_Redirect_ent = "https://login.xecurify.com/moas/login?username=".$username."&redirectUrl=https://login.xecurify.com/moas/initializepayment&requestOrigin=drupal_oauth_client_enterprise_plan";
        $targetBlank = 'target="_blank"';
    } else {
        $URL_Redirect_std = $base_url.'/admin/config/people/oauth_login_oauth2/customer_setup';
        $URL_Redirect_pre = $base_url.'/admin/config/people/oauth_login_oauth2/customer_setup';
        $URL_Redirect_ent = $base_url.'/admin/config/people/oauth_login_oauth2/customer_setup';
        $targetBlank = '';
    }

    $form['markup_free'] = array(
        '#markup' => '<html lang="en">
        <head>
            <meta charset="utf-8">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <!-- Main Style -->
        </head>
        <body>
        <!-- Pricing Table Section -->
        <section id="pricing-table">
            <div class="container_1">
                <div class="row">
                    <div class="pricing">
                        <div>
                            <div class="pricing-table class_inline_1">
                                <div class="pricing-header">
                                    <h2 class="pricing-title">Features / Plans</h2>
                                </div>
                                <div class="pricing-list">
                                    <ul>
                                    <li>OAuth Provider Support</li>
                                        <li>Auto fill OAuth servers configuration</li>
                                        <li>Basic Attribute Mapping (Email, Username)</li>
                                        <li>Export Configuration</li>
                                        <li>Auto Create Users</li>
                                        <li>Import Configuration</li>
                                        <li>Advanced Attribute Mapping (Username, Email, First Name, Custom Attributes, etc.)</li>
                                        <li>Custom Redirect URL after login and logout</li>
                                        <li>Basic Role Mapping (Support for default role for new users)</li>
                                        <li>Advanced Role Mapping</li>
                                        <li>Force authentication / Protect complete site</li>
                                        <li>OpenId Connect Support (Login using OpenId Connect Server)</li>
                                        <li>Domain specific registration</li>
                                        <li>Dynamic Callback URL</li>
                                        <li>Page Restriction</li>
                                        <li>Login Reports / Analytics</li>
                                    </ul>
                                </div>
                            </div>
                            <div class="pricing-table class_inline">
                                <div class="pricing-header">
                                <p class="pricing-title">Free</p>
                                <br><p class="pricing-rate"><sup>$</sup> 0</p>
                                <div class="filler-class"></div>
                                    <a class="btn btn-primary">You are on this plan</a>
                                </div>
                            <div class="pricing-list">
                                <ul>
                                <li>1</li>
                                <li>&#x2714;</li>
                                <li>&#x2714;</li>
                                <li>&#x2714;</li>
                                <li></li>
                                <li> </li>
                                <li> </li>
                                <li> </li>
                                <li> </li>
                                <li> </li>
                                <li> </li>
                                <li> </li>
                                <li> </li>
                                <li></li>
                                <li></li>
                                <li> </li>
                                </ul>
                            </div>
                        </div>

                        <div class="pricing-table class_inline">
                            <div class="pricing-header">
                                <p class="pricing-title">Standard<br><span>(Role and Attribute mapping)<br>[One Time Payment]</span></p>
                                <br><p class="pricing-rate"><sup>$</sup> 249</p>
                                <div class="filler-class"></div>
                                 <a href="'.$URL_Redirect_std.'" '.$targetBlank.' class="btn btn-primary">Click to Upgrade</a>
                        </div>

                        <div class="pricing-list">
                            <ul>
                            <li>1</li>
                            <li>&#x2714;</li>
                            <li>&#x2714;</li>
                            <li>&#x2714;</li>
                            <li>Unlimited</li>
                            <li>&#x2714;</li>
                            <li>&#x2714;</li>
                            <li>&#x2714;</li>
                            <li>&#x2714;</li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            </ul>
                        </div>
                        </div>
                        <div class="pricing-table class_inline">
                            <div class="pricing-header">
                                <p class="pricing-title">Premium<br><span>(OpenID support)<br>[One Time Payment]</span></p>
                                <br><p class="pricing-rate"><sup>$</sup> 399</p>
                                 <a href="'.$URL_Redirect_pre.'" '.$targetBlank.' class="btn btn-primary">Click to Upgrade</a>
                            </div>

                            <div class="pricing-list">
                                <ul>
                                <li>1</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>Unlimited</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li></li>
                                    <li></li>
                                    <li></li>
                                    <li></li>
                                </ul>
                            </div>
                        </div>
                        <div class="pricing-table class_inline">
                            <div class="pricing-header">
                                <p class="pricing-title">Enterprise<br><span>(Domain & Page restriction)<br>[One Time Payment]</span></p>
                                <br><p class="pricing-rate"><sup>$</sup> 449</p>
                                 <a href="'.$URL_Redirect_ent.'" '.$targetBlank.' class="btn btn-primary">Click to Upgrade</a>
                            </div>

                            <div class="pricing-list">
                                <ul>
                                    <li>1*</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>Unlimited</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                    <li>&#x2714;</li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </section>
    <!-- Pricing Table Section End -->
<p style="padding-left: 1.5%"><b> Steps to Upgrade to Premium Module -</b></p>
 <p style="padding-left: 1.5%">1. After Clicking on the <b>Click to upgrade</b> button, you will be redirected to miniOrange Login Console. Enter your username and password with which you created an account with us. After that you will be redirected to payment page.<br>
    2. Enter your card details and complete the payment. On successful payment completion, you will see the link to download the premium module.<br>
    3. Once you download the premium module, first delete existing module then install the premium module.<br><br>
 * Multiple OAuth providers are supported using Xecurify Broker service. For more information contact us at using the <b>Support </b> button or by dropping us an email at <a href="mailto:drupalsupport@xecurify.com">drupalsupport@xecurify.com</a></p><br>
    <p style="padding-left: 1.5%"><b>10 Days Return Policy - </b><br><br>
    At miniOrange, we want to ensure that you are 100% happy with your purchase. If the module you purchased is not working as advertised and you have attempted to resolve any issues with our support team, which could not get resolved, we will refund the whole amount given that you raised a request for refund within the first 10 days of the purchase. Please email us at <a href="mailto:drupalsupport@xecurify.com">drupalsupport@xecurify.com</a> for any queries regarding the return policy.</p><br><br>
    </body>
    </html>',
    );

    Utilities::AddrfdButton($form, $form_state);
    return $form;
}

/**
 * Send support query.
 */
function send_rfd_query(&$form, $form_state) {
    $email = trim($form['customer_email']['#value']);
    $demo_plan = $form['demo_plan']['#value'];
    $description_doubt = trim($form['description_doubt']['#value']);
    $query = $demo_plan.' -> '.$description_doubt;

    if(valid_email_address($email))
        Utilities::send_query($email,null, $query, 'demo');
    else {
        drupal_set_message('Please enter valid Email ID', 'error');
        return;
    }
}

?>
