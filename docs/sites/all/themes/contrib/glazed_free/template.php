<?php

/**
 * @file template.php
 */

// Load Glazed Theme Settings CSS File
global $theme;
$files_path = variable_get('file_public_path', conf_path() . '/files');
if (is_file($files_path . '/glazed-themesettings-' . $theme . '.css')) {
  drupal_add_css(
    $files_path . '/glazed-themesettings-' . $theme . '.css', array(
      'preprocess' => variable_get('preprocess_css', '') == 1 ? TRUE : FALSE,
      'group' => CSS_THEME,
      'media' => 'all',
      'every_page' => TRUE,
      'weight' => 100
    )
  );
}

/**
 * Load template.php logic from theme features
 */

foreach (file_scan_directory(drupal_get_path('theme', 'glazed_free') . '/features', '/controller.inc/i') as $file) {
  require_once($file->uri);
}
/**
 * Load template.php Glazed theme functions
 */

foreach (file_scan_directory(drupal_get_path('theme', 'glazed_free') . '/includes', '/.inc/i') as $file) {
  require_once($file->uri);
}
