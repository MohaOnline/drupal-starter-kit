<?php
if (user_is_logged_in()):
  drupal_goto('user');
endif;
$ciam_api_key = trim(variable_get('lr_ciam_apikey'));
if (!empty($ciam_api_key)):

  print theme('lr_message');
  ?>
  <script>
    jQuery(document).ready(function () {
      initializeForgotPasswordCiamForms();
    });
  </script>
  <div class="ciam-lr-form my-form-wrapper">
    <div id="forgotpassword-container"></div>
  </div>
<?php
endif;