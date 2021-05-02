<?php

/**
 * @file
 * Displays an overlay containing a form.
 *
 * Available variables:
 *  - $introduction: A short introductory text,
 *  - $form: A renderable form array.
 *
 * @see campaignion_overlay_field_collection_item_view()
 *
 * @ingroup themeable
 */
?>
<div class="campaignion-overlay-options">
  <div class="campaignion-overlay-introduction">
    <?php echo render($introduction); ?>
  </div>
  <div class="campaignion-overlay-content">
    <?php echo render($content); ?>
  </div>
</div>
