<div class='reveal-modal blocking-modal api-key-modal' id='failed_to_create_api_key'>
  <h4><?php print t('Setup Shareaholic') ?></h4>
  <div class="content pal">
  <div class="line pvl">
    <div class="unit size3of3">
      <p>
        <?php print t('It appears that we are having some trouble setting up Shareaholic for Drupal right now. This is usually temporary. Please revisit this section after a few minutes or click "retry" now.'); ?>
      </p>
    </div>
  </div>
  <div class="pvl">
    <?php print $variables['shareaholic_failure_modal']['hidden'] ?>
    <?php print $variables['shareaholic_failure_modal']['submit'] ?>
    <br /><br />
    <?php print l(t('or, try again later'), 'admin', array('attributes' => array('style' => 'font-size:12px; font-weight:normal;'))); ?>
    <br /><br />
    <span style="font-size:11px; font-weight:normal;">
      <?php echo sprintf(t('If you continue to get this prompt for more than a few hours, try to check server connectivity or reset the module in %s'), l(t('advanced settings.'), 'admin/config/shareaholic/advanced')); ?> <?php echo sprintf(t('Also, if you have a question or have a bug to report, please %slet us know%s.'), '<a href="#" onclick="SnapEngage.startLink();">','</a>'); ?>
    </span>
  </div>
  </div>
</div>
