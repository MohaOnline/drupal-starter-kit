<?php

/**
 * @file
 * Theme to present login buttons for the Authentication Manager.
 *
 * This template is used to loop through and render each login button
 * on the login selection page.
 *
 * @see dvg-authentication-login-options.tpl.php
 *      for the parent markup.
 *
 * Available variables:
 * - $link: Link to the login callback path.
 * - $description: Description to show above the link.
 * - $logo: Rendered image with the provider's logo.
 * - $title: Accessible title for the login blocks.
 * - $level_indicator: Rendered image with the provider's
 *                     level indicator (optional).
 * - $attributes: HTML attributes. Usually renders classes.
 *
 * @see template_preprocess_dvg_authentication_login_button()
 */
?>
<div <?php print $button_attributes?>>
  <?php if (!empty($title)): ?>
  <h3 class="dvgauth__title element-invisible"><?php print $title?></h3>
  <?php endif;?>
  <?php if (!empty($logo)): ?>
  <div class="dvgauth__logos">
    <?php print $logo; ?>
    <?php if (!empty($level_indicator)): ?>
      <?php print $level_indicator; ?>
    <?php endif; ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($description)): ?>
    <p class="dvgauth__description"><?php print $description; ?></p>
  <?php endif; ?>
  <?php print $link; ?>
</div>
