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
<div class="<?php print $container_class; ?> panel-display penfield clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['penfield_top']): ?>
  <div class="region-penfield-top col-md-12">
    <div class="region-inner clearfix">
      <?php print $content['penfield_top']; ?>
    </div>
  </div>
  <?php endif; ?>
  <div id="wb-main" role="main" class="col-md-8">
    <div id="wb-main-in">
      <div class="row">
        <div class="region-penfield-first col-md-4">
          <div class="region-inner clearfix">
            <?php print $content['penfield_first']; ?>
          </div>
        </div>
        <div class="region-penfield-second col-md-2">
          <div class="region-inner clearfix">
            <?php print $content['penfield_second']; ?>
          </div>
        </div>
        <div class="region-penfield-content-footer col-md-6">
          <div class="region-inner clearfix">
            <?php print $content['penfield_content_footer']; ?>
          </div>
        </div>
      </div>
    </div>
  </div>
  <div id="wb-sec">
    <div id="wb-sec-in">
      <div class="region-penfield-third col-md-4">
        <div class="region-inner clearfix">
          <nav role="navigation" id="wb-sec">
            <h2 id="wb-side-nav"><?php print t('Section menu'); ?></h2>
            <div class="wb-sec-def">
              <?php print $content['penfield_third']; ?>
            </div>
          </nav>
        </div>
      </div>
    </div>
  </div>
  <?php if ($content['penfield_bottom']): ?>
  <div class="region-penfield-bottom col-md-12">
    <div class="region-inner clearfix">
      <?php print $content['penfield_bottom']; ?>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
