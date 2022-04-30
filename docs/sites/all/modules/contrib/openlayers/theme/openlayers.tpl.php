<?php

/**
 * @file
 * Default theme implementation to display an Openlayers map.
 *
 * Debug the result of the function get_defined_vars() to have an overview
 * of all the variables you have access to.
 */
?>

<div <?php print $openlayers['attributes']['openlayers-container']; ?>>
  <div <?php print $openlayers['attributes']['openlayers-map-container']; ?>>
    <?php print render($openlayers['map_prefix']); ?>
    <div <?php print $openlayers['attributes']['openlayers-map']; ?>></div>
    <?php print render($openlayers['map_suffix']); ?>
  </div>
</div>

<?php if (isset($openlayers['parameters'])): ?>
  <?php print render($openlayers['parameters']); ?>
<?php endif; ?>

<?php if (isset($openlayers['capabilities'])): ?>
  <?php print render($openlayers['capabilities']); ?>
<?php endif; ?>
