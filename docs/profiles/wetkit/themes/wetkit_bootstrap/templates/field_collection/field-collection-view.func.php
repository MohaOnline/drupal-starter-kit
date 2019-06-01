<?php
/**
 * @file
 * field_collection.func.php
 */

/**
 * Themes field collection items printed using the field_collection_view formatter.
 */
function wetkit_bootstrap_field_collection_view($variables) {
  $element = $variables['element'];
  return $element['#children'];
}
