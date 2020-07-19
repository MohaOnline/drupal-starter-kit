<?php

use Drupal\miniorange_saml\MiniorangeSAMLConstants;

function miniorange_saml_registration($payment_plan){

    $status = variable_get('miniorange_saml_idp_status','');
    if ((isset($_POST['mo_otp_resend'])) && ($_POST['mo_otp_resend'] == "True")) {
        Utilities::saml_resend_otp(true);
    }
    elseif ((isset($_POST['mo_otp_check'])) && ($_POST['mo_otp_check'] == "True")) {
        $otp_token = trim($_POST['otp']);
        Utilities::validate_otp_submit($otp_token,true,$payment_plan);
    }
    elseif ((isset($_POST['mo_saml_check'])) && ($_POST['mo_saml_check'] == "True")) {
        $username = $_POST['Email'];
        $phone = '';
        $password = $_POST['password'];
        Utilities::customer_setup_submit($username, $phone, $password, true, $payment_plan);

    }
    elseif ($status == 'MOIDP_VALIDATE_OTP') {
        miniorange_otp(false,false,false);
    }
    else{
        register_data();
    }
}

function register_data($transaction_limit=false, $invalid_credential=false){
    global $base_url;
    $requestUrl = $base_url . '/admin/config/people/miniorange_saml_idp/licensing';
    $myArray = array();
    $myArray = $_POST;
    $form_id = isset($_POST['form_id'])?$_POST['form_id']:'';
    $form_token = isset($_POST['form_token'])?$_POST['form_token']:'';
    $admin_email = variable_get('miniorange_saml_idp_customer_admin_email', '');
    ?>

    <html>
    <head>
        <title>Register with miniOrange</title>
        <link href="https://fonts.googleapis.com/css?family=PT+Serif" rel="stylesheet">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
        <style>
            .saml_loader {
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
            .popup{
                background: #fff;
                padding: 4em 4em 2em;
                max-width: 400px;
                margin: 50px auto 0;
                box-shadow: 0 0 1em #222;
                border-radius: 2px;
            }
            .popup-header {
                margin:0 0 50px 0;
                padding:10px;
                text-align:center;
                font-size:20px;
                color:darkslategray;
                border-bottom:solid 1px #e5e5e5;
            }
            .popup-para {
                margin: 0 0 3em 0;
                position: relative;
            }
            .popup-input {
                display: block;
                box-sizing: border-box;
                width: 90%;
                outline: none;
                margin:0;
            }
            .popup-input[type="text"],
            .popup-input[type="password"],
            .popup-input[type="email"] {
                background: #fff;
                border: 1px solid #dbdbdb;
                font-size: 1.2em;
                padding: 6px 0 0 5px;
                border-radius: 4px;
            }
            .popup-input[type="text"]:focus,
            .popup-input[type="password"]:focus,
            .popup-input[type="email"]:focus {
                background: #fff
            }

            .popup-label{
                position: absolute;
                left: -4px;
                top: -25px;
                color: #999;
                font-size: 16px;
                display: inline-block;
                padding: 4px 10px;
                font-weight: 400;
                background-color: rgba(255,255,255,0);
            @include transition(color .3s, top .3s, background-color .8s);
            &.floatLabel{
                 top: -11px;
                 background-color: rgba(255,255,255,0.8);
                 font-size: 14px;
             }
            }

        </style>
        <script type="text/javascript">
            $(document).ready(function () {
                $("#myModal").modal({
                    backdrop: 'static',
                    keyboard: false
                });
                $('.button').click(function() {
                    document.getElementById('saml_loader').style.display = 'block';
                });
            });
            function check() {
                if (document.getElementById("password").value == document.getElementById("confirm_password").value) {
                    if (document.getElementById("password").value.length < 6) {
                        document.getElementById("saml_continue").disabled = true;
                        document.getElementById("message").innerHTML = "<p style=color:green;>Password match: <b style=color:green;>Yes</b></p>";
                        document.getElementById("password_error").innerHTML = "<p style=color:green;>Minimum Length: <b style=color:red;>6</b></p>";
                    }else{
                        document.getElementById("saml_continue").disabled = false;
                        document.getElementById("message").innerHTML = "<p style=color:green;>Password match: <b style=color:green;>Yes</b></p>";
                        document.getElementById("password_error").innerHTML = "<p></p>";
                    }

                } else {
                    if (document.getElementById("password").value.length < 6) {
                        document.getElementById("password_error").innerHTML = "<p style=color:green;>Minimum Length: <b style=color:red;>6</b></p>";
                    }else{
                        document.getElementById("password_error").innerHTML = "<p></p>";
                    }
                    document.getElementById("saml_continue").disabled = true;
                    document.getElementById("message").innerHTML = "<p style=color:green>Password match: <b style=color:red;>No</b></p>";
                }
                if (document.getElementById("Email").value.length == 0 || document.getElementById("password").value.length == 0 || document.getElementById("confirm_password").value.length == 0) {
                    document.getElementById("saml_continue").disabled = true;
                    document.getElementById("message").innerHTML = "<p></p>";
                }
            }
        </script>
    </head>
    <body>
    <div class="container">
        <div class="modal fade" id="myModal" role="dialog" style="background: rgba(0,0,0,0.1);">
            <div class="modal-dialog" style="width: 500px;">
                <div class="modal-content popup" style="border-radius: 20px;">
                    <?php if($transaction_limit == true){ ?>
                        <p style="color: red;font-size: 11px;">An error has been occured. Please try after some time.</p>
                    <?php }elseif($invalid_credential == true){ ?>
                        <p style="color: red;font-size: 11px;text-align: center;">Invalid Credentials!</p>
                    <?php }else { ?>
                        <p style="color: green;font-size: 11.2px;text-align: center;">You need to register with mini<span style="color:orange;"><b>O</b></span>range in order to upgrade to the licensed versions of this module.</p>
                    <?php } ?>
                    <h2 class="popup-header">Register/Login with mini<span style="color:orange;"><b>O</b></span>range</h2>


                    <form name="f" method="post" action="" id="mo_register">
                        <div>
                            <p class="popup-para">
                                <label for="Email" class="floatLabel popup-label">Email</label>
                                <input id="Email" name="Email" type="email" class="popup-input">
                            </p>
                            <p class="popup-para">
                                <label for="password" class="floatLabel popup-label">Password</label>
                                <input id="password" name="password" type="password" class="popup-input" onkeyup="check();">
                                <span id="password_error" style="float:left;font-size:11px;"></span>
                            </p>
                            <p class="popup-para">
                                <label for="confirm_password" class="floatLabel popup-label">Confirm Password</label>
                                <input id="confirm_password" name="confirm_password" type="password" class="popup-input" onkeyup="check();">
                                <span id="message" style="float:left;font-size:11px;"></span>
                            </p>

                            <br>
                            <input type="hidden" name="mo_saml_check" value="True">
                            <input type="hidden" name="form_token" value=<?php echo $form_token ?>>
                            <input type="hidden" name="form_id" value= <?php echo $form_id ?>>

                            <div class="modal-footer">
                                <a type="button" href=<?php echo $requestUrl ?> class="btn btn-default"  style="float:left;" >Close</a>
                                <input type="submit" class="btn btn-danger" id="saml_continue" disabled="disabled" value="Register" onclick="" /><br>
                                <div class="saml_loader" id="saml_loader" style="display: none;"></div>
                                <br>
                                <h6 style="text-align:center;">In case of any queries or issues, <br> please <a href="mailto:drupalsupport@xecurify.com"><strong>contact us</strong>.</a> </h6>
                            </div>
                            </p>
                        </div>
                    </form>

                </div>
            </div>
        </div>
    </div>
    </body>
    </html>

    <?php
    exit;
}

function miniorange_otp($wrong_otp=false, $resend_otp=false, $resend_limit=false){

    global $base_url;
    $close_url = $base_url . '/?q=close_registration';
    $myArray = array();
    $myArray = $_POST;
    $otp_form_id = isset($_POST['otp_form_id'])?$_POST['otp_form_id']:'';
    $otp_form_token = isset($_POST['otp_form_token'])?$_POST['otp_form_token']:'';
    $admin_email = variable_get('miniorange_saml_idp_customer_admin_email', '');
    ?>

    <html>
    <head>
        <title>Validate OTP</title>
        <link href="https://fonts.googleapis.com/css?family=PT+Serif" rel="stylesheet">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
        <style>
            .saml_loader {
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
            .popup{
                background: #fff;
                padding: 4em 4em 2em;
                max-width: 400px;
                margin: 50px auto 0;
                box-shadow: 0 0 1em #222;
                border-radius: 2px;
            }
            .popup-header {
                margin:0 0 50px 0;
                padding:10px;
                text-align:center;
                font-size:16px;
                color:darkslategray;
                border-bottom:solid 1px #e5e5e5;
            }
            .popup-para {
                position: relative;
            }
            .popup-input {
                display: block;
                box-sizing: border-box;
                width: 90%;
                outline: none;
                margin:0;
            }
            .popup-input[type="text"],
            .popup-input[type="password"] {
                background: #fff;
                border: 1px solid #dbdbdb;
                font-size: 1.2em;
                padding: 6px 0 0 5px;
                border-radius: 4px;
            }
            .popup-input[type="text"]:focus,
            .popup-input[type="password"]:focus {
                background: #fff
            }

            .popup-label{
                position: absolute;
                left: -4px;
                top: -25px;
                color: #999;
                font-size: 16px;
                display: inline-block;
                padding: 4px 10px;
                font-weight: 400;
                background-color: rgba(255,255,255,0);
            @include transition(color .3s, top .3s, background-color .8s);
            &.floatLabel{
                 top: -11px;
                 background-color: rgba(255,255,255,0.8);
                 font-size: 14px;
             }
            }

        </style>
        <script type="text/javascript">
            $(document).ready(function () {
                $("#myModal").modal({
                    backdrop: 'static',
                    keyboard: false
                });
                $('.button').click(function() {
                    document.getElementById('saml_loader').style.display = 'block';
                });
                if (document.getElementById("otp").value.length != 0) {
                    document.getElementById("validate").disabled = false;
                }else{
                    document.getElementById("validate").disabled = true;
                }
            });
            function check_empty(){
                if (document.getElementById("otp").value.length == 0) {
                    document.getElementById("validate").disabled = true;
                }else{
                    document.getElementById("validate").disabled = false;
                }
            }

        </script>
    </head>
    <body>
    <div class="container">
        <div class="modal fade" id="myModal" role="dialog" style="background: rgba(0,0,0,0.1);">
            <div class="modal-dialog" style="width: 500px;">
                <div class="modal-content popup" style="border-radius: 20px;">
                    <?php if ($resend_otp == true){ ?>
                        <h4 class="popup-header">An OTP has been resent to <?php echo $admin_email ?></h4>
                    <?php } ?>
                    <?php if ($resend_otp == false){ ?>
                        <h4 class="popup-header">Please enter the OTP sent to <?php echo $admin_email ?></h4>
                    <?php } ?>

                    <form name="ff" method="post" action="" id="mo_otp_verify">
                            <?php if ($wrong_otp == true) { ?>
                                <p class="popup-para">
                                    <label for="otp" class="floatLabel popup-label">Enter OTP</label>
                                    <input id="otp" name="otp" type="text" class="popup-input" style="border: 1px solid red !important;" onkeyup="check_empty();">
                                <p style="color: red;font-size: 11px;">Invalid OTP</p>
                                </p>
                            <?php } ?>
                            <?php if ($wrong_otp == false) { ?>
                                <p class="popup-para">
                                    <label for="otp" class="floatLabel popup-label">Enter OTP</label>
                                    <input id="otp" name="otp" type="text" class="popup-input" onkeyup="check_empty();">
                                </p>
                            <?php } ?>

                            <br>
                            <input type="hidden" name="mo_otp_check" value="True">
                            <input type="hidden" name="otp_form_token" value=<?php echo $otp_form_token ?>>
                            <input type="hidden" name="otp_form_id" value= <?php echo $otp_form_id ?>>

                            <div class="modal-footer">
                                <input type="submit" class="btn btn-danger" id="validate" value="Validate" style="float:left;" onclick="" />


                                <div class="saml_loader" id="saml_loader" style="display: none;"></div>

                    </form>
                    <form name="f" method="post" action="" id="mo_otp_resend">
                        <input type="hidden" name="mo_otp_resend" value="True">
                        <input type="submit" class="btn btn-danger" id="resend" value="Resend" style="float:left;margin-left: 24px;" onclick="" />
                        <a type="button" href="<?php echo $close_url; ?>" class="btn btn-default" >Close</a>
                    </form>
                </div>
            </div>
        </div>
    </div>
    </div>
    </div>
    </body>
    </html>

    <?php
    exit;
}
function miniorange_redirect_successfull($redirectUrl){
    global $base_url;
    $redirect = Utilities::getLicensePageURL();
    ?>
    <html>
    <head>
        <title> Redirecting to Xecurify</title>
    </head>
    <body>
    <script type="text/javascript">
        window.location="<?php echo $redirect; ?>";
        var x = window.open("<?php echo $redirectUrl; ?>","_blank" );
        if (x == null) {
            window.location = "<?php echo $redirectUrl; ?>";
        }
    </script>
    </body>
    </html>
    <?php

}
