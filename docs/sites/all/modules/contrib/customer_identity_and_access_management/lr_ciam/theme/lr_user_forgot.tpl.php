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
    <div class="item-list">
        <ul>
            <?php
             if(isset($register_link) && $register_link != '')
            {
                 ?>
            <li class="first"><a href="<?php print $register_link;?>" title="Create a new user account.">Create new account</a></li>
            <?php
            }
            if(isset($login_link) && $login_link != '')
            {
                ?>
<li class="last"><a href="<?php print $login_link;?>" title="Login with a user account">Login</a></li>
<?php
}
?>
</ul></div>
  </div>
<?php
endif;