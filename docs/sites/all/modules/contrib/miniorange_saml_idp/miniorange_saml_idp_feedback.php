<?php
function miniorange_idp_feedback()
{
    If( (isset($_POST['mo_idp_check'])) && ($_POST['mo_idp_check'] == "True" ) && ($_SESSION['mo_other'] == "True") )
    {
        //code to send email alert
        unset($_SESSION['mo_other']);
        $reason = $_POST['deactivate_plugin'];
        $q_feedback = $_POST['query_feedback'];
        $email = $_POST['query_feedback_email'];
        $message = '<br><b>Reason: </b>'.$reason.'<br><br><b>Feedback:</b> '.$q_feedback;

        $url = 'https://auth.miniorange.com/moas/api/notify/send';
        $ch = curl_init($url);
        $phone = variable_get('miniorange_saml_idp_customer_admin_phone','');
        $customerKey= variable_get('miniorange_saml_idp_customer_id', '');
        $apikey = variable_get('miniorange_saml_idp_customer_api_key', '');

        if(valid_email_address($email)) {

            if ($customerKey == '') {
                $customerKey = "16555";
                $apikey = "fFd2XcvTGDemZvbw1bcUesNJWEqKbbUq";
            }

            $currentTimeInMillis = get_idp_timestamp();
            $stringToHash = $customerKey . $currentTimeInMillis . $apikey;
            $hashValue = hash("sha512", $stringToHash);
            $customerKeyHeader = "Customer-Key: " . $customerKey;
            $timestampHeader = "Timestamp: " . $currentTimeInMillis;
            $authorizationHeader = "Authorization: " . $hashValue;
            $fromEmail = $email;
            $query = '[Drupal-7 SAML IDP Free] ' . $message;

            $content = '<div >Hello, <br><br>Company :<a href="' . $_SERVER['SERVER_NAME'] . '" target="_blank" >' . $_SERVER['SERVER_NAME'] . '</a>
                                   <br><br>Phone Number :' . $phone . '<br><br>Email :<a href="mailto:' . $fromEmail . '" target="_blank">' . $fromEmail . '</a>
                                   <br><br><b>Query:</b> ' . $query . '</div>';

            $fields = array(
                'customerKey' => $customerKey,
                'sendEmail' => true,
                'email' => array(
                    'customerKey' => $customerKey,
                    'fromEmail' => $fromEmail,
                    'fromName' => 'miniOrange',
                    'toEmail' => 'drupalsupport@miniorange.com',
                    'toName' => 'drupalsupport@miniorange.com',
                    'subject' => 'Drupal-7 SAML IDP Module Feedback',
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
            curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: application/json", $customerKeyHeader, $timestampHeader, $authorizationHeader));
            curl_setopt($ch, CURLOPT_POST, true);
            curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
            $content = curl_exec($ch);

            if (curl_errno($ch)) {
                return json_encode(array("status" => 'ERROR', 'statusMessage' => curl_error($ch)));
            }
            curl_close($ch);
        }
    }
    else if($_SESSION['mo_other'] == "False")
    {
        $_SESSION['mo_other']= "True";
        $myArray = array();
        $myArray = $_POST;
        $form_id=$_POST['form_id'];
        $form_token=$_POST['form_token'];
        $admin_email = variable_get('miniorange_saml_idp_customer_admin_email', '');
        ?>
        <html>
        <head>
            <title>Feedback</title>
            <link href="https://fonts.googleapis.com/css?family=PT+Serif" rel="stylesheet">
        </head>
        <body style="font-family: 'PT Serif', serif;">
        <h5 style="font-size:20px; color: black; text-align: center; margin-top:10px">Hey, it seems like you want to deactivate MiniOrange SAML IDP Module</h5>
        <h3 style="font-size:42px; margin-top: -20px; text-align: center; color: maroon">What Happened? </h3>
        <div style="margin-left: 38%; margin-top: -20px;">
            <form name="f" method="post" action="" id="mo_feedback">
                <div style="width: 50%; text-align: justify-all">
                    <?php
                    $deactivate_reasons = array
                    (
                        "Not Working",
                        "Bugs in the plugin",
                        "Confusing Interface",
                        "Not Receiving OTP During Registration",
                        "Does not have the features I'm looking for",
                        "Redirecting back to login page after Authentication",
                        "Other Reasons:"
                    );
                    foreach ( $deactivate_reasons as $deactivate_reasons ) {
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
                    <input type="hidden" name="mo_idp_check" value="True">
                    <input type="hidden" name="form_token" value=<?php echo $form_token ?> >
                    <input type="hidden" name="form_id" value= <?php echo $form_id ?>>

                    <br>
                    <input id="query_feedback_email" name="query_feedback_email" value= "<?php echo $admin_email?>" style="width: 67%" placeholder="Enter your Email address"/>
                    <br><br>
                    <textarea id="query_feedback" name="query_feedback"  rows="4" cols="50" placeholder="Write your query here"></textarea>
                    <br><br>
                    <div class="mo2f_modal-footer">
                        <input type="submit" style="background-color: #4CAF50;border: none;color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;" name="miniorange_feedback_submit" class="button button-primary button-large" value="Submit and Continue" />
                    </div>
                    <?php
                    echo "<br><br>";
                    foreach($_POST as $key => $value) {
                        hiddenIDPFields($key,$value);
                    }
                    ?>
                </div>
            </form>
        </div>
        </body>
        </html>

        <?php
        exit;
    }
}

function get_idp_timestamp() {
    $url = 'https://auth.miniorange.com/moas/rest/mobile/get-timestamp';
    $ch  = curl_init( $url );

    curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
    curl_setopt( $ch, CURLOPT_ENCODING, "" );
    curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
    curl_setopt( $ch, CURLOPT_AUTOREFERER, true );
    curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );
    curl_setopt( $ch, CURLOPT_SSL_VERIFYHOST, false ); // required for https urls

    curl_setopt( $ch, CURLOPT_MAXREDIRS, 10 );

    curl_setopt( $ch, CURLOPT_POST, true );

    if ( defined( 'WP_PROXY_HOST' ) && defined( 'WP_PROXY_PORT' ) && defined( 'WP_PROXY_USERNAME' ) && defined( 'WP_PROXY_PASSWORD' ) ) {
        curl_setopt( $ch, CURLOPT_PROXY, WP_PROXY_HOST );
        curl_setopt( $ch, CURLOPT_PROXYPORT, WP_PROXY_PORT );
        curl_setopt( $ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC );
        curl_setopt( $ch, CURLOPT_PROXYUSERPWD, WP_PROXY_USERNAME . ':' . WP_PROXY_PASSWORD );
    }

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

function hiddenIDPFields($key,$value)
{
    $hiddenIDPField = "";
    $value2 = array();
    if(is_array($value)) {
        foreach($value as $key2 => $value2)
        {
            if(is_array($value2)){
                hiddenIDPFields($key."[".$key2."]",$value2);
            } else {
                $hiddenIDPField = "<input type='hidden' name='".$key."[".$key2."]"."' value='".$value2."'>";
            }
        }
    }else{
        $hiddenIDPField = "<input type='hidden' name='".$key."' value='".$value."'>";
    }

    echo $hiddenIDPField;
}
?>