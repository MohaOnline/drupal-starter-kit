<?php ShareaholicAdmin::show_header(); ?>

<div id="shareaholic-form-container">
<ul class="nav nav-tabs">
  <li class="active"><?php print l(t('App Manager'), 'admin/config/shareaholic/settings'); ?></li>
  <li><?php print l(t('Advanced Settings'), 'admin/config/shareaholic/advanced'); ?></li>
</ul>
<div class='wrap'>

<div class='reveal-modal' id='editing_modal'>
  <div id='iframe_container' class='bg-loading-img' allowtransparency='true'></div>
  <a class="close-reveal-modal">&#215;</a>
</div>

<div class='unit size3of5'>
<?php
  $form = drupal_get_form('shareaholic_apps_configuration_form');
  print(drupal_render($form));
?>
</div>

  <div class="signuppromo unit size1of5">
  <p class="promoh1"><?php print t('Gain access to more features with a FREE Shareaholic account:'); ?></p>
  <ul>
    <li><?php print t('Floated Share Buttons'); ?></li>
    <li><?php print t('Follow Buttons'); ?></li>
    <li><?php print t('Revenue Generating Apps'); ?></li>
    <li><?php print t('Social Analytics, plus lots more!'); ?></li>
  </ul>
  <button data-href='edit' id='general_settings' class="btn btn-success btn-large"><?php print t('Configure additional features'); ?></button>
  <p class="signuppromo_note"><?php print t("Already have a Shareaholic account? Click the button above to log in."); ?></p>
  </div>

  <div class="help_links unit size1of5">
    <ul>
      <li><a href="http://support.shareaholic.com/" target="_blank"><?php print t('Need help? Visit the Shareaholic Helpdesk'); ?></a></li>
      <li><a href="https://localize.drupal.org/translate/downloads?project=shareaholic" target="_blank"><?php print t('Submit a new or updated language translation'); ?></a></li>
      <li><a href="https://shareaholic.com/tools/browser/" target="_blank"><?php print t('Get the Shareaholic Browser Extension to share content from anywhere on the web'); ?></a></li>
      <li><a href="http://support.shareaholic.com/hc/en-us/articles/201770175?utm_source=drupal_plugin&utm_medium=appsett&utm_campaign=psa_faq" target="_blank"><?php print t('Why am I seeing ads?'); ?></a></li>
    </ul>
  </div>


</div>

<?php ShareaholicAdmin::draw_modal_popup(); ?>
<?php ShareaholicAdmin::draw_verify_api_key(); ?>
<?php ShareaholicAdmin::show_footer(); ?>
<?php ShareaholicAdmin::include_snapengage(); ?>
</div>