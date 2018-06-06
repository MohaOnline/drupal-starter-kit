<?php
/**
 * @file
 * Display a signup page listing all the available plans.
 */
drupal_add_css(drupal_get_path('module', 'recurly') . '/css/recurly.css');
drupal_add_js(drupal_get_path('module', 'recurly') . '/js/recurly.js');
?>
<div class="recurly-signup">
  <div class="recurly-plan-list clearfix">
    <?php foreach ($filtered_plans as $plan): ?>
      <div class="plan plan-<?php print $plan['plan_code']; ?><?php print ($mode == 'change' && $plan['selected']) ? ' plan-selected' : '' ?>">
        <h2><?php print $plan['name']; ?></h2>
        <div class="plan-interval"><?php print $plan['plan_interval']; ?></div>
        <?php if ($plan['trial_interval']): ?>
          <div class="plan-trial"><?php print $plan['trial_interval']; ?></div>
        <?php endif; ?>
        <div class="plan-signup">
          <?php if ($mode == 'signup'): ?>
            <?php if ($plan['signup_url']): ?>
              <?php if ($plan['selected']): ?>
                <strong><?php print t('Selected'); ?></strong>
              <?php else: ?>
                <a class="plan-select" href="<?php print $plan['signup_url']; ?>"><?php print t('Sign up'); ?></a>
              <?php endif; ?>
            <?php else: ?>
              <?php print t('Contact us to sign up'); ?>
            <?php endif; ?>
          <?php else: ?>
            <?php if ($plan['selected']): ?>
              <strong><?php print t('Selected'); ?></strong>
            <?php else: ?>
              <a class="plan-select" href="<?php print $plan['change_url']; ?>"><?php print t('Select'); ?></a>
            <?php endif; ?>
          <?php endif; ?>
        </div>
        <div class="plan-description"><?php print nl2br($plan['description']); ?></div>
      </div>
    <?php endforeach; ?>
  </div>
</div>
