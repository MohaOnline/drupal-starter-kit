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
<div class="<?php print $container_class; ?> panel-display hadfield clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['hadfield_top']): ?>
  <div class="region-hadfield-top ">
    <div class="region-inner clearfix">
      <?php print $content['hadfield_top']; ?>
    </div>
  </div>
  <?php endif; ?>
    <div class="row">
      <?php if ($content['hadfield_first']): ?>
      <div class="col-md-4 region-hadfield-first">
        <div class="region-inner clearfix">
          <?php print $content['hadfield_first']; ?>
        </div>
      </div>
      <?php endif; ?>
      <?php if ($content['hadfield_second']): ?>
      <div class="col-md-8 region-hadfield-second">
        <div class="region-inner clearfix">
          <?php print $content['hadfield_second']; ?>
        </div>
      </div>
      <?php endif; ?>
    </div>
  <?php if ($content['hadfield_bottom']): ?>
  <div class="region-hadfield-bottom ">
    <div class="region-inner clearfix">
      <?php print $content['hadfield_bottom']; ?>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
