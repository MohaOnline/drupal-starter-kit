<?php

/**
 * @file
 * CKEditor integration for the entity_embed module.
 */

/**
 * Implements hook_ckeditor_plugin().
 */
function entity_embed_ckeditor_plugin() {
  $plugins = array(
    'drupalentity' => array(
      'name' => 'drupalentity',
      'desc' => t('Plugin for embedding entities'),
      'path' => base_path() . drupal_get_path('module', 'entity_embed') . '/js/plugins/drupalentity/',
      'default' => 'f',
    ),
  );

  // Add user-defined buttons.
  $embed_buttons = entity_embed_embed_button_load_all();

  $buttons = array();

  foreach ($embed_buttons as $embed_button) {
    $icon = 'entity.png';
    $icon_path = FALSE;

    // Use a custom icon when available.
    if ($fid = $embed_button->button_icon_fid) {
      if ($file = file_load($fid)) {
        // We are unable to use _entity_embed_button_image() because CKEditor
        // requires a specific format for icon paths.
        if ($wrapper = file_stream_wrapper_get_instance_by_uri($file->uri)) {
          $icon = file_uri_target($file->uri);
          $icon_path = base_path() . $wrapper->getDirectoryPath() . '/';
        }
      }
    }

    $buttons[$embed_button->name] = array(
      'label' => check_plain($embed_button->button_label),
      'icon' => $icon,
      'icon_path' => $icon_path,
    );
  }

  $plugins['drupalentity']['buttons'] = $buttons;

  return $plugins;
}

/**
 * Implements hook_ckeditor_settings_alter().
 */
function entity_embed_ckeditor_settings_alter(&$settings, $conf) {
  // Add theme-independent styles for aligning content.
  $settings['contentsCss'][] = base_path() . drupal_get_path('module', 'entity_embed') . '/css/entity_embed.css';

  // Add base styles for accessibility, clearing, etc.
  $settings['contentsCss'][] = base_path() . drupal_get_path('module', 'system') . '/system.base.css';

  // Add styles for handling contextual links.
  if (module_exists('contextual')) {
    $settings['contentsCss'][] = base_path() . drupal_get_path('module', 'contextual') . '/contextual.css';
  }
}
