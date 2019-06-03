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
      initializeSocialRegisterCiamForm();      
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
  <div class="my-form-wrapper">
    <div id="login-container"></div>

    <?php
    print theme('lr_loading');
    print theme('lr_ciam_popup');
    ?>
    <div class="item-list">
        <ul>
             <?php
            if(isset($register_link) && $register_link != '')
            {
                ?>
            <li class="first"><a href="<?php print $register_link;?>" title="Create a new user account.">Create new account</a></li>
            <?php
            }
            if(isset($forgot_link) && $forgot_link != '')
            {
            ?>
<li class="last"><a href="<?php print $forgot_link;?>" title="Request new password via e-mail.">Request new password</a></li>
<?php
            }
            ?>
</ul></div>
  </div>
<?php
endif;
