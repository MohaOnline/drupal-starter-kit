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
<div class="<?php print $container_class; ?> panel-display berton clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['berton_top']): ?>
    <div class="region-berton-top">
      <?php print $content['berton_top']; ?>
    </div>
  <?php endif; ?>
  <div class="berton-highlights row">
    <?php if ($content['berton_highlight_1']): ?>
    <div class="region-berton-highlight-1 col-md-4">
        <?php print $content['berton_highlight_1']; ?>
    </div>
    <?php endif; ?>
    <?php if ($content['berton_highlight_2']): ?>
    <div class="region-berton-highlight-2 col-md-4">
        <?php print $content['berton_highlight_2']; ?>
    </div>
    <?php endif; ?>
    <?php if ($content['berton_highlight_3']): ?>
    <div class="region-berton-highlight-3 col-md-4">
        <?php print $content['berton_highlight_3']; ?>
    </div>
    <?php endif; ?>
  </div>
  <div class="berton-content-banners row">
    <?php if ($content['berton_content_banner']): ?>
    <div class="region-berton-content-banner col-md-8">
        <?php print $content['berton_content_banner']; ?>
    </div>
    <?php endif; ?>
    <?php if ($content['berton_content_listings']): ?>
    <div class="region-berton-content-listings col-md-4">
        <?php print $content['berton_content_listings']; ?>
    </div>
    <?php endif; ?>
  </div>
  <div class="berton-berton-spotlight row">
    <?php if ($content['berton_spotlight_1']): ?>
    <div class="region-berton-spotlight-1 col-md-4">
        <?php print $content['berton_spotlight_1']; ?>
    </div>
    <?php endif; ?>
    <?php if ($content['berton_spotlight_2']): ?>
    <div class="region-berton-spotlight-2 col-md-4">
        <?php print $content['berton_spotlight_2']; ?>
    </div>
    <?php endif; ?>
    <?php if ($content['berton_spotlight_3']): ?>
    <div class="region-berton-spotlight-3 col-md-4">
        <?php print $content['berton_spotlight_3']; ?>
    </div>
    <?php endif; ?>
  </div>
  <?php if ($content['berton_bottom']): ?>
    <div class="region-berton-bottom">
      <?php print $content['berton_bottom']; ?>
    </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
