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
<div class="<?php print $container_class; ?> panel-display gretzky clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['gretzky_top']): ?>
  <div class="region-gretzky-top ">
    <div class="region-inner clearfix">
      <?php print $content['gretzky_top']; ?>
    </div>
  </div>
  <?php endif; ?>
  <div class="row">
    <?php if ($content['gretzky_first']): ?>
    <div class="region-gretzky-first col-md-7">
      <div class="region-inner clearfix">
        <?php print $content['gretzky_first']; ?>
      </div>
    </div>
    <?php endif; ?>
    <?php if ($content['gretzky_second']): ?>
    <div class="region-gretzky-second col-md-5">
      <div class="region-inner clearfix">
        <?php print $content['gretzky_second']; ?>
      </div>
    </div>
    <?php endif; ?>
    </div>
  <?php if ($content['gretzky_bottom']): ?>
  <div class="region-gretzky-bottom ">
    <div class="region-inner clearfix">
      <?php print $content['gretzky_bottom']; ?>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
