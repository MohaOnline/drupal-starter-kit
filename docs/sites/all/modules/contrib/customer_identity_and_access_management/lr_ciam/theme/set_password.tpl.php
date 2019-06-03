<?php

/**
 * @file
 * Change Password Template file.
 */
$ciam_api_key = trim(variable_get('lr_ciam_apikey'));
if (!empty($ciam_api_key)):
      print theme('lr_message');      
    ?>
  
  <div class="my-form-wrapper">
   <div id="setpassword-container">
          <form name="loginradius-setpassword" method="POST">
              <div class="loginradius--form-element-content content-loginradius-newpassword">
                  <label for="loginradius-setpassword-newpassword">Password</label>
                  <input type="password" name="setnewpassword" id="loginradius-setpassword-newpassword" class="loginradius-password loginradius-newpassword lr-required"><div id="validation-loginradius-setpassword-newpassword" class="loginradius-validation-message validation-loginradius-newpassword"></div>
                      
              </div>
              <div class="loginradius--form-element-content content-loginradius-confirmnewpassword">
                  <label for="loginradius-setpassword-confirmnewpassword">Confirm Password</label>
                  <input type="password" name="setconfirmpassword" id="loginradius-setpassword-confirmnewpassword" class="loginradius-password loginradius-confirmnewpassword lr-required">
                  <div id="validation-loginradius-setpassword-confirmnewpassword" class="loginradius-validation-message validation-loginradius-confirmnewpassword"></div>
                     
              </div>
              <input type="submit" name="setpasswordsubmit" value="submit" id="loginradius-submit-submit" class="loginradius-submit submit-loginradius-submit">
          </form>
      </div> 
  </div>
<?php
endif;
