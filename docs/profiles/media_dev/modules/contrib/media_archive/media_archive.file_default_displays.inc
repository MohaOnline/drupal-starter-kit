<?php

/**
 * @file
 * Default display configuration for the default file types.
 */

/**
 * Implements hook_file_default_displays().
 */
function media_archive_file_default_displays() {
  $file_displays = array();

  // Default
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__default__media_archive';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width'         => 640,
    'height'        => 388,
    'autoplay'      => FALSE,
    'controls'      => TRUE,
    'controls_hide' => TRUE,
    'loop'          => FALSE,
  );
  $file_displays['video__default__media_archive'] = $file_display;

  // Teaser.
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__teaser__media_archive';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width'         => 560,
    'height'        => 348,
    'autoplay'      => FALSE,
    'controls'      => TRUE,
    'controls_hide' => TRUE,
    'loop'          => FALSE,
  );
  $file_displays['video__teaser__media_archive'] = $file_display;

  // Media Large.
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__media_large__media_archive';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width'         => 480,
    'height'        => 360,
    'autoplay'      => FALSE,
    'controls'      => TRUE,
    'controls_hide' => TRUE,
    'loop'          => FALSE,
  );
  $file_displays['video__media_large__media_archive'] = $file_display;

  // Media Original.
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__media_original__media_archive';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width'         => 640,
    'height'        => 480,
    'autoplay'      => FALSE,
    'controls'      => TRUE,
    'controls_hide' => TRUE,
    'loop'          => FALSE,
  );
  $file_displays['video__media_original__media_archive'] = $file_display;
 
  // Default settings for displaying a video preview image.
  // We enable preview images even for view modes that also play video
  // for use inside a running WYSIWYG editor. We weight them so video
  // formatters come first in the cascade to make sure the video formatter
  // is used whenever possible.
  $default_image_styles = array(
    'default' => 'large',
    'preview' => 'square_thumbnail',
    'teaser' => 'large',
    // Legacy view modes, see note above.
    'media_preview' => 'square_thumbnail',
    'media_large' => 'large',
    'media_original' => '',
  );
  foreach ($default_image_styles as $view_mode => $image_style) {
    $display_name = 'video__' . $view_mode . '__media_archive_image';

    $file_display = new stdClass();
    $file_display->api_version = 1;
    $file_display->name = $display_name;
    $file_display->weight = 0;
    $file_display->status = TRUE;
    $file_display->settings = array(
      'api_version' => 1,
      'name' => $display_name,
      'status' => 1,
      'weight' => 2,
      'settings' => array('image_style' => $image_style),
    );
    $file_displays[$display_name] = $file_display;
  }

  return $file_displays;
}
