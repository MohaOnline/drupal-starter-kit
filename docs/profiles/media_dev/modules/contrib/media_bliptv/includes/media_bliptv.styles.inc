<?php

/**
 * @file media_bliptv/includes/media_bliptv.styles.inc
 * Styles definitions for Media: BlipTV.
 */

/**
 * Implementation of Styles module hook_styles_register().
 */
function media_bliptv_styles_register() {
  return array(
    'MediaBlipTVStyles' => array(
      'field_types' => 'file',
      'name' => t('MediaBlipTV'),
      'description' => t('Media BlipTV styles.'),
      'path' => drupal_get_path('module', 'media_bliptv') .'/includes',
      'file' => 'media_bliptv.styles.inc',
    ),
  );
}

/**
 *  Implements hook_styles_containers(). (Deprecated in version 2)
 */
function media_bliptv_styles_containers() {
  return array(
    'file' => array(
      'containers' => array(
        'media_bliptv' => array(
          'label' => t('BlipTV Styles'),
          'data' => array(
            'streams' => array(
              'bliptv',
            ),
            'mimetypes' => array(
              'video/bliptv',
            ),
          ),
          'weight' => 0,
          'filter callback' => 'media_bliptv_formatter_filter',
          'themes' => array(
            'field_formatter_styles' => 'media_bliptv_field_formatter_styles',
            'styles' => 'media_bliptv_styles',
            'preview' => 'media_bliptv_preview_style',
          ),
          'description' => t('BlipTV Styles will display embedded BlipTV videos and thumbnails to your choosing, such as by resizing, setting colors, and autoplay. You can !manage.', array('!manage' => l(t('manage your BlipTV styles here'), 'admin/config/media/media-bliptv-styles'))),
        ),
      ),
    ),
  );
}

function media_bliptv_formatter_filter($variables) {
  if (isset($variables['object'])) {
    $object = isset($variables['object']->file) ? $variables['object']->file : $variables['object'];
    return (file_uri_scheme($object->uri) == 'bliptv') && ($object->filemime == 'video/bliptv');
  }
}

/**
 * Implementation of the File Styles module's hook_file_styles_filter().
 */
function media_bliptv_file_styles_filter($object) {
  $file = isset($object->file) ? $object->file : $object;
  if ((file_uri_scheme($file->uri) == 'bliptv') && ($file->filemime == 'video/bliptv')) {
    return 'media_bliptv';
  }
}

/**
 *  Implements hook_styles_styles().
 */
function media_bliptv_styles_styles() {
  $styles = array(
    'file' => array(
      'containers' => array(
        'media_bliptv' => array(
          'styles' => array(
            'bliptv_thumbnail' => array(
              'name' => 'bliptv_thumbnail',
              'effects' => array(
                array('label' => t('Thumbnail'), 'name' => 'thumbnail', 'data' => array('thumbnail' => 1)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 100, 'height' => 75)),
              ),
            ),
            'bliptv_preview' => array(
              'name' => 'bliptv_preview',
              'effects' => array(
                array('label' => t('Autoplay'), 'name' => 'autoplay', 'data' => array('autoplay' => 0)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 220, 'height' => 165)),
              ),
            ),
            'bliptv_full' => array(
              'name' => 'bliptv_full',
              'effects' => array(
                array('label' => t('Autoplay'), 'name' => 'autoplay', 'data' => array('autoplay' => 0)),
                array('label' => t('Resize'), 'name' => 'resize', 'data' => array('width' => 640, 'height' => 480)),
                array('label' => t('Full screen'), 'name' => 'fullscreen', 'data' => array('fullscreen' => 1)),
              ),
            ),
          ),
        ),
      ),
    ),
  );

  // Allow any image style to be applied to the thumbnail.
  foreach (image_styles() as $style_name => $image_style) {
    $styles['file']['containers']['media_bliptv']['styles']['bliptv_thumbnail_' . $style_name] = array(
      'name' => 'bliptv_thumbnail_' . $style_name,
      'image_style' => $style_name,
      'effects' => array(
        array('label' => t('Thumbnail'), 'name' => 'thumbnail', 'data' => array('thumbnail' => 1)),
      ),
    );
  }

  return $styles;
}

/**
 *  Implements hook_styles_presets().
 */
function media_bliptv_styles_presets() {
  $presets = array(
    'file' => array(
      'square_thumbnail' => array(
        'media_bliptv' => array(
          'bliptv_thumbnail_square_thumbnail',
        ),
      ),
      'thumbnail' => array(
        'media_bliptv' => array(
          'bliptv_thumbnail',
        ),
      ),
      'small' => array(
        'media_bliptv' => array(
          'bliptv_preview',
        ),
      ),
      'large' => array(
        'media_bliptv' => array(
          'bliptv_full',
        ),
      ),
      'original' => array(
        'media_bliptv' => array(
          'bliptv_full',
        ),
      ),
    ),
  );
  return $presets;
}

/**
 * Implementation of Styles module hook_styles_default_containers().
 */
function media_bliptv_styles_default_containers() {
  // We append BlipTV to the file containers.
  return array(
    'file' => array(
      'containers' => array(
        'media_bliptv' => array(
          'class' => 'MediaBlipTVStyles',
          'name' => 'media_bliptv',
          'label' => t('BlipTV'),
          'preview' => 'media_bliptv_preview_style',
        ),
      ),
    ),
  );
}


/**
 * Implementation of Styles module hook_styles_default_presets().
 */
function media_bliptv_styles_default_presets() {
  $presets = array(
    'file' => array(
      'containers' => array(
        'media_bliptv' => array(
          'default preset' => 'unlinked_thumbnail',
          'styles' => array(
            'original' => array(
              'default preset' => 'video',
            ),
            'thumbnail' => array(
              'default preset' => 'linked_thumbnail',
            ),
            'square_thumbnail' => array(
              'default preset' => 'linked_square_thumbnail',
            ),
            'medium' => array(
              'default preset' => 'linked_medium',
            ),
            'large' => array(
              'default preset' => 'large_video',
            ),
          ),
          'presets' => array(
            'video' => array(
              array(
                'name' => 'video',
                'settings' => array(),
              ),
            ),
            'large_video' => array(
              array(
                'name' => 'resize',
                'settings' => array(
                  'width' => 640,
                  'height' => 390,
                ),
              ),
              array(
                'name' => 'video',
                'settings' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
  );
  // Allow any image style to be applied to the thumbnail.
  foreach (image_styles() as $style_name => $image_style) {
    $presets['file']['containers']['media_bliptv']['presets']['linked_' . $style_name] = array(
      array(
        'name' => 'linkToMedia',
        'settings' => array(),
      ),
      array(
        'name' => 'imageStyle',
        'settings' => array(
          'image_style' => $style_name,
        ),
      ),
      array(
        'name' => 'thumbnail',
        'settings' => array(),
      ),
    );
    $presets['file']['containers']['media_bliptv']['presets']['unlinked_' . $style_name] = $presets['file']['containers']['media_bliptv']['presets']['linked_' . $style_name];
    array_shift($presets['file']['containers']['media_bliptv']['presets']['unlinked_' . $style_name]);
    foreach ($image_style['effects'] as $effect) {
      if (in_array($effect['name'], array('image_scale', 'image_scale_and_crop', 'image_resize', 'image_crop'))) {
        $presets['file']['containers']['media_bliptv']['presets']['video_' . $style_name] = array(
          array(
            'name' => 'resize',
            'settings' => $effect['data'],
          ),
          array(
            'name' => 'video',
            'settings' => array(),
          ),
        );
      }
    }
  }
  return $presets;
}
