<?php
/**
 * @file
 * wetkit_images.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function wetkit_images_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'picture_ckeditor_groups';
  $strongarm->value = array(
    'wxt_responsive_image' => array(
      'enabled' => 1,
      'weight' => '1',
      'fallback' => 'wetkit_image_narrow',
    ),
  );
  $export['picture_ckeditor_groups'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'picture_ckeditor_label';
  $strongarm->value = 'Image size (required)';
  $export['picture_ckeditor_label'] = $strongarm;

  return $export;
}
