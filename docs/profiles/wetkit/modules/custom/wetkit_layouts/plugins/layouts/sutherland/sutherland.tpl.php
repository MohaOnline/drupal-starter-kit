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
<div class="panel-display sutherland clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['sutherland_top']): ?>
  <aside class="prm-flpr">
    <div class="<?php print $container_class; ?>">
      <div class="row">
        <div class="region-sutherland-top">
          <div class="region-inner clearfix">
          <?php print $content['sutherland_top']; ?>
          </div>
        </div>
      </div>
    </div>
  </aside>
  <?php endif; ?>
  <?php if ($content['sutherland_top_inner']): ?>
  <div class="<?php print $container_class; ?>">
    <div class="row">
      <div class="region-sutherland-top-inner col-md-12">
        <div class="region-inner clearfix">
        <?php print $content['sutherland_top_inner']; ?>
        </div>
      </div>
    </div>
  </div>
  <?php endif; ?>
  <div class="<?php print $container_class; ?>">
    <div id="gcwu-content">
      <?php if ($content['sutherland_first']): ?>
      <div class="region-sutherland-first col-md-4">
        <div class="region-inner clearfix">
          <?php print $content['sutherland_first']; ?>
        </div>
      </div>
      <?php endif; ?>
      <?php if ($content['sutherland_second']): ?>
      <div class="region-sutherland-second col-md-5">
        <div class="region-inner clearfix">
          <?php print $content['sutherland_second']; ?>
        </div>
      </div>
      <?php endif; ?>
      <?php if ($content['sutherland_third']): ?>
      <div id="wb-aside" class="region-sutherland-third col-md-3">
      <div class="region-inner clearfix">
        <?php print $content['sutherland_third']; ?>
      </div>
    </div>
    <?php endif; ?>
    </div>
  </div>
  <?php if ($content['sutherland_bottom_inner']): ?>
  <div class="<?php print $container_class; ?>">
    <div class="row">
      <div class="region-sutherland-bottom-inner col-md-12">
        <div class="region-inner clearfix">
        <?php print $content['sutherland_bottom_inner']; ?>
        </div>
      </div>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['sutherland_bottom']): ?>
  <aside class="features">
    <div class="<?php print $container_class; ?>">
      <div class="row">
        <div class="region-sutherland-bottom col-md-12">
          <div class="region-inner clearfix">
            <?php print $content['sutherland_bottom']; ?>
          </div>
        </div>
      </div>
    </div>
  </aside>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
