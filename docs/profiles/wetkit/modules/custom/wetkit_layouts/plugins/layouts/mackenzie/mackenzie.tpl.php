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
<div class="<?php print $container_class; ?> panel-display mackenzie clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['mackenzie_header']): ?>
  <div class="region-mackenzie-header ">
    <div class="region-inner clearfix">
      <?php print $content['mackenzie_header']; ?>
    </div>
  </div>
  <?php endif; ?>
  <div class="row">
    <?php if ($content['mackenzie_header']): ?>
  <div class="col-md-4 col-md-push-8 ">
    <div class="region-inner clearfix">
      <?php print $content['mackenzie_sidebar_first']; ?>
    </div>
  </div>
  <?php endif; ?>
  <div class="col-md-8 col-md-pull-4 margin-top-none margin-bottom-medium">
    <?php if ($content['mackenzie_banner_top']): ?>
    <div class="margin-top-none margin-bottom-medium">
      <div class="region-inner clearfix">
        <?php print $content['mackenzie_banner_top']; ?>
      </div>
    </div>
    <?php endif; ?>
    <div class="row equalize indent-none">
      <?php if ($content['mackenzie_section_first']): ?>
      <div class="col-md-4 row-start">
        <?php print $content['mackenzie_section_first']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['mackenzie_section_second']): ?>
      <div class="col-md-4">
        <?php print $content['mackenzie_section_second']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['mackenzie_section_third']): ?>
      <div class="col-md-4 row-end">
        <?php print $content['mackenzie_section_third']; ?>
      </div>
      <?php endif; ?>
    </div>
    <div class="row equalize indent-none">
      <?php if ($content['mackenzie_section_fourth']): ?>
      <div class="col-md-4 row-start">
        <?php print $content['mackenzie_section_fourth']; ?>
        </div>
        <?php endif; ?>
        <?php if ($content['mackenzie_section_fifth']): ?>
      <div class="col-md-4">
        <?php print $content['mackenzie_section_fifth']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['mackenzie_section_sixth']): ?>
      <div class="col-md-4 row-end">
        <?php print $content['mackenzie_section_sixth']; ?>
      </div>
      <?php endif; ?>
    </div>
    <?php if ($content['mackenzie_banner_bottom']): ?>
    <div>
      <div class="region-inner clearfix">
        <?php print $content['mackenzie_banner_bottom']; ?>
      </div>
    </div>
    <?php endif; ?>
  </div>
  </div>
  <?php if ($content['mackenzie_footer']): ?>
  <div>
    <div class="region-inner clearfix">
      <?php print $content['mackenzie_footer']; ?>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
