<?php
/**
 * @file
 * wetkit_widgets.file_type.inc
 */

/**
 * Implements hook_file_default_types().
 */
function wetkit_widgets_file_default_types() {
  $export = array();

  $file_type = new stdClass();
  $file_type->disabled = FALSE; /* Edit this to true to make a default file_type disabled initially */
  $file_type->api_version = 1;
  $file_type->type = 'wetkit_video';
  $file_type->label = 'WetKit Video';
  $file_type->description = 'A <em>WetKit Video</em> file is a moving visual recording using the WET video player.';
  $file_type->mimetypes = array(
    0 => 'video/mp4',
    1 => 'video/ogg',
  );
  $export['wetkit_video'] = $file_type;

  return $export;
}
