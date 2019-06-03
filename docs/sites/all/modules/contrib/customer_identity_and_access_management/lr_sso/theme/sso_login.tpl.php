<?php
/**
 * @file
 * sso login
 */
  print theme('lr_message');
  ?>
  <script>
    jQuery(document).ready(function () {
      initializeSocialRegisterCiamForm();
    });
  </script>

  <div class="ciam-lr-form my-form-wrapper">
    <?php
    print theme('lr_loading');
    print theme('lr_ciam_popup');
    ?>
  </div>
