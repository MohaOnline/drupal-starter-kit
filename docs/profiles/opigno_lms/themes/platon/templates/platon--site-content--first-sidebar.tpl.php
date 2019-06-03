<?php
  $settings = variable_get('theme_platon_settings');
  if(!empty($settings['palette'])) {
    $backgroundColor = $settings['palette']['dark_blue'];
  }
?>

<div id="first-sidebar">
  <?php if (!empty($main_navigation) && ($logged_in || theme_get_setting('platon_menu_show_for_anonymous')) && theme_get_setting('toggle_main_menu')): ?>
    <div id="main-navigation-wrapper">
      <?php
        $settings = theme_get_setting('platon_home_page_settings');
        global $base_url;
        (empty($settings)) ? $settings = variable_get('theme_platon_settings') : null;
        (drupal_is_front_page() && !$logged_in && $settings['platon_use_home_page_markup']) ? print('<div class="title">'. t('Menu') .'<div class="close-menu"><a href="#" class="open"><img src="'. $base_url .'/profiles/opigno_lms/themes/platon/img/homepage-close-menu.png"></a></div></div>') : null;
      ?>
      <?php if(drupal_is_front_page() && !$logged_in): ?>
        <!-- <div style="background-color: <?php (isset($backgroundColor) && !empty($backgroundColor)) ? print($backgroundColor) : print('#009ee0'); ?>"> -->
      <?php endif; ?>
      <?php print $main_navigation; ?>
      <?php if(drupal_is_front_page() && !$logged_in): ?>
        <!-- </div> -->
      <?php endif; ?>
    </div>
  <?php endif; ?>
  <?php print render($page['sidebar_first']); ?>
  <button class="trigger"><span></span></button>
</div>
