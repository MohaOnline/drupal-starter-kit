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
$wb_eqht1 =  (isset($vanier_content_search) || isset($vanier_content_datasets)) ? 'wb-eqht' : '';
$wb_eqht2 =  (isset($vanier_spotlight_1) || isset($vanier_spotlight_2) || isset($vanier_spotlight_3)) ? 'wb-eqht' : '';
?>
<?php print $panel_prefix; ?>
<div class="<?php print $container_class; ?>">
  <div class="panel-display vanier clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
    <?php if ($content['vanier_top']): ?>
    <div class="region-vanier-top">
      <?php print $content['vanier_top']; ?>
    </div>
    <?php endif; ?>
    <div class="row profile">
      <?php if ($content['vanier_content_banner']): ?>
      <div class="region-vanier-content-banner col-md-6">
        <?php print $content['vanier_content_banner']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['vanier_content_listings']): ?>
      <div class="region-vanier-content-listings col-md-6">
        <?php print $content['vanier_content_listings']; ?>
      </div>
      <?php endif; ?>
    </div>
    <div class="row <?php print $wb_eqht1; ?>">
      <?php if ($content['vanier_content_search']): ?>
      <div class="region-vanier-content-search col-md-8">
        <?php print $content['vanier_content_search']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['vanier_content_datasets']): ?>
      <div class="region-vanier-content-datasets col-md-4">
        <?php print $content['vanier_content_datasets']; ?>
      </div>
      <?php endif; ?>
    </div>
    <div class="row">
      <?php if ($content['vanier_section_1']): ?>
      <div class="region-vanier-section-1 col-md-3">
        <?php print $content['vanier_section_1']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['vanier_section_2']): ?>
      <div class="region-vanier-section-2 col-md-3">
        <?php print $content['vanier_section_2']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['vanier_section_3']): ?>
      <div class="region-vanier-section-3 col-md-3">
        <?php print $content['vanier_section_3']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['vanier_section_4']): ?>
      <div class="region-vanier-section-4 col-md-3">
        <?php print $content['vanier_section_4']; ?>
      </div>
      <?php endif; ?>
    </div>
    <div class="row <?php print $wb_eqht1; ?> priorities">
      <?php if ($content['vanier_spotlight_1']): ?>
      <div class="region-vanier-spotlight-1 col-md-4">
        <?php print $content['vanier_spotlight_1']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['vanier_spotlight_2']): ?>
      <div class="region-vanier-spotlight-2 col-md-4">
        <?php print $content['vanier_spotlight_2']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['vanier_spotlight_3']): ?>
      <div class="region-vanier-spotlight-3 col-md-4">
        <?php print $content['vanier_spotlight_3']; ?>
      </div>
      <?php endif; ?>
    </div>
    <div class="row pagedetails">
      <?php if ($content['vanier_spotlight_4']): ?>
      <div class="region-vanier-spotlight-4 col-md-4">
        <?php print $content['vanier_spotlight_4']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['vanier_spotlight_5']): ?>
      <div class="region-vanier-spotlight-5 col-md-4">
        <?php print $content['vanier_spotlight_5']; ?>
      </div>
      <?php endif; ?>
      <?php if ($content['vanier_spotlight_6']): ?>
      <div class="region-vanier-spotlight-6 col-md-4">
        <?php print $content['vanier_spotlight_6']; ?>
      </div>
      <?php endif; ?>
    </div>
    <?php if ($content['vanier_bottom']): ?>
    <div class="region-vanier-bottom">
      <?php print $content['vanier_bottom']; ?>
    </div>
    <?php endif; ?>
  </div>
</div>
<?php print $panel_suffix; ?>
