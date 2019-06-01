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
<div class="<?php print $container_class; ?> panel-display pearson clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['pearson_top']): ?>
  <div class="row">
    <div class="region-pearson-top col-md-12">
      <div class="region-inner clearfix">
        <?php print $content['pearson_top']; ?>
      </div>
    </div>
  </div>
  <?php endif; ?>
      <div class="row">
        <div role="main" property="mainContentOfPage" class="col-md-9 col-md-push-3">
          <div class="region-pearson-first">
            <div class="region-inner clearfix">
              <?php print $content['pearson_first']; ?>
            </div>
          </div>
        </div>
        <div class="region-pearson-second">
          <nav typeof="SiteNavigationElement" class="col-md-3 col-md-pull-9 <?php print $sidebar_class; ?>">
              <div id="wb-side-nav"></div>
              <?php if ($content['pearson_second']): ?>
                <div id="wb-sec" class="wb-sec-def">
                  <?php print $content['pearson_second']; ?>
                </div>
              <?php endif; ?>
              <?php if ($content['pearson_third']): ?>
                <div class="wb-sec-def-other">
                  <?php print $content['pearson_third']; ?>
                </div>
              <?php endif; ?>
          </nav>
        </div>
      </div>
  <?php if ($content['pearson_bottom']): ?>
  <div class="row">
    <div class="region-pearson-bottom col-md-12">
      <div class="region-inner clearfix">
        <?php print $content['pearson_bottom']; ?>
      </div>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
