<?php

/**
 * @file
 * Default display configuration for the default file types.
 */

/**
 * Implements hook_file_default_displays().
 */
function govcms_ckan_media_file_default_displays() {
  $file_displays = array();

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'ckan__preview__govcms_ckan_media_image';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'media_thumbnail',
  );
  $file_displays['ckan__preview__govcms_ckan_media_image'] = $file_display;

  return $file_displays;
}
