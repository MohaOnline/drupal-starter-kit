<?php
/**
 * @file
 * Template for WetKit Boxton.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="<?php print $container_class; ?> panel-display boxton clearfix <?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="row">
    <div class="boxton-container boxton-content boxton-content-region panel-panel col-md-12">
      <?php print $content['contentmain']; ?>
    </div>
  </div>
</div><!-- /.boxton -->
