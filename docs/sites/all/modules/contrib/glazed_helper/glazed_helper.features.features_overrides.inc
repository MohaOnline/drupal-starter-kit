<?php
/**
 * @file
 * glazed_helper.features.features_overrides.inc
 */

/**
 * Implements hook_features_override_default_overrides().
 */
function glazed_helper_features_override_default_overrides() {
  // This code is only used for UI in features. Exported alters hooks do the magic.
  $overrides = array();

  // Exported overrides for: filter
  $overrides["filter.wysiwyg_full.filters|shortcode|settings|animate"] = 1;
  $overrides["filter.wysiwyg_full.filters|shortcode|settings|dropshadow"] = 1;

 return $overrides;
}
