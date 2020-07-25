<?php

/**
 * This function initiates the oauth sso flow
 */
function mo_oauth_client_initiateLogin() {
	global $base_url;
    $_SESSION['redirect_url'] = $_SERVER['HTTP_REFERER'];
	$app_name = variable_get('miniorange_auth_client_app_name','');
  	$client_id = variable_get('miniorange_auth_client_client_id','');
  	$client_secret = variable_get('miniorange_auth_client_client_secret','');
  	$scope = variable_get('miniorange_auth_client_scope','');
    $authorizationUrl = variable_get('miniorange_auth_client_authorize_endpoint','');
    $callback_uri = variable_get('miniorange_auth_client_callback_uri','');
    $state = base64_encode($app_name);
	if (strpos($authorizationUrl,'?') !== false) {
	$authorizationUrl =$authorizationUrl. "&client_id=".$client_id."&scope=".$scope."&redirect_uri=".$callback_uri."&response_type=code&state=".$state;
	}
	else {
		$authorizationUrl =$authorizationUrl. "?client_id=".$client_id."&scope=".$scope."&redirect_uri=".$callback_uri."&response_type=code&state=".$state;
	}
	$_SESSION['oauth2state'] = $state;
	$_SESSION['appname'] = $app_name;
    header('Location: ' . $authorizationUrl);
    drupal_goto($authorizationUrl);
}

/**
 * This function recieves the access token from the server.
 */
function getAccessToken($tokenendpoint, $grant_type, $clientid, $clientsecret, $code, $redirect_url, $send_headers, $send_body) {
   $ch = curl_init($tokenendpoint);
		curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
		curl_setopt( $ch, CURLOPT_ENCODING, "" );
		curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
		curl_setopt( $ch, CURLOPT_AUTOREFERER, true );
		curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );
		curl_setopt( $ch, CURLOPT_MAXREDIRS, 10 );
		curl_setopt( $ch, CURLOPT_POST, true);

    if($send_headers && !$send_body) {
        curl_setopt($ch, CURLOPT_HTTPHEADER, array(
            'Authorization: Basic ' . base64_encode( $clientid . ":" . $clientsecret ),
            'Accept: application/json'
        ));
        curl_setopt( $ch, CURLOPT_POSTFIELDS, 'redirect_uri='.urlencode($redirect_url).'&grant_type='.$grant_type.'&code='.$code);
    }else if(!$send_headers && $send_body){
        curl_setopt($ch, CURLOPT_HTTPHEADER, array(
            'Accept: application/json'
        ));
        curl_setopt( $ch, CURLOPT_POSTFIELDS, 'redirect_uri='.urlencode($redirect_url).'&grant_type='.$grant_type.'&client_id='.$clientid.'&client_secret='.$clientsecret.'&code='.$code);
    }else {
        curl_setopt($ch, CURLOPT_HTTPHEADER, array(
            'Authorization: Basic ' . base64_encode( $clientid . ":" . $clientsecret ),
            'Accept: application/json'
        ));
        curl_setopt( $ch, CURLOPT_POSTFIELDS, 'redirect_uri='.urlencode($redirect_url).'&grant_type='.$grant_type.'&client_id='.$clientid.'&client_secret='.$clientsecret.'&code='.$code);
    }
		$content = curl_exec($ch);
		if(curl_error($ch)){
			echo "<b>Response : </b><br>";print_r($content);echo "<br><br>";
			exit( curl_error($ch) );
		}
		if(!is_array(json_decode($content, true))){
			echo "<b>Response : </b><br>";print_r($content);echo "<br><br>";
			exit("Invalid response received.");
		}
		$content = json_decode($content,true);

        if (isset($content["error"])) {
            if (is_array($content["error"])) {
                $content["error"] = $content["error"]["message"];
            }
            exit($content["error"]);
        }
        else if(isset($content["error_description"])){
            exit($content["error_description"]);
        } else if(isset($content["access_token"])) {
			$access_token = $content["access_token"];
		} else {
			echo "<b>Response : </b><br>";print_r($content);echo "<br><br>";
			exit('Invalid response received from OAuth Provider. Contact your administrator for more details.');
		}
		return $access_token;
	}

	/**
	 * This function recieves the user resources from the server.
	 */
    function getResourceOwner($resourceownerdetailsurl, $access_token){
		$ch = curl_init($resourceownerdetailsurl);
		curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
		curl_setopt( $ch, CURLOPT_ENCODING, "" );
		curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
		curl_setopt( $ch, CURLOPT_AUTOREFERER, true );
		curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );
		curl_setopt( $ch, CURLOPT_MAXREDIRS, 10 );
		curl_setopt( $ch, CURLOPT_POST, false);
		curl_setopt($ch, CURLOPT_HTTPHEADER, array(
				'Authorization: Bearer '.$access_token
			)
		);
        $t_vers = curl_version();
        curl_setopt( $ch, CURLOPT_USERAGENT, 'curl/' . $t_vers['version']);
		$content = curl_exec($ch);
		if(curl_error($ch)){
			echo "<b>Response : </b><br>";print_r($content);echo "<br><br>";
			exit( curl_error($ch) );
		}
		if(!is_array(json_decode($content, true))) {
			echo "<b>Response : </b><br>";print_r($content);echo "<br><br>";
			exit("Invalid response received.");
		}
		$content = json_decode($content,true);
		if(isset($content["error_description"])){
			if(is_array($content["error_description"]))
				print_r($content["error_description"]);
			else
				echo $content["error_description"];
			exit;
		} else if(isset($content["error"])){
			if(is_array($content["error"]))
				print_r($content["error"]);
			else
				echo $content["error"];
			exit;
		}
		return $content;
	}
?>