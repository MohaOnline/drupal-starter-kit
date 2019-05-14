<?php

// This needs to be here because Drupal subthemes don't automaitically inherit
// base theme theme-settings.php which contains form submit functions
if (!function_exists('glazed_free_settings_form_submit')) {
  require_once(DRUPAL_ROOT . '/' . drupal_get_path('theme', 'glazed_free') . '/theme-settings.php');
}