<?php
/**
 * @file
 * Print the output of the subscription cancelation form.
 */
?>
<?php if ($form['terminate']['#access'] && recurly_subscription_in_trial($subscription)): ?>
  <div class="messages warning"><?php print t('This plan is currently in a trial, so no refund will be issued if terminated.'); ?></div>
<?php elseif ($form['terminate']['#access'] && $past_due): ?>
  <div class="messages warning"><?php print t('Even though this account is past due, canceling the account will not close it immediately. Terminate the account to close it immediately.'); ?></div>
<?php endif; ?>

<?php if ($form['cancel']['#access']): ?>
  <h2><?php print t('Cancel at Renewal'); ?></h2>
  <?php print drupal_render($form['cancel']); ?>
<?php endif; ?>

<?php if ($form['terminate']['#access']): ?>
  <?php if ($form['terminate']['refund_amount']['#access']): ?>
  <h2><?php print t('Terminate Immediately'); ?></h2>
  <?php endif; ?>

  <?php print drupal_render($form['terminate']); ?>
<?php endif; ?>

<?php print drupal_render_children($form); ?>
