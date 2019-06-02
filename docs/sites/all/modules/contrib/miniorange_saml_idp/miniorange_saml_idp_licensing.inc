<?php
/**
 * @file
 * Contains Licensing information for miniOrange SAML IDP Module.
 */

 /**
 * Showing Licensing form info.
 */
 function miniorange_saml_idp_licensing() {

     global $base_url;
     if(!Utilities::isCustomerRegistered())
     {
         $admin_email =  variable_get('miniorange_saml_idp_customer_admin_email');
         $payment_page1 = "https://auth.miniorange.com/moas/login?username=$admin_email&redirectUrl=https://auth.miniorange.com/moas/initializepayment&requestOrigin=drupal_saml_idp_basic_plan";
         $payment_page2 = "https://auth.miniorange.com/moas/login?username=$admin_email&redirectUrl=https://auth.miniorange.com/moas/initializepayment&requestOrigin=drupal_saml_idp_premium_plan";
         $targetBlank = 'target="_blank"';
     }
     else
     {
         $payment_page1=$payment_page2 = $base_url.'/admin/config/people/miniorange_saml_idp/customer_setup';
         $targetBlank = '';
     }

     $form['header_top_style_2'] = array(
         '#markup' => '<div class="mo_saml_table_layout_1"><div class="mo_saml_table_layout">'
     );

     $form['markup_1'] = array(
         '#markup' =>'<br><h3 style="margin-left: 37px;">LICENSING PLANS</h3><hr>'
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
                                    <h1 class="pricing-title" style="margin-bottom:31px;">Features / Plans</h1>
                                </div>
                                <div class="pricing-list">
                                    <ul>
                                        <li>Unlimited Authentications with</li>
                                        <li>SP Initiated Login</li>
                                        <li>Signed Assertion</li>
                                        <li>Metadata XML File</li>
                                        <li>Metadata XML URL</li>
                                        <li>Export Configuration</li>
                                        <li>Import Configuration</li>
                                        <li>Customized Attribute Mapping</li>
                                        <li>Single Logout</li>
                                        <li>Signed Response</li>
                                        <li>Encrypted Assertion</li>
                                        <li>HTTP-POST Binding</li>
                                        <li>Generate Custom certificate</li>                                      
                                        <li>End to End Configuration **</li>
                                    </ul>
                                </div>
                        </div>
                                                     
                        <div class="pricing-table class_inline">
                            <div class="pricing-header">
                                <p class="pricing-title">Free</p><br><br><br><br><br><br>
                                <p class="pricing-rate">                                                                      
                                </p>
                                <div class="filler-class"></div>
                                 <a class="btn btn-primary btn-sm" style="padding:5px; margin-bottom: 6px;font-size: 15px;">You are on this plan</a> <br><br>                                
                            </div>
                            
                            <div class="pricing-list">
                                <ul>
                                    <li>1 SP</li>
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
                                    <li></li>
                                </ul>
                            </div>
                        </div>
                        
                        <div class="pricing-table class_inline">
                            <div class="pricing-header">
                                <p class="pricing-title">DO IT YOURSELF<br></p><br>  
                                <p style="font-size: 15px; color: #fff; text-align: center; ">Service Providers :
						            <select required style="width: 100px; margin-left: 7px;">				
						                <option>1 - $50                 
						                <option>2 - $100                
						                <option>3 - $150                
						                <option>4 - $200                
						                <option>5 - $250                
						                <option>10 - $400                
						                <option>15 - $525                
						                <option>20 - $600                
						        	</select> - One Time    
						        </p>
						        <p style="font-size: 15px; text-align: center;color: #fff; line-height: 0">+<br/><br/></p>
				                <p style="font-size: 15px; color: #fff; ">Users :
    								<select class="user_pricing" required style="width: 135px;">
    	    							<option selected>Upto 200 - $99
    		    						<option>Upto 400 - $199
    			    					<option>Upto 600 - $249
    				    				<option>Upto 800 - $299
    					    			<option>Upto 1000 - $349
    						    		<option>Upto 2000 - $449
    							    	<option>Upto 3000 - $549
    								    <option>Upto 4000 - $649
        								<option>Upto 5000 - $749
        								<option>5000+ Users - Contact Us
    	    				         </select> - One Time
	    						</p>
                                <p class="pricing-rate"></p>
                                <div class="filler-class"></div>
                                     <a class="btn btn-primary btn-sm" style="padding:5px;font-size: 15px;" '.$targetBlank.' href="'.$payment_page1.'">Click here to upgrade</a>*<br><br></h4>
                            </div>
                            <div class="pricing-list">
                                <ul>
                                    <li>Multiple SPs</li>
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
                                    <li></li> 
                                </ul>
                            </div>
                        </div>
                        
                        <div class="pricing-table class_inline">
                            <div class="pricing-header">
                                <p class="pricing-title">Premium<br></p><br>
                                <p style="font-size: 15px; color: #fff; text-align: center; ">Service Providers :
						            <select required style="width: 100px; margin-left: 7px;">				
						                <option>1 - $50                 
						                <option>2 - $100                
						                <option>3 - $150                
						                <option>4 - $200                
						                <option>5 - $250                
						                <option>10 - $400                
						                <option>15 - $525                
						                <option>20 - $600                
						        	</select> - One Time    
						        </p>
						        <p style="font-size: 15px; text-align: center;color: #fff; line-height: 0">+<br/><br/></p>
				                <p style="font-size: 15px; color: #fff; ">Users :
    								<select class="user_pricing" required style="width: 135px;">
    	    							<option selected>Upto 200 - $99
    		    						<option>Upto 400 - $199
    			    					<option>Upto 600 - $249
    				    				<option>Upto 800 - $299
    					    			<option>Upto 1000 - $349
    						    		<option>Upto 2000 - $449
    							    	<option>Upto 3000 - $549
    								    <option>Upto 4000 - $649
        								<option>Upto 5000 - $749
        								<option>5000+ Users - Contact Us
    	    				         </select> - One Time
	    						</p>
                                <p class="pricing-rate"></p>
                                 <a class="btn btn-primary btn-sm" style="padding:5px;font-size: 15px;" '.$targetBlank.' href="'.$payment_page2.'">Click here to upgrade</a>*<br><br></h4>
                            </div>
                            <div class="pricing-list">
                                <ul>                                  
                                    <li>Multiple SPs</li>
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
    </body>
    </html>',
     );


     $form['markup_5'] = array(
         '#markup' => '</b><br><div class="mo_saml_space"><h3>Steps to Upgrade to Premium Plugin</h3>'
             . '<ol>
                <li>You will be redirected to miniOrange Login Console. Enter your password with which you created an account with us. After that you will be redirected to payment page.</li>'
             . '<li>Enter you card details and complete the payment. On successful payment completion, you will see the link to download the premium module.</li>'
             . 'Once you download the premium module, just unzip it and replace the folder with existing module. Clear Drupal Cache.</li></ol><br>'
     );

     $form['markup_6'] = array(
         '#markup' => '<h3>** End to End Identity Provider Integration (additional charges applicable)</h3>'
             . ' We will setup a Conference Call / Gotomeeting and do end to end configuration for you to setup dupal as IDP.'
             . ' We provide services to do the configuration on your behalf.<br />'
             . ' If you have any doubts regarding the licensing plans, you can mail us at <a href="mailto:info@miniorange.com"><i>info@miniorange.com</i></a> or submit a query using the support form <b>(support form available on each tab)</b>.<br><br></div></div>'
     );

     $form['markup_8'] = array(
         '#markup' => '</div></div>'
     );

  return $form;
 }

/**
 * Send support query.
 */
function send_support_query(&$form, $form_state)
{
    $email = $form['miniorange_saml_email_address_support']['#value'];
    $phone = $form['miniorange_saml_phone_number_support']['#value'];
    $query = $form['miniorange_saml_support_query_support']['#value'];
    $support = new MiniOrangeSamlIdpSupport($email, $phone, $query);

    $support_response = $support->sendSupportQuery();
    if($support_response) {
        drupal_set_message(t('Your support query has been sent successfully. We will get back to you soon.'));
    }
    else {
        drupal_set_message(t('Error sending support query. Please try again.'), 'error');
    }
}