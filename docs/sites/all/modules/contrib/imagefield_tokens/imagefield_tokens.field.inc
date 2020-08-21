<?php

/**
 * @file
 * Implements additional formatter for ImageField Tokens.
 */

/**
 * Implements hook_field_formatter_info().
 */
function imagefield_tokens_field_formatter_info() {
  $formatters = [
    'imagefield_tokens' => [
      'label' => t('ImageField Tokens'),
      'field types' => ['image'],
      'settings' => ['image_style' => '', 'image_link' => ''],
    ],
  ];

  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function imagefield_tokens_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $image_styles = image_style_options(FALSE, PASS_THROUGH);
  $element['image_style'] = [
    '#title' => t('Image style'),
    '#type' => 'select',
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
    '#options' => $image_styles,
  ];

  $link_types = [
    'content' => t('Content'),
    'file' => t('File'),
  ];
  $element['image_link'] = [
    '#title' => t('Link image to'),
    '#type' => 'select',
    '#default_value' => $settings['image_link'],
    '#empty_option' => t('Nothing'),
    '#options' => $link_types,
  ];

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function imagefield_tokens_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = [];

  $image_styles = image_style_options(FALSE, PASS_THROUGH);
  // Unset possible 'No defined styles' option.
  unset($image_styles['']);
  // Styles could be lost because of enabled/disabled modules that defines
  // their styles in code.
  if (isset($image_styles[$settings['image_style']])) {
    $summary[] = t('Image style: @style', ['@style' => $image_styles[$settings['image_style']]]);
  }
  else {
    $summary[] = t('Original image');
  }

  $link_types = [
    'content' => t('Linked to content'),
    'file' => t('Linked to file'),
  ];
  // Display this setting only if image is linked.
  if (isset($link_types[$settings['image_link']])) {
    $summary[] = $link_types[$settings['image_link']];
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function imagefield_tokens_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = [];

  // Check if the formatter involves a link.
  if ($display['settings']['image_link'] == 'content') {
    $uri = entity_uri($entity_type, $entity);
  }
  elseif ($display['settings']['image_link'] == 'file') {
    $link_file = TRUE;
  }

  foreach ($items as $delta => $item) {
    if (isset($link_file)) {
      $uri = [
        'path' => file_create_url($item['uri']),
        'options' => [],
      ];
    }
    $element[$delta] = [
      '#theme' => 'image_formatter',
      '#item' => $item,
      '#image_style' => $display['settings']['image_style'],
      '#path' => isset($uri) ? $uri : '',
    ];
  }
  return $element;
}

/**
 * Implements hook_field_formatter_prepare_view().
 */
function imagefield_tokens_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  foreach ($entities as $id => $entity) {
    if (!empty($items[$id][0]) && isset($items[$id][0]['is_default'])) {
      if (!empty($instances[$id]['settings']['alt_field']) &&
          !empty($instances[$id]['settings']['alt_field_default'])) {
        $alt_field_default = $instances[$id]['settings']['alt_field_default'];
      }
      if (!empty($instances[$id]['settings']['title_field']) &&
          !empty($instances[$id]['settings']['title_field_default'])) {
        $title_field_default = $instances[$id]['settings']['title_field_default'];
      }
    }

    // Truncate fields to max allowed length.
    // alt=512, title=1024 per image.install.
    if (!empty($alt_field_default)) {
      $alt_field_default = token_replace($alt_field_default, [$entity_type => $entity], ['sanitize' => TRUE, 'clear' => TRUE]);
      $alt_field_default = truncate_utf8($alt_field_default, 1024, TRUE, TRUE);
      $alt_field_default = htmlspecialchars_decode($alt_field_default, ENT_QUOTES | ENT_HTML401);
    }
    if (!empty($title_field_default)) {
      $title_field_default = token_replace($title_field_default, [$entity_type => $entity], ['sanitize' => TRUE, 'clear' => TRUE]);
      $title_field_default = truncate_utf8($title_field_default, 1024, TRUE, TRUE);
      $title_field_default = htmlspecialchars_decode($title_field_default, ENT_QUOTES | ENT_HTML401);
    }
    if (isset($items[$id][0]['alt']) && isset($alt_field_default)) {
      $items[$id][0]['alt'] = $alt_field_default;
    }
    if (isset($items[$id][0]['title']) && isset($title_field_default)) {
      $items[$id][0]['title'] = $title_field_default;
    }
  }
}
