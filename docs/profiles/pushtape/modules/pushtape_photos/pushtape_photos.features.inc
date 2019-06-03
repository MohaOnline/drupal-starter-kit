<?php
/**
 * @file
 * pushtape_photos.features.inc
 */

/**
 * Implements hook_views_api().
 */
function pushtape_photos_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function pushtape_photos_image_default_styles() {
  $styles = array();

  // Exported image style: photo_large.
  $styles['photo_large'] = array(
    'effects' => array(
      3 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 900,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'photo_large',
  );

  // Exported image style: photo_thumb.
  $styles['photo_thumb'] = array(
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 205,
          'height' => 205,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'photo_thumb',
  );

  // Exported image style: photo_tiny.
  $styles['photo_tiny'] = array(
    'effects' => array(
      4 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 100,
          'height' => 100,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'photo_tiny',
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function pushtape_photos_node_info() {
  $items = array(
    'photoset' => array(
      'name' => t('Photoset'),
      'base' => 'node_content',
      'description' => t('A simple photoset for sharing a group of photos.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
