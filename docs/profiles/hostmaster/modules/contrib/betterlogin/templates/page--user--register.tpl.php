<?php

/**
 * @file
 * User registration page - Template file.
 */
?>
<div id="auth_box" class="register">
  <div id="top_part">
    <h1 id="the_logo">
      <a href="<?php print url('<front>'); ?>">
        <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>">
      </a>
    </h1>
  </div>

  <div id="middle_part">
    <h2 class="title"><?php print $title; ?></h2>

    <?php print $messages; ?>
    
    <?php print render($page['content']); ?>
  </div>

  <div id="bottom_part">
    <div class="login_link">
      <?php print l(t('Login'), 'user/login'); ?>
    </div>

    <div class="password_link">
      <?php print l(t('Forgot your password?'), 'user/password'); ?>
    </div>

    <div class="back_link">
      <a href="<?php print url('<front>'); ?>">&larr; <?php print t('Back Home'); ?> </a>
    </div>
  </div>
</div>
