<?php

/**
 * Implements hook_js_alter().
 */
function glazed_free_js_alter(&$js) {
  $theme_path = drupal_get_path('theme', 'glazed_free') . '/';
  // Add Bootstrap settings.
  $js['settings']['data'][]['glazed_free'] = array(
    'glazedPath' => $theme_path,
  );
  // Override color module color.js because it's broken when bootstrap theme is loaded
  if (isset( $js['modules/color/color.js']['data'])) {
    $js['modules/color/color.js']['data'] = $theme_path . 'js/color.js';
  }
}
