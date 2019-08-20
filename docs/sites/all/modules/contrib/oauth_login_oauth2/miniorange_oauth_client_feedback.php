<?php
	function miniorange_oauth_client_feedback()
	{
		If( (isset($_POST['mo_oauth_client_check'])) && ($_POST['mo_oauth_client_check'] == "True" ) )
		{
			//code to send email alert
			$_SESSION['mo_other']="False";
			$reason=$_POST['deactivate_plugin'];
			$q_feedback=$_POST['query_feedback'];
			$message='Reason: '.$reason.'<br>Feedback: '.$q_feedback;
			$url = 'https://login.xecurify.com/moas/api/notify/send';
			$ch = curl_init($url);
			$email =variable_get('miniorange_oauth_client_customer_admin_email', '');
			$phone = variable_get('miniorange_oauth_client_customer_admin_phone','');
			$customerKey= variable_get('miniorange_oauth_client_customer_id', '');
			$apikey = variable_get('miniorange_oauth_client_customer_api_key', '');
			if($customerKey==''){
			$customerKey="16555";
			$apikey="fFd2XcvTGDemZvbw1bcUesNJWEqKbbUq";
			}
			$currentTimeInMillis = get_oauth_timestamp();
			$stringToHash 		= $customerKey .  $currentTimeInMillis . $apikey;
			$hashValue 			= hash("sha512", $stringToHash);
			$customerKeyHeader 	= "Customer-Key: " . $customerKey;
			$timestampHeader 	= "Timestamp: " .  $currentTimeInMillis;
			$authorizationHeader= "Authorization: " . $hashValue;
			$fromEmail 			= $email;
			$subject            = "Drupal 7 Login OAuth Client Plugin Feedback";
			$query        = '[Drupal 7 OAuth Login Client]: ' . $message;
			$content='<div >Hello, <br><br>Company :<a href="'.$_SERVER['SERVER_NAME'].'" target="_blank" >'.$_SERVER['SERVER_NAME'].'</a><br><br>Phone Number :'.$phone.'<br><br>Email :<a href="mailto:'.$fromEmail.'" target="_blank">'.$fromEmail.'</a><br><br>Query :'.$query.'</div>';
			$fields = array(
				'customerKey'	=> $customerKey,
				'sendEmail' 	=> true,
				'email' 		=> array(
					'customerKey' 	=> $customerKey,
					'fromEmail' 	=> $fromEmail,
					'fromName' 		=> 'miniOrange',
					'toEmail' 		=> 'drupalsupport@xecurify.com',
					'toName' 		=> 'drupalsupport@xecurify.com',
					'subject' 		=> $subject,
					'content' 		=> $content
				),
			);
			$field_string = json_encode($fields);
			curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
			curl_setopt( $ch, CURLOPT_ENCODING, "" );
			curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
			curl_setopt( $ch, CURLOPT_AUTOREFERER, true );
			curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );    # required for https urls
			curl_setopt( $ch, CURLOPT_MAXREDIRS, 10 );
			curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: application/json", $customerKeyHeader,
				$timestampHeader, $authorizationHeader));
			curl_setopt( $ch, CURLOPT_POST, true);
			curl_setopt( $ch, CURLOPT_POSTFIELDS, $field_string);
			$content = curl_exec($ch);
			if(curl_errno($ch)){
				return json_encode(array("status"=>'ERROR','statusMessage'=>curl_error($ch)));
			}
			curl_close($ch);
		}
		else
		{
            $_SESSION['mo_other']= "True";
			$myArray = array();
			$myArray = $_POST;
			$form_id=$_POST['form_id'];
			$form_token=$_POST['form_token'];

?>
			<html>
				<head>
            <title>Feedback</title>
            <link href="https://fonts.googleapis.com/css?family=PT+Serif" rel="stylesheet">
			</head>
			<body style="font-family: 'PT Serif', serif;">
			<h5 style="font-size:20px;color: black;margin-left:26%;margin-top:3%">Hey, it seems like you want to deactivate miniOrange OAuth Client Module</h5>
			<!-- The Modal -->
			<div id="myModal" style="margin-left:40%;margin-top: 7%"/>
			<!-- Modal content -->
			<div>
			<h3 style="font-size:42px;color: maroon"/>What Happened? </h3>

				<div style="padding:10px;">
					<div class="alert alert-info" style="margin-bottom:0px">
						<p style="font-size:15px"></p>
                    </div>
                 </div>
				<?php

				?>
			<form name="f" method="post" action="" id="mo_feedback">
			<div >
				<p style="margin-left:2%">
					<?php
						$deactivate_reasons = array
						(
							"Not Working",
							"Not Receiving OTP During Registration",
							"Does not have the features I'm looking for",
							"Redirecting back to login page after Authentication",
							"Confusing Interface",
							"Bugs in the plugin",
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
						<textarea id="query_feedback" name="query_feedback"  rows="4" style="margin-left:2%" cols="50" placeholder="Write your query here"></textarea>
						<br><br>
						<div class="mo2f_modal-footer">
							<input type="submit" style="background-color: #4CAF50;border: none;color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;" name="miniorange_feedback_submit" class="button button-primary button-large" value="Submit and Continue" />
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

	/**
	 * This function is used to get the timestamp value
	 */
	function get_oauth_timestamp() {
		$url = 'https://login.xecurify.com/moas/rest/mobile/get-timestamp';
		$ch  = curl_init( $url );
		curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
		curl_setopt( $ch, CURLOPT_ENCODING, "" );
		curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
		curl_setopt( $ch, CURLOPT_AUTOREFERER, true );
		curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );
		curl_setopt( $ch, CURLOPT_SSL_VERIFYHOST, false ); // required for https urls
		curl_setopt( $ch, CURLOPT_MAXREDIRS, 10 );
		curl_setopt( $ch, CURLOPT_POST, true );
		$content = curl_exec( $ch );
		if ( curl_errno( $ch ) ) {
			echo 'Error in sending curl Request';
			exit ();
		}
		curl_close( $ch );
		if(empty( $content )){
			$currentTimeInMillis = round( microtime( true ) * 1000 );
			$currentTimeInMillis = number_format( $currentTimeInMillis, 0, '', '' );
		}
		return empty( $content ) ? $currentTimeInMillis : $content;
	}
?>