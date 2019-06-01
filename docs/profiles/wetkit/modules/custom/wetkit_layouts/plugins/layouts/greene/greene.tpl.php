<?php
/**
 * @file
 * Implementation to present a Panels based layout.
 *
 * Available variables:
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout.
 * - $css_id: unique id if present.
 * - $panel_prefix: prints a wrapper when this template is used in a context,
 *   such as when rendered by Display Suite or other module.
 * - $panel_suffix: closing element for the $prefix.
 */
$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';
?>
<?php print $panel_prefix; ?>
<div class="<?php print $container_class; ?> panel-display greene clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['greene_top']): ?>
  <div class="region-greene-top ">
    <div class="region-inner clearfix">
      <?php print $content['greene_top']; ?>
    </div>
  </div>
  <?php endif; ?>
  <div class="row">
    <?php if ($content['greene_first']): ?>
    <div class="region-greene-first col-md-5">
      <div class="region-inner clearfix">
        <?php print $content['greene_first']; ?>
      </div>
    </div>
    <?php endif; ?>
    <?php if ($content['greene_second']): ?>
    <div class="region-greene-second col-md-7">
      <div class="region-inner clearfix">
        <?php print $content['greene_second']; ?>
      </div>
    </div>
    <?php endif; ?>
    </div>
  <?php if ($content['greene_bottom']): ?>
  <div class="region-greene-bottom ">
    <div class="region-inner clearfix">
      <?php print $content['greene_bottom']; ?>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
