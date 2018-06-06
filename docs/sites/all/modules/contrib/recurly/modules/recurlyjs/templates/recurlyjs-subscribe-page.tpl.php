<?php
/**
 * @file
 * Print out the subscription page for a particular plan.
 */
drupal_add_css(drupal_get_path('module', 'recurlyjs') . '/css/recurlyjs.css');
?>
<div id="subscribe-page">
  <?php print drupal_render($form); ?>
</div>
