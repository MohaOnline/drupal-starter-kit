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
<div class="<?php print $container_class; ?> panel-display laurier clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['laurier_top']): ?>
  <div class="row">
    <div class="col-md-12 region-laurier-top ">
      <?php print $content['laurier_top']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['laurier_content']): ?>
  <div class="row">
    <div class="col-md-12 region-laurier-first">
      <?php print $content['laurier_content']; ?>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['laurier_bottom']): ?>
  <div class="row">
    <div class="col-md-12 region-laurier-bottom ">
      <?php print $content['laurier_bottom']; ?>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
