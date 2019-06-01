<?php
/**
 * @file
 * Template for WetKit siris.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display siris clearfix <?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <div class="siris-container siris-header clearfix panel-panel">
    <div class="siris-container-inner siris-header-inner panel-panel-inner">
      <?php print $content['header']; ?>
    </div>
  </div>

  <div class="siris-container siris-banner clearfix panel-panel">
    <div class="siris-container-inner siris-banner-inner panel-panel-inner">
      <?php print $content['banner']; ?>
    </div>
  </div>

  <div class="siris-container siris-column-content clearfix">
    <div class="siris-column-content-region siris-column1 siris-column panel-panel">
      <div class="siris-column-content-region-inner siris-column1-inner siris-column-inner panel-panel-inner">
        <?php print $content['column1']; ?>
      </div>
    </div>
    <div class="siris-column-content-region siris-column2 siris-column panel-panel">
      <div class="siris-column-content-region-inner siris-column2-inner siris-column-inner panel-panel-inner">
        <?php print $content['column2']; ?>
      </div>
    </div>
  </div>

  <div class="siris-container siris-footer clearfix panel-panel">
    <div class="siris-container-inner siris-footer-inner panel-panel-inner">
      <?php print $content['footer']; ?>
    </div>
  </div>

</div><!-- /.siris -->
