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
<div class="<?php print $container_class; ?> panel-display doohan clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['doohan_top']): ?>
  <div class="region-doohan-top">
    <div class="region-inner clearfix">
      <?php print $content['doohan_top']; ?>
    </div>
  </div>
  <?php endif; ?>
    <div class="row">
      <div class="col-md-8 col-md-push-4">
        <?php if ($content['doohan_first']): ?>
      <div class="region-doohan-first ">
        <div class="region-inner clearfix">
          <?php print $content['doohan_first']; ?>
        </div>
      </div>
      <?php endif; ?>
      <div class="row">
        <?php if ($content['doohan_second']): ?>
      <div class="region-doohan-second col-md-8">
        <div class="region-inner clearfix">
          <?php print $content['doohan_second']; ?>
        </div>
      </div>
      <?php endif; ?>
      <?php if ($content['doohan_third']): ?>
      <div class="region-doohan-third col-md-4">
        <div class="region-inner clearfix">
          <?php print $content['doohan_third']; ?>
        </div>
      </div>
      <?php endif; ?>
    </div>
      <?php if ($content['doohan_content_footer']): ?>
      <div class="region-doohan-content-footer">
        <div class="region-inner clearfix">
          <?php print $content['doohan_content_footer']; ?>
        </div>
      </div>
      <?php endif; ?>
      </div>
      <?php if ($content['doohan_fourth']): ?>
       <div class="region-doohan-third col-md-4 col-md-pull-8 row-start">
        <div class="region-inner clearfix">
          <nav role="navigation" id="wb-sec">
            <h2 id="wb-side-nav"><?php print t('Section menu'); ?></h2>
            <div class="wb-sec-def">
              <?php print $content['doohan_fourth']; ?>
            </div>
          </nav>
        </div>
      </div>
      <?php endif; ?>
    </div>

  <?php if ($content['doohan_bottom']): ?>
  <div class="region-doohan-bottom">
    <div class="region-inner clearfix">
      <?php print $content['doohan_bottom']; ?>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
