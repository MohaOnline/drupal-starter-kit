<?php
/**
 * @file
 * wetkit_widgets.theme.inc
 */

/**
 * Returns HTML for displaying an HTML5 <video> tag.
 *
 * @param array $variables
 *   An associative array containing:
 *   - file: Associative array of file data, which must include "uri".
 *   - controls: Boolean indicating whether or not controls should be displayed.
 *   - autoplay: Boolean indicating whether or not the video should start
 *   playing automatically.
 *   - loop: Boolean indicating whether or not the video should loop.
 *   - muted: Boolean indicating whether or not the sound should be muted.
 *   - width: Width, in pixels, of the video player.
 *   - height: Height, in pixels, of the video player.
 *
 * @ingroup themeable
 */
function theme_wetkit_widgets_file_video($variables) {
  $files = $variables['files'];
  $output = '';

  $video_attributes = array();
  if ($variables['controls']) {
    $video_attributes['controls'] = 'controls';
  }
  if ($variables['autoplay']) {
    $video_attributes['autoplay'] = 'autoplay';
  }
  if ($variables['loop']) {
    $video_attributes['loop'] = 'loop';
  }
  if ($variables['muted']) {
    $video_attributes['muted'] = 'muted';
  }
  if ($variables['width'] && $variables['height']) {
    $video_attributes['width'] = $variables['width'];
    $video_attributes['height'] = $variables['height'];
  }

  if (isset($files[0]['field_poster'][LANGUAGE_NONE][0]['fid'])) {
    $imgpath = file_load($files[0]['field_poster'][LANGUAGE_NONE][0]['fid'])->uri;
    $video_attributes['poster'] = file_create_url($imgpath);
  }

  $output .= '<div class="wb-mltmd"><video' . drupal_attributes($video_attributes) . '>';
  foreach ($files as $file) {
    $source_attributes = array(
      'src' => file_create_url($file['uri']),
      'type' => $file['filemime'],
    );
    $output .= '<source' . drupal_attributes($source_attributes) . ' />';
  }

  if (isset($files[0]['field_caption'][LANGUAGE_NONE][0]['fid'])) {
    $file = file_load($files[0]['field_caption'][LANGUAGE_NONE][0]['fid']);
    $track_attributes = array(
      'data-type' => $file->filemime,
      'kind' => 'captions',
      'src' => file_create_url($file->uri),
    );
    $output .= '<track' . drupal_attributes($track_attributes) . ' />';
  }

  $output .= '</video></div>';
  return $output;
}
