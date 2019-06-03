<div id="ae-bar" class="clearfix">

  <a href="#" id="ae-logo" title="Click here to visit the Dashboard"><img src="<?php echo $GLOBALS['base_path'] . path_to_theme(); ?>/theme/aelogo.png"></a>

  <?php print theme('ae_menu', array('sidebar' => FALSE, 'links' => menu_tree_all_data('ae-menu'))); ?>

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
  <?php print theme('ae_menu', array('sidebar' => TRUE, 'links' => menu_tree_all_data('ae-menu'), 'wide' => array('New'))); ?>
</div>

<div id="ae-widepopups">
  <?php print theme('ae_menu', array('widebar' => TRUE, 'links' => menu_tree_all_data('ae-menu'), 'wide' => array('New'))); ?>

  <div id="ae-menu-modal-hotkey-menu" title="AE bar modal hotkey menu">
    <ul class="big-icons">
      <li>
        <a href="">Front Page</a>
      </li>
      <!-- li>
        <a href="">Monitor</a>
      </li -->
      <li>
        <a href="">Manage</a>
      </li>
      <li>
        <a href="">New</a>
      </li>
    </ul>
    <ul class="last">
      <li>
        <a class="tooltipped" title="Want to leave?" href="/user/logout">Logout</a>
      </li>
      <li>
        <a class="tooltipped" href="http://advocacyengine.desk.com/" title="Visit our support center!">Help &amp; Support</a>
      </li>
      <li>
        <a href="">Settings</a>
      </li>
      <li>
        <a href="">Your Account</a>
      </li>
    </ul>
  </div>
</div>
