<?php
	function miniorange_oauth_client_feedback()
	{

        $modules_info = system_get_info('module', 'oauth_login_oauth2');
        $modules_version = $modules_info['version'];

		If( (isset($_POST['mo_oauth_client_check'])) && ($_POST['mo_oauth_client_check'] == "True" ) ) {
            //code to send email alert
            $_SESSION['mo_other'] = "False";
            $reason = $_POST['deactivate_plugin'];
            $q_feedback = $_POST['query_feedback'];
            $message = 'Reason: ' . $reason . '<br>Feedback: ' . $q_feedback;
            $url = 'https://login.xecurify.com/moas/api/notify/send';
            $ch = curl_init($url);
            $admin_email = variable_get('miniorange_oauth_client_customer_admin_email', '');
            if (empty($admin_email))
                $email = $_POST['miniorange_feedback_email'];
            else
                $email = $admin_email;

            if (valid_email_address($email)) {
                $phone = variable_get('miniorange_oauth_client_customer_admin_phone', '');
                $customerKey = variable_get('miniorange_oauth_client_customer_id', '');
                $apikey = variable_get('miniorange_oauth_client_customer_api_key', '');
                if ($customerKey == '') {
                    $customerKey = "16555";
                    $apikey = "fFd2XcvTGDemZvbw1bcUesNJWEqKbbUq";
                }
                $currentTimeInMillis = Utilities::get_oauth_timestamp();
                $stringToHash = $customerKey . $currentTimeInMillis . $apikey;
                $hashValue = hash("sha512", $stringToHash);
                $customerKeyHeader = "Customer-Key: " . $customerKey;
                $timestampHeader = "Timestamp: " . $currentTimeInMillis;
                $authorizationHeader = "Authorization: " . $hashValue;
                $fromEmail = $email;
                $subject = "Drupal 7 OAuth Login Module Feedback | " .$modules_version;
                $query = '[Drupal 7 OAuth Login Client | '.$modules_version.' ]: ' . $message;
                $content = '<div >Hello, <br><br>Company :<a href="' . $_SERVER['SERVER_NAME'] . '" target="_blank" >' . $_SERVER['SERVER_NAME'] . '</a><br><br>Phone Number :' . $phone . '<br><br>Email :<a href="mailto:' . $fromEmail . '" target="_blank">' . $fromEmail . '</a><br><br>Query :' . $query . '</div>';
                $fields = array(
                    'customerKey' => $customerKey,
                    'sendEmail' => true,
                    'email' => array(
                        'customerKey' => $customerKey,
                        'fromEmail' => $fromEmail,
                        'fromName' => 'miniOrange',
                        'toEmail' => 'drupalsupport@xecurify.com',
                        'toName' => 'drupalsupport@xecurify.com',
                        'subject' => $subject,
                        'content' => $content
                    ),
                );
                $field_string = json_encode($fields);
                curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
                curl_setopt($ch, CURLOPT_ENCODING, "");
                curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
                curl_setopt($ch, CURLOPT_AUTOREFERER, true);
                curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);    # required for https urls
                curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
                curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json', $customerKeyHeader,
                    $timestampHeader, $authorizationHeader));
                curl_setopt($ch, CURLOPT_POST, true);
                curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
                $content = curl_exec($ch);
                if (curl_errno($ch)) {
                    return json_encode(array("status" => 'ERROR', 'statusMessage' => curl_error($ch)));
                }
                curl_close($ch);
            }
        } else{
            unset($_SESSION['mo_other']);
            $myArray = array();
            $myArray = $_POST;
            $form_id = $_POST['form_id'];
            $form_token = $_POST['form_token'];
            $admin_email = variable_get('miniorange_oauth_client_customer_admin_email', ''); ?>

			<html>
				<head>
            <title>Feedback</title>
            <link href="https://fonts.googleapis.com/css?family=PT+Serif" rel="stylesheet">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
            <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
            <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>

            <style>
                .oauth_loader {
                    margin: auto;
                    display: block;
                    border: 5px solid #f3f3f3; /* Light grey */
                    border-top: 5px solid #3498db; /* Blue */
                    border-radius: 50%;
                    width: 50px;
                    height: 50px;
                    animation: spin 2s linear infinite;
                }
                @keyframes spin {
                    0% { transform: rotate(0deg); }
                    100% { transform: rotate(360deg); }
                }
            </style>
                    <script type="text/javascript">
                        $(document).ready(function () {
                            if(document.getElementById('miniorange_feedback_email').value == '') {
                                document.getElementById('email_error').style.display = "block";
                                document.getElementById('submit_button').disabled = true;
                            }
                            $("#myModal").modal({
                                backdrop: 'static',
                                keyboard: false
                            });
                            $('.button').click(function() {
                                document.getElementById('oauth_loader').style.display = 'block';
                            });
                        });

                        function validateEmail(emailField) {
                            var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;

                            if (reg.test(emailField.value) === false) {
                                document.getElementById('email_error').style.display = "block";
                                document.getElementById('submit_button').disabled = true;
                            } else {
                                document.getElementById('email_error').style.display = "none";
                                document.getElementById('submit_button').disabled = false;
                            }
                        }
                    </script>
			</head>
            <body>
                    <div class="modal-dialog" style="width: 500px;">
                        <div class="modal-content" style="border-radius: 20px;">
                            <div class="modal-header"
                                 style="padding: 25px; border-top-left-radius: 20px; border-top-right-radius: 20px; background-color: #8fc1e3;">
                                <h4 class="modal-title" style="color: white; text-align: center;">
                                    Hey, it seems like you want to deactivate miniOrange Oauth Client Login module
                                </h4>
                                <hr>
                                <h4 style="text-align: center; color: white;">What happened?</h4>
                            </div>
                            <div class="modal-body"
                                 style="font-size: 11px; padding-left: 25px; padding-right: 25px; border-bottom-left-radius: 20px; border-bottom-right-radius: 20px; background-color: #ececec;">
				<form name="f" method="post" action="" id="mo_feedback">
			<div>
				<p style="margin-left:0%">
				<?php
                if (empty($admin_email)) { ?>
                    <br>Email: <input onblur="validateEmail(this)" class="form-control" type="email"
                                      id="miniorange_feedback_email"
                                      name="miniorange_feedback_email" required/>
                <p style="display: none;color:red" id="email_error">Invalid Email</p>
                <?php } ?>
                <br>
                <?php
						$deactivate_reasons = array
						(
							"Not Working",
							"Not Receiving OTP During Registration",
							"Does not have the features I'm looking for",
							"Redirecting back to login page after Authentication",
							"Confusing Interface",
							"Bugs in the module",
							"Other Reasons:"
						);
						foreach ( $deactivate_reasons as $deactivate_reasons )
						{
							?>
							<div  class="radio" style="padding:2px;font-size: 8px">
								<label style="font-weight:normal;font-size:14.6px;color:maroon" for="<?php echo $deactivate_reasons; ?>">

								<input type="radio" name="deactivate_plugin" value="<?php echo $deactivate_reasons;?>" required>
								<?php echo $deactivate_reasons;

									?>

								</label>
							</div>

						<?php

						}
							?>
							<input type="hidden" name="mo_oauth_client_check" value="True">
							<input type="hidden" name="form_token" value=<?php echo $form_token ?> >
							<input type="hidden" name="form_id" value= <?php echo $form_id ?>>

						<br>
						<textarea id="query_feedback" name="query_feedback"  rows="4" style="margin-left:2%; font-size: medium;" cols="50" placeholder="Write your query here"></textarea>
						<br><br>
						<div class="mo2f_modal-footer">
							<input type="submit" style="background-color: #4CAF50;border: none;color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;" name="miniorange_feedback_submit" class="button button-primary button-large" value="Submit and Continue" />
                            <div class="oauth_loader" id="oauth_loader" style="display: none;"></div>
						</div>
						<?php
							echo "<br><br>";
							foreach($_POST as $key => $value) {
								hiddenOauthClientFields($key,$value);
							}
						?>
					</div>
			</form>
			</body>
			</html>

			<?php
			exit;
		}
	}


	function hiddenOauthClientFields($key,$value)
	{
		$hiddenOauthClientField = "";
        $value2 = array();
        if(is_array($value)) {
            foreach($value as $key2 => $value2)
            {
                if(is_array($value2)){
                    hiddenOauthClientFields($key."[".$key2."]",$value2);
                } else {
                    $hiddenOauthClientField = "<input type='hidden' name='".$key."[".$key2."]"."' value='".$value2."'>";
                }
            }
        }else{
            $hiddenOauthClientField = "<input type='hidden' name='".$key."' value='".$value."'>";
        }

        echo $hiddenOauthClientField;
    }
?>
