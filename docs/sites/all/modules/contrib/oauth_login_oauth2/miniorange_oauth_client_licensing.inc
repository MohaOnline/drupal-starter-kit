<?php
function miniorange_oauth_client_license($form, $form_state)
{

    drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
      drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));



      $form['header_top_style_2'] = array(
          '#markup' => '<div class="mo_saml_table_layout_1"><div class="mo_saml_table_layout">'
      );

    $form['markup_1'] = array(
        '#markup' =>'<br><h3>LICENSING PLANS</h3><hr>'
    );

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
                                        <li>Login using the link</li>
                                        <li>Auto Create Users</li>
                                        <li>Advanced Attribute Mapping (Username, Display Name, Email, Group Name)	</li>
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
                                <p class="pricing-rate"><sup>$</sup> 0</p>
                                <div class="filler-class"></div>
                                    <a class="btn btn-primary">You are here</a>
                                </div>
                            <div class="pricing-list">
                                <ul>
                                <li>1</li>
                                <li>&#x2714;</li>
                                <li>&#x2714;</li>
                                <li>&#x2714;</li>
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
                                <p class="pricing-title">Standard</p>
                                <p class="pricing-rate"><sup>$</sup> 99<sup>*</sup></p>
                                <div class="filler-class"></div>
                                 <a href="https://www.miniorange.com/contact" target="_blank" class="btn btn-primary">Contact Us</a>
                        </div>

                        <div class="pricing-list">
                            <ul>
                            <li>1</li>
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
                            <li></li>
                            <li></li>
                            <li></li>
                            </ul>
                        </div>
                        </div>
                        <div class="pricing-table class_inline">
                            <div class="pricing-header">
                                <p class="pricing-title">Premium</p>
                                <p class="pricing-rate"><sup>$</sup> 199<sup>*</sup></p>
                                 <a href="https://www.miniorange.com/contact" target="_blank" class="btn btn-primary">Contact Us</a>
                            </div>

                            <div class="pricing-list">
                                <ul>
                                <li>1</li>
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
                                    <li></li>
                                    <li></li>
                                    <li></li>
                                    <li></li>
                                </ul>
                            </div>
                        </div>
                        <div class="pricing-table class_inline">
                            <div class="pricing-header">
                                <p class="pricing-title">Enterprise</p>
                                <p class="pricing-rate"><sup>$</sup> 249<sup>*</sup></p>
                                 <a href="https://www.miniorange.com/contact" target="_blank" class="btn btn-primary">Contact Us</a>
                            </div>

                            <div class="pricing-list">
                                <ul>
                                    <li>1*</li>
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
    <h3>* If you want support for multiple OAuth Providers, please contact us at <a href="mailto:info@xecurify.com">info@xecurify.com</a></h3>
    </body>
    </html>',
    );


    return $form;
}
    ?>
