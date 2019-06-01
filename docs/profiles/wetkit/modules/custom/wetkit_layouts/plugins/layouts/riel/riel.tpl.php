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
<div class="<?php print $container_class; ?>">
<?php print $panel_prefix; ?>
<div class="panel-display riel clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['riel_top']): ?>
  <div class="region-riel-top ">
    <div class="region-inner clearfix">
      <?php print $content['riel_top']; ?>
    </div>
  </div>
  <div class="clear"></div>
  <?php endif; ?>
    <div class="row">
        <?php if ($content['riel_first']): ?>
       <div class="region-riel-first col-md-8 ">
         <div class="region-inner clearfix">
           <?php print $content['riel_first']; ?>
         </div>
       </div>
       <?php endif; ?>
       <?php if ($content['riel_second']): ?>
       <div class="region-riel-second col-md-4 ">
         <div class="region-inner clearfix">
           <?php print $content['riel_second']; ?>
         </div>
       </div>
       <?php endif; ?>
    </div>
  <?php if ($content['riel_bottom']): ?>
  <div class="region-riel-bottom">
    <div class="region-inner clearfix">
      <?php print $content['riel_bottom']; ?>
    </div>
  </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
</div>
