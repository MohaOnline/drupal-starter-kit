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
<div class="<?php print $container_class; ?> panel-display payette clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <div class="row">
    <?php if ($content['payette_top_left']): ?>
    <div class="region-payette-top-left col-md-8 ">
      <div class="region-inner clearfix">
        <?php print $content['payette_top_left']; ?>
      </div>
    </div>
    <?php endif; ?>
    <?php if ($content['payette_top_right']): ?>
    <div class="region-payette-top-right col-md-4 ">
      <div class="region-inner clearfix">
        <?php print $content['payette_top_right']; ?>
      </div>
    </div>
    <?php endif; ?>
    </div>
    <div class="row">
      <?php if ($content['payette_mid_left']): ?>
    <div class="region-payette-mid-left col-md-4 ">
      <div class="region-inner clearfix">
        <?php print $content['payette_mid_left']; ?>
      </div>
    </div>
    <?php endif; ?>
    <?php if ($content['payette_mid_center']): ?>
    <div class="region-payette-mid-center col-md-4 ">
      <div class="region-inner clearfix">
        <?php print $content['payette_mid_center']; ?>
      </div>
    </div>
    <?php endif; ?>
    <?php if ($content['payette_mid_right']): ?>
    <div class="region-payette-mid-right col-md-4 ">
      <div class="region-inner clearfix">
        <?php print $content['payette_mid_right']; ?>
      </div>
    </div>
    <?php endif; ?>
    </div>
    <div class="row">
      <?php if ($content['payette_bottom_left']): ?>
    <div class="region-payette-bottom-left col-md-4 ">
      <div class="region-inner clearfix">
        <?php print $content['payette_bottom_left']; ?>
      </div>
    </div>
    <?php endif; ?>
    <?php if ($content['payette_bottom_center']): ?>
    <div class="region-payette-bottom-center col-md-4 ">
      <div class="region-inner clearfix">
        <?php print $content['payette_bottom_center']; ?>
      </div>
    </div>
    <?php endif; ?>
    <?php if ($content['payette_bottom_right']): ?>
    <div class="region-payette-bottom-right col-md-4 ">
      <div class="region-inner clearfix">
        <?php print $content['payette_bottom_right']; ?>
      </div>
    </div>
    <?php endif; ?>
  </div>
</div>
<?php print $panel_suffix; ?>
