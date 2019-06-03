<?php

/**
 * @file
 * Default theme implementation to present login options.
 *
 * @see dvg-authentication-login-button.tpl.php
 *      where each login option is rendered.
 *
 * Available variables:
 * - $title: Title to show above the selection list
 * - $description: Additional description to show above the selection.
 * - $attributes: HTML attributes. Usually renders classes.
 *
 * @see template_preprocess_authentication_login_options()
 */
?>
<div class="dvgauth">
  <?php if (!empty($title)): ?>
      <h2><?php print $title; ?></h2>
  <?php endif; ?>
  <div<?php print $attributes; ?> class="dvgauth__items">
      <?php print render($login_options); ?>
  </div>
</div>
