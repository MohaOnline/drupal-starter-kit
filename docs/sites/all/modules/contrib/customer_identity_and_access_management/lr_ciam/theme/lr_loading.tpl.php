<?php
drupal_add_css((drupal_get_path('module', 'lr_ciam') .'/css/lr_loading.min.css'));
$loading_image = $GLOBALS['base_url'] . '/' . drupal_get_path('module', 'lr_ciam') . '/images/loading-white.png';
?>
<div class="overlay" id="lr-loading" style="display: none;">
  <div class="circle">
    <div id="imganimation">
<!--        <div style="position: fixed;top: -500%;left: -500%;right: -500%;bottom: -500%;z-index: 9999;background: linear-gradient(to bottom, #f9f9f9 10%, #eeeff3 100%);">
            <div class="lr_loading_screen">
                <div class="lr_loading_screen_center" style="position: fixed;">-->
                    <img src="<?php print $loading_image; ?>" alt="LoginRadius Processing" class="lr_loading_screen_spinner">
<!--                </div>
            </div>
        </div>-->
<!--      <img src="<?php print $loading_image; ?>" alt="LoginRadius Processing"
           style="margin-top: -66px;margin-left: -73px;width: 150px;" class="lr_loading_screen_spinner">-->
    </div>
  </div>
  <div></div>
</div>
