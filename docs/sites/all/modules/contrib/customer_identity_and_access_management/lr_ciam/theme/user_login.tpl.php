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
      initializeLoginCiamForm();
      <?php
      if(!(isset($_GET['vtype']) && isset($_GET['vtoken']) && $_GET['vtype'] == 'emailverification')){
          ?>
        initializeSocialRegisterCiamForm(); 
      <?php 
      }
      ?>
       
        var isClear = 1;
          var formIntval;
        setTimeout(show_birthdate_date_block, 1000);
          formIntval = setInterval(function(){ jQuery('#lr-loading').hide();
             if (isClear > 0) {
                 clearInterval(formIntval);
             }
         }, 1000);
    });
  </script>

  <p><?php print $intro_text; ?></p>
  

  <div>
    <?php
    drupal_add_js(array('lrsociallogin' => $my_settings), 'setting');
    print theme('ciam_social_widget_container');
    ?>
  </div>
  <div class="ciam-lr-form my-form-wrapper">
    <div id="login-container"></div>
    <div id="resetpassword-container" style="display: block"></div>
    <?php
    print theme('lr_loading');
    print theme('lr_ciam_popup');
    ?>
  </div>
<?php
endif;
