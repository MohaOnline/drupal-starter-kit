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
<div class="panel-display dallaire clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['dallaire_top']): ?>
  <div class="region-dallaire-top">
    <div class="region-inner clearfix">
      <?php print $content['dallaire_top']; ?>
    </div>
  </div>
  <?php endif; ?>
  <div class="<?php print $container_class; ?>">
    <div class="row" role="main">
      <?php if ($content['dallaire_second']): ?>
      <div class="region-dallaire-first col-md-6 col-md-push-3">
      <?php else: ?>
      <div class="region-dallaire-first col-md-9 col-md-push-3">
      <?php endif; ?>
        <div class="region-inner clearfix">
          <?php print $content['dallaire_first']; ?>
        </div>
      </div>
      <?php if ($content['dallaire_second']): ?>
      <div class="region-dallaire-second col-md-3 col-md-push-3">
        <div class="region-inner clearfix">
          <?php print $content['dallaire_second']; ?>
        </div>
      </div>
      <?php endif; ?>
      <div class="region-dallaire-third col-md-3 col-md-pull-9 row-start">
        <div class="region-inner clearfix">
          <nav role="navigation" id="wb-sec">
            <h2 id="wb-side-nav"><?php print t('Section menu'); ?></h2>
            <div class="wb-sec-def">
              <?php print $content['dallaire_third']; ?>
            </div>
          </nav>
        </div>
      </div>
    </div>
  </div>
    <?php if ($content['dallaire_bottom']): ?>
    <div class="region-dallaire-bottom">
      <div class="region-inner clearfix">
        <?php print $content['dallaire_bottom']; ?>
      </div>
    </div>
    <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
