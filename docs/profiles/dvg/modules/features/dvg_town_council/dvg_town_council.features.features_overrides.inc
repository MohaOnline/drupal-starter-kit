<?php
/**
 * @file
 * dvg_town_council.features.features_overrides.inc
 */

/**
 * Implements hook_features_override_default_overrides().
 */
function dvg_town_council_features_override_default_overrides() {
  // This code is only used for UI in features. Exported alters hooks do the magic.
  $overrides = array();

  // Exported overrides for: variable
  $overrides["variable||menu_options_page||value|3"] = 'menu-town-council';
  $overrides["variable||menu_options_profile_group||value|3"] = 'menu-town-council';

 return $overrides;
}
