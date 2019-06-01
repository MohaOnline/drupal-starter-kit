<?php
/**
 * @file
 * wetkit_images.default_picture_mapping.inc
 */

/**
 * Implements hook_default_picture_mapping().
 */
function wetkit_images_default_picture_mapping() {
  $export = array();

  $picture_mapping = new stdClass();
  $picture_mapping->disabled = FALSE; /* Edit this to true to make a default picture_mapping disabled initially */
  $picture_mapping->api_version = 1;
  $picture_mapping->machine_name = 'wxt_responsive_image';
  $picture_mapping->breakpoint_group = 'wxt_responsive_image';
  $picture_mapping->mapping = array(
    'custom.user.mobile' => array(
      '1x' => 'wetkit_image_mobile',
    ),
    'custom.user.narrow' => array(
      '1x' => 'wetkit_image_narrow',
    ),
    'custom.user.wide' => array(
      '1x' => 'wetkit_image_wide',
    ),
  );
  $export['wxt_responsive_image'] = $picture_mapping;

  return $export;
}
