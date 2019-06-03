<div id="auth_box" class="login">
  <div id="top_part">
      <?php if (isset($logo)): ?>
        <h1 id="the_logo">
          <a href="<?php print $front_page; ?>"><img src="<?php if (isset($svg_logo)): print $svg_logo; else: print $logo; endif ?>" /></a>
        </h1>
      <?php endif; ?>
  </div>

  <div id="middle_part">
    <h2 class="title"><?php print $title; ?></h2>

    <?php print $messages; ?>
    
    <?php print render($page['content']); ?>
  </div>

  <div id="bottom_part">
    <div class="password_link">
      <?php print l(t('Forgot your password?'), 'user/password'); ?>
    </div>

    <?php if (variable_get('user_register')): ?>
    <div class="register_link">
      <?php print l(t('Register a new account'), 'user/register'); ?>
    </div>
    <?php endif; ?>
  </div>
</div>
