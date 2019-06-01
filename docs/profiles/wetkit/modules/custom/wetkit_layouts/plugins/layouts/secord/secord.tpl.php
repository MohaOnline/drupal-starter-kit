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
<div class="panel-display secord clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
    <div class="<?php print $container_class; ?>">
      <div class="row">
        <?php if ($content['secord_top_left']): ?>
        <div class="region-secord-top-left col-md-8 ">
          <div class="region-inner clearfix">
            <?php print $content['secord_top_left']; ?>
          </div>
        </div>
        <?php endif; ?>
          <?php if ($content['secord_top_right_1']): ?>
          <div class="region-secord-top-right-2 col-md-4 ">
            <div class="region-inner clearfix">
              <?php print $content['secord_top_right_1']; ?>
            </div>
          </div>
          <?php endif; ?>
        </div>
          <div class="row equalize">
            <?php if ($content['secord_mid_left']): ?>
            <div class="region-secord-mid-left col-md-4 ">
              <div class="region-inner clearfix">
                <?php print $content['secord_mid_left']; ?>
              </div>
            </div>
            <?php endif; ?>
            <?php if ($content['secord_mid_center']): ?>
            <div class="region-secord-mid-center col-md-4 ">
              <div class="region-inner clearfix">
                <?php print $content['secord_mid_center']; ?>
              </div>
            </div>
            <?php endif; ?>
            <?php if ($content['secord_mid_right']): ?>
            <div class="region-secord-mid-right col-md-4 ">
              <div class="region-inner clearfix">
                <?php print $content['secord_mid_right']; ?>
              </div>
            </div>
            <?php endif; ?>
          </div>
          <div class="row">
            <?php if ($content['secord_bottom_left']): ?>
          <div class="region-secord-bottom-left col-md-4 ">
            <div class="region-inner clearfix">
              <?php print $content['secord_bottom_left']; ?>
            </div>
          </div>
          <?php endif; ?>
          <?php if ($content['secord_bottom_center']): ?>
          <div class="region-secord-bottom-center col-md-4 ">
            <div class="region-inner clearfix">
              <?php print $content['secord_bottom_center']; ?>
            </div>
          </div>
          <?php endif; ?>
          <?php if ($content['secord_bottom_right']): ?>
          <div class="region-secord-bottom-right col-md-4 ">
            <div class="region-inner clearfix">
              <?php print $content['secord_bottom_right']; ?>
            </div>
          </div>
          <?php endif; ?>
        </div>
    </div>
</div>
<?php print $panel_suffix; ?>
