<header id="site-header"<?php print $site_header_attributes; ?>>
  <div class="row">

    

    <div class="col col-2-out-of-2 col-4-out-of-4 col-6-out-of-6 header-user-tools">

      <?php if (!empty($logo)): ?>
        <a href="<?php print $front_page; ?>" id="logo"><img src="<?php print $logo; ?>" alt="Opigno"></a>
      <?php endif; ?>

      <?php if ($logged_in): ?>
        <a href="<?php print url('user/logout'); ?>" class="mobile-link-icon">
          <img src="<?php print $base_path . $directory; ?>/img/logout-icon.png">
        </a>
      <?php endif; ?>

      <a href="<?php print url('search/node'); ?>" class="mobile-link-icon">
        <img src="<?php print $base_path . $directory; ?>/img/search-submit.png">
      </a>

      <a href="#top" id="menu-toggle-link" class="mobile-link-icon">
        <img src="<?php print $base_path . $directory; ?>/img/menu-toggle-icon.png">
      </a>

      <div id="header-login" class="hidden-mobile">
        <div class="link-block-user-login">
          <?php if ($logged_in): ?>
            <?php print l(t("logout"), 'user/logout'); ?>
          <?php else: ?>
            <a class="trigger-block-user-login"><?php print t("login"); ?></a>
          <?php endif; ?>
        </div>
        <?php print render($page['header_login']); ?>
      </div>

      <?php if (!empty($search_form)): ?>
        <div id="header-search">
          <?php print render($search_form); ?>
        </div>
      <?php endif; ?>

      <div id="user-account-information">
        <div id="user-links">
          <a href="<?php print url('user'); ?>">
            <img src="<?php print $base_path . $directory; ?>/img/anonymous-account.png">
          </a>
          <span class="welcome hidden-mobile"><?php print t("welcome @user", array('@user' => $logged_in ? $user->name : t("guest"))); ?></span>
        </div>
      </div>
    </div>
  </div>
</header>
