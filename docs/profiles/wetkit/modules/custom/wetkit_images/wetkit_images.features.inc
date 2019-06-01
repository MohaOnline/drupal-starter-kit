<?php
/**
 * @file
 * wetkit_images.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wetkit_images_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "breakpoints" && $api == "default_breakpoint_group") {
    return array("version" => "1");
  }
  if ($module == "breakpoints" && $api == "default_breakpoints") {
    return array("version" => "1");
  }
  if ($module == "file_entity" && $api == "file_default_displays") {
    return array("version" => "1");
  }
  if ($module == "picture" && $api == "default_picture_mapping") {
    return array("version" => "2");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function wetkit_images_image_default_styles() {
  $styles = array();

  // Exported image style: wetkit_image_featured.
  $styles['wetkit_image_featured'] = array(
    'effects' => array(
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 480,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
      3 => array(
        'name' => 'image_crop',
        'data' => array(
          'width' => 480,
          'height' => 260,
          'anchor' => 'center-center',
        ),
        'weight' => 3,
      ),
    ),
    'label' => 'wetkit_image_featured',
  );

  // Exported image style: wetkit_image_full.
  $styles['wetkit_image_full'] = array(
    'effects' => array(
      2 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 960,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 2,
      ),
    ),
    'label' => 'wetkit_image_full',
  );

  // Exported image style: wetkit_image_half.
  $styles['wetkit_image_half'] = array(
    'effects' => array(
      2 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 480,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'wetkit_image_half',
  );

  // Exported image style: wetkit_image_mobile.
  $styles['wetkit_image_mobile'] = array(
    'label' => 'wetkit_image_mobile',
    'effects' => array(
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 300,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: wetkit_image_narrow.
  $styles['wetkit_image_narrow'] = array(
    'label' => 'wetkit_image_narrow',
    'effects' => array(
      2 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 940,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: wetkit_image_quarter.
  $styles['wetkit_image_quarter'] = array(
    'effects' => array(
      5 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 240,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'wetkit_image_quarter',
  );

  // Exported image style: wetkit_image_spotlight.
  $styles['wetkit_image_spotlight'] = array(
    'effects' => array(
      2 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 960,
          'height' => 560,
        ),
        'weight' => 2,
      ),
    ),
    'label' => 'wetkit_image_spotlight',
  );

  // Exported image style: wetkit_image_square.
  $styles['wetkit_image_square'] = array(
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 200,
          'height' => 200,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'wetkit_image_square',
  );

  // Exported image style: wetkit_image_thumbnail.
  $styles['wetkit_image_thumbnail'] = array(
    'effects' => array(
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 80,
          'height' => 80,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'wetkit_image_thumbnail',
  );

  // Exported image style: wetkit_image_video.
  $styles['wetkit_image_video'] = array(
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 640,
          'height' => 390,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'wetkit_image_video',
  );

  // Exported image style: wetkit_image_wide.
  $styles['wetkit_image_wide'] = array(
    'label' => 'wetkit_image_wide',
    'effects' => array(
      3 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 1180,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
