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
<div class="<?php print $container_class; ?> panel-display polley clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['polley_top']): ?>
  <div class="region-polley-top">
    <div class="region-inner clearfix">
      <?php print $content['polley_top']; ?>
    </div>
  </div>
  <?php endif; ?>
  <div class="<?php print $container_class; ?>">
    <div class="row">
      <div class="region-polley-top-banner col-md-12">
        <div class="region-inner clearfix">
          <?php print $content['polley_top_banner']; ?>
        </div>
      </div>
    </div>
    <div class="row">
      <div class="region-polley-spotlight col-md-9">
        <div class="region-inner clearfix">
          <?php print $content['polley_spotlight']; ?>
        </div>
      </div>
      <div class="region-polley-content-listings col-md-3">
        <div class="region-inner clearfix">
          <?php print $content['polley_content_listings']; ?>
        </div>
      </div>
    </div>
    <div class="row">
      <div class="region-polley-bottom-banner col-md-12">
        <div class="region-inner clearfix">
          <?php print $content['polley_bottom_banner']; ?>
        </div>
      </div>
    </div>
  </div>
  <?php if ($content['polley_bottom']): ?>
  <div class="region-polley-bottom">
    <div class="region-inner clearfix">
      <?php print $content['polley_bottom']; ?>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
