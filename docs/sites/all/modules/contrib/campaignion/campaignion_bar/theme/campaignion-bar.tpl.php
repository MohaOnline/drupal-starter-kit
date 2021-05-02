<div id="ae-bar" class="clearfix">

  <a href="#" id="ae-logo" title="Click here to visit the Dashboard"><img src="https://ucarecdn.com/51febc91-6f3f-4f75-9d03-b5a71d564cf3/ae_admin_impacstack_logocopy.png"></a>

  <?php print theme('campaignion_bar_menu', array('sidebar' => FALSE, 'links' => menu_tree_all_data('ae-menu'))); ?>

  <ul id="ae-sidemenu">

    <li id="ae-sidemenu-for-account"><a href="/user">Your account</a></li>
    <li id="ae-sidemenu-for-help"><a class="tooltipped" title="Get Support or view the documentation" href="<?php print variable_get('campaignion_support_link', 'https://www.campaignion.org/campaignion-support'); ?>">Help & support</a></li>
    <li id="ae-sidemenu-for-logout"><a class="tooltipped" title="Want to leave?" href="/user/logout">Logout</a></li>
    <li id="ae-menu-hide"><a class="tooltipped" title="Hide this toolbar" href="#toggle">Hide the toolbar</a></li>

  </ul>
</div>

<div id="ae-menu-show">
  <a class="tooltipped" title="Show the toolbar" href="#">Show</a>
</div>

<div id="ae-popups">
  <?php print theme('campaignion_bar_menu', array('sidebar' => TRUE, 'links' => menu_tree_all_data('ae-menu'), 'wide' => array('New'))); ?>
</div>

<div id="ae-widepopups">
  <?php print theme('campaignion_bar_menu', array('widebar' => TRUE, 'links' => menu_tree_all_data('ae-menu'), 'wide' => array('New'))); ?>
</div>
