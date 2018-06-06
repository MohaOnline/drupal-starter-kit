<?php
/**
 * @file
 * Displays the form for updating a credit card number for a subscription.
 */
?>

<?php if (isset($form['existing']['#last_four'])): ?>
  <p><?php print t('We currently have the following credit card on file:'); ?></p>
  <p><?php print drupal_render($form['existing']); ?></p>
  <p><?php print t('To change your credit card, update your information below:'); ?></p>
<?php else: ?>
  <p><?php print t('No credit card currently on file.'); ?></p>
<?php endif; ?>

<?php print drupal_render_children($form); ?>
