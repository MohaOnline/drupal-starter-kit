<?php
/**
 * @file
 * Output a list of invoices for a particular account. Typically displayed under
 * user/x/subscription/invoices.
 */
drupal_add_css(drupal_get_path('module', 'recurly') . '/css/recurly.css');
?>

<?php print theme('table', $table); ?>

<?php if ($total > $per_page): ?>
  <?php print theme('pager'); ?>
<?php endif; ?>
