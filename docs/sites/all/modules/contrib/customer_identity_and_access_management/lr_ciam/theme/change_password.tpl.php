<?php

/**
 * @file
 * Change Password Template file.
 */
$ciam_api_key = trim(variable_get('lr_ciam_apikey'));
if (!empty($ciam_api_key)):
  ?>
  <script>
    jQuery(document).ready(function () {
      initializeChangePasswordCiamForms();
    });
  </script>
  <?php
      print theme('lr_message');      
    ?>
  
  <div class="my-form-wrapper">
    <div id="changepassword-container"></div>  
  </div>
<?php
endif;
