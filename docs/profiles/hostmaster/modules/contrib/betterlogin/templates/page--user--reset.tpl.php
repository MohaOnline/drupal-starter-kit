<?php

/**
 * @file
 * User reset page - Template file.
 */
?>
<div id="auth_box" class="reset">
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

    <?php if (variable_get('user_register')): ?>
      <div class="register_link">
        <?php print l(t('Register a new account'), 'user/register'); ?>
      </div>
    <?php endif; ?>

    <div class="back_link">
      <a href="<?php print url('<front>'); ?>">&larr; <?php print t('Back Home'); ?> </a>
    </div>
  </div>
</div>
