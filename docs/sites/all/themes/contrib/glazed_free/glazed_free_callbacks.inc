<?php

//
//  Glazed Helper functions
//

function _color_module_css_write($theme) {
  // stuff copied from color module, because this code is not encapsulated in a function other than theme submit
  module_load_include('module', 'color');
  $info = color_get_info($theme);
  $palette = color_get_palette($theme);

  // Delete old files.
  foreach (variable_get('color_' . $theme . '_files', array()) as $file) {
    @drupal_unlink($file);
  }
  if (isset($file) && $file = dirname($file)) {
    @drupal_rmdir($file);
  }

  // Prepare target locations for generated files.
  $id = $theme . '-' . substr(hash('sha256', serialize($palette) . microtime()), 0, 8);
  $paths['color'] = 'public://color';
  $paths['target'] = $paths['color'] . '/' . $id;
  foreach ($paths as $path) {
    file_prepare_directory($path, FILE_CREATE_DIRECTORY);
  }
  $paths['target'] = $paths['target'] . '/';
  $paths['id'] = $id;
  $paths['source'] = drupal_get_path('theme', $theme) . '/';
  $paths['files'] = $paths['map'] = array();

  // Save palette and logo location.
  variable_set('color_' . $theme . '_palette', $palette);
  variable_set('color_' . $theme . '_logo', $paths['target'] . 'logo.png');

  // Copy over neutral images.
  foreach ($info['copy'] as $file) {
    $base = drupal_basename($file);
    $source = $paths['source'] . $file;
    $filepath = file_unmanaged_copy($source, $paths['target'] . $base);
    $paths['map'][$file] = $base;
    $paths['files'][] = $filepath;
    drupal_set_message($file);
    drupal_set_message($filepath);
    drupal_set_message('<pre>' . print_r($paths, TRUE) . '</pre>');
  }

  // Render new images, if image has been provided.
  if ($info['base_image']) {
    _color_render_images($theme, $info, $paths, $palette);
  }

  // Rewrite theme stylesheets.
  $css = array();
  foreach ($info['css'] as $stylesheet) {
    // Build a temporary array with LTR and RTL files.
    $files = array();
    if (file_exists($paths['source'] . $stylesheet)) {
      $files[] = $stylesheet;

      $rtl_file = str_replace('.css', '-rtl.css', $stylesheet);
      if (file_exists($paths['source'] . $rtl_file)) {
        $files[] = $rtl_file;
      }
    }

    foreach ($files as $file) {
      // Aggregate @imports recursively for each configured top level CSS file
      // without optimization. Aggregation and optimization will be
      // handled by drupal_build_css_cache() only.
      $style = drupal_load_stylesheet($paths['source'] . $file, FALSE);

      // Return the path to where this CSS file originated from, stripping
      // off the name of the file at the end of the path.
      $base = base_path() . dirname($paths['source'] . $file) . '/';
      _drupal_build_css_path(NULL, $base);

      // Prefix all paths within this CSS file, ignoring absolute paths.
      $style = preg_replace_callback('/url\([\'"]?(?![a-z]+:|\/+)([^\'")]+)[\'"]?\)/i', '_drupal_build_css_path', $style);

      // Rewrite stylesheet with new colors.
      $style = _color_rewrite_stylesheet($theme, $info, $paths, $palette, $style);
      $base_file = drupal_basename($file);
      $css[] = $paths['target'] . $base_file;
      _color_save_stylesheet($paths['target'] . $base_file, $style, $paths);
    }
  }

  // Maintain list of files.
  variable_set('color_' . $theme . '_stylesheets', $css);
  variable_set('color_' . $theme . '_files', $paths['files']);
}

/**
 * Theme CSS generator from Glazed theme-settings.php
 * Custom theme settings might need a lot of CSS
 * So we put it in a file for optimal performance
 */
function glazed_free_css_cache_build($theme) {
  global $base_path, $base_theme_info;
  $files_path = variable_get('file_public_path', conf_path() . '/files');

  $glazed_free_css_file = $files_path . '/glazed-themesettings-' . $theme . '.css';
  //Construct CSS file:
  $CSS = '';
  // Load Sooper Features CSS
  foreach (file_scan_directory(drupal_get_path('theme', 'glazed_free') . '/features', '/css.inc/i') as $file) {
    require_once($file->uri);
  }

  $fh = fopen($glazed_free_css_file, 'w');
  if ($fh) {
    fwrite($fh, $CSS); // write css to file
    watchdog('glazed_free', 'Glazed CSS file cache built for %theme', array('%theme' => $theme));
  }
  else {
    drupal_set_message(t('Cannot write theme-settings file, please check your file system. (See status report page)'), 'error');
  }

  fclose($fh);
  // If the CSS & JS aggregation are enabled we need to clear the caches
  drupal_clear_css_cache();
  drupal_clear_js_cache();
}