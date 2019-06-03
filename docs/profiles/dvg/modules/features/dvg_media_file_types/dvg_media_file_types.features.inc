<?php
/**
 * @file
 * dvg_media_file_types.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dvg_media_file_types_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "file_entity" && $api == "file_default_displays") {
    return array("version" => "1");
  }
  if ($module == "file_entity" && $api == "file_type") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function dvg_media_file_types_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function dvg_media_file_types_image_default_styles() {
  $styles = array();

  // Exported image style: custom_crop.
  $styles['custom_crop'] = array(
    'label' => 'Custom crop',
    'effects' => array(
      0 => array(
        'name' => 'manualcrop_crop',
        'data' => array(
          'width' => '',
          'height' => '',
          'keepproportions' => 0,
          'reuse_crop_style' => 'page_image',
          'style_name' => 'custom_crop',
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: digid_logo.
  $styles['digid_logo'] = array(
    'label' => 'DigiD logo',
    'effects' => array(
      0 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 54,
          'height' => 54,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: frontpage.
  $styles['frontpage'] = array(
    'label' => 'Frontpage',
    'effects' => array(
      0 => array(
        'name' => 'manualcrop_crop_and_scale',
        'data' => array(
          'width' => 970,
          'height' => 255,
          'upscale' => 0,
          'respectminimum' => 1,
          'onlyscaleifcrop' => 1,
          'style_name' => 'frontpage',
        ),
        'weight' => 0,
      ),
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 970,
          'height' => 255,
          'upscale' => 0,
        ),
        'weight' => 2,
      ),
      2 => array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '100%',
            'height' => '100%',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => '',
          ),
        ),
        'weight' => 3,
      ),
    ),
  );

  // Exported image style: highlight.
  $styles['highlight'] = array(
    'label' => 'Highlight',
    'effects' => array(
      0 => array(
        'name' => 'manualcrop_crop_and_scale',
        'data' => array(
          'width' => 446,
          'height' => 283,
          'upscale' => 0,
          'respectminimum' => 1,
          'onlyscaleifcrop' => 1,
          'style_name' => 'highlight',
        ),
        'weight' => 0,
      ),
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 446,
          'height' => 283,
          'upscale' => 0,
        ),
        'weight' => 2,
      ),
      2 => array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '100%',
            'height' => '100%',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => '',
          ),
        ),
        'weight' => 3,
      ),
    ),
  );

  // Exported image style: menu_image.
  $styles['menu_image'] = array(
    'label' => 'Menu Image',
    'effects' => array(
      0 => array(
        'name' => 'manualcrop_crop_and_scale',
        'data' => array(
          'width' => 446,
          'height' => 283,
          'upscale' => 0,
          'respectminimum' => 1,
          'onlyscaleifcrop' => 1,
          'style_name' => 'menu_image',
        ),
        'weight' => 0,
      ),
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 446,
          'height' => 283,
          'upscale' => 0,
        ),
        'weight' => 2,
      ),
      2 => array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '100%',
            'height' => '100%',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => '',
          ),
        ),
        'weight' => 3,
      ),
    ),
  );

  // Exported image style: page_image.
  $styles['page_image'] = array(
    'label' => 'Page Image',
    'effects' => array(
      0 => array(
        'name' => 'manualcrop_crop_and_scale',
        'data' => array(
          'width' => 830,
          'height' => 553,
          'upscale' => 0,
          'respectminimum' => 1,
          'onlyscaleifcrop' => 1,
          'style_name' => 'page_image',
        ),
        'weight' => 1,
      ),
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 830,
          'height' => 553,
          'upscale' => 0,
        ),
        'weight' => 2,
      ),
      2 => array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '100%',
            'height' => '100%',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => '',
          ),
        ),
        'weight' => 3,
      ),
    ),
  );

  // Exported image style: profile.
  $styles['profile'] = array(
    'label' => 'Profile',
    'effects' => array(
      0 => array(
        'name' => 'manualcrop_crop_and_scale',
        'data' => array(
          'width' => 400,
          'height' => 415,
          'upscale' => 0,
          'respectminimum' => 1,
          'onlyscaleifcrop' => 1,
          'style_name' => 'profile',
        ),
        'weight' => 0,
      ),
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 400,
          'height' => 415,
          'upscale' => 0,
        ),
        'weight' => 2,
      ),
      2 => array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '100%',
            'height' => '100%',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => '',
          ),
        ),
        'weight' => 3,
      ),
    ),
  );

  // Exported image style: profile_group.
  $styles['profile_group'] = array(
    'label' => 'Profile Group',
    'effects' => array(
      0 => array(
        'name' => 'manualcrop_crop_and_scale',
        'data' => array(
          'width' => 830,
          'height' => 452,
          'upscale' => 0,
          'respectminimum' => 1,
          'onlyscaleifcrop' => 0,
          'style_name' => 'profile_group',
        ),
        'weight' => 0,
      ),
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 830,
          'height' => 452,
          'upscale' => 0,
        ),
        'weight' => 2,
      ),
      2 => array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
          'RGB' => array(
            'HEX' => '',
          ),
          'under' => 1,
          'exact' => array(
            'width' => '100%',
            'height' => '100%',
            'xpos' => 'center',
            'ypos' => 'center',
          ),
          'relative' => array(
            'leftdiff' => '',
            'rightdiff' => '',
            'topdiff' => '',
            'bottomdiff' => '',
          ),
        ),
        'weight' => 3,
      ),
    ),
  );

  return $styles;
}
