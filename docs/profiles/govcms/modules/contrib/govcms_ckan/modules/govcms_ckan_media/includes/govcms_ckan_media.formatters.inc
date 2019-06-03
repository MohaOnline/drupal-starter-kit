<?php
/**
 * @file
 * Formatters for CKAN datasets.
 */

/**
 * Implements hook_file_formatter_info().
 *
 * The basic file formatters for both preview and the file, the preview is
 * displayed as a static image and the file is a link to the public dataset.
 */
function govcms_ckan_media_file_formatter_info() {

  $formatters['govcms_ckan_media_image'] = array(
    'label' => t('CKAN Preview Image'),
    'file types' => array('ckan'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'govcms_ckan_media_file_formatter_image_view',
    'mime types' => array('other/ckan'),
  );

  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 *
 * The formatter for the preview image.
 */
function govcms_ckan_media_file_formatter_image_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);
  if ($scheme == 'ckan') {
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    // Create a thumbnail placeholder from a static image.
    return array(
      '#theme' => 'image',
      '#path' => $wrapper->getThumbnailPath(),
      '#alt' => isset($file->override['attributes']['alt']) ? $file->override['attributes']['alt'] : $file->filename,
    );
  }
}

/**
 * Implements hook_field_formatter_info().
 *
 * This is the formatter for the visualisation. The config field is rendered
 * using this formatter.
 */
function govcms_ckan_media_field_formatter_info() {
  return array(
    'govcms_ckan_media_config_formatter' => array(
      'label' => t('Visualisation'),
      'field types' => array('govcms_ckan_media_config'),
      'view callback' => 'govcms_ckan_media_field_formatter_view',
      'mime types' => array('other/ckan'),
      'file types' => array('ckan'),
      'settings' => array(
        'export_width' => NULL,
        'export_height' => NULL,
        'palette' => NULL,
        'x_axis_label_position' => 'inner-right',
        'y_axis_label_position' => 'inner-top',
        'x_axis_tick_centered' => 1,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 *
 * The formatter for the config field that gets rendered as the visualisation.
 */
function govcms_ckan_media_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'govcms_ckan_media_config_formatter':
      foreach ($items as $delta => $item) {
        // Load the config so we can include the correct renderer.
        $config = _govcms_ckan_media_get_visualisation_config($entity);
        if (isset($config['visualisation'])) {
          $element[$delta] = govcms_ckan_get_plugin_view($config['visualisation'], $entity, $display, $config['visualisation_config']);
        }
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_form().
 *
 * Generic field wide formatter settings for the visualisation.
 */
function govcms_ckan_media_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  // This form only applies to this formatter.
  if ($display['type'] != 'govcms_ckan_media_config_formatter') {
    return $element;
  }

  $element['export_width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#default_value' => $settings['export_width'],
    '#element_validate' => array('element_validate_integer'),
    '#description' => t('A pixel value (without unit). Applied when exporting as an image. Leave blank for auto sizing.'),
  );
  $element['export_height'] = array(
    '#title' => t('Height'),
    '#type' => 'textfield',
    '#default_value' => $settings['export_height'],
    '#element_validate' => array('element_validate_integer'),
    '#description' => t('A pixel value (without unit). Applied when exporting as an image. Leave blank for auto sizing.'),
  );

  // TODO: Add validator.
  $element['palette'] = array(
    '#title' => t('Palette override'),
    '#type' => 'textfield',
    '#default_value' => $settings['palette'],
    '#description' => t('Palette is a comma separated list of hex values. If not set, default palette is applied.'),
  );

  $element['x_axis_label_position'] = array(
    '#title' => t('X axis label position'),
    '#type' => 'select',
    '#default_value' => $settings['x_axis_label_position'],
    '#options' => array(
      'inner-right' => t('Inner right'),
      'inner-center' => t('Inner center'),
      'inner-left' => t('Inner left'),
      'outer-right' => t('Outer right'),
      'outer-center' => t('Outer center'),
      'outer-left' => t('Outer left'),
    ),
  );

  $element['y_axis_label_position'] = array(
    '#title' => t('Y axis label position'),
    '#type' => 'select',
    '#default_value' => $settings['y_axis_label_position'],
    '#options' => array(
      'inner-top' => t('Inner top'),
      'inner-middle' => t('Inner middle'),
      'inner-bottom' => t('Inner bottom'),
      'outer-top' => t('Outer top'),
      'outer-middle' => t('Outer middle'),
      'outer-bottom' => t('Outer bottom'),
    ),
  );

  $element['x_axis_tick_centered'] = array(
    '#title' => t('Center X axis tick above label'),
    '#description' => t('When enabled the tick will display directly above the label on a the X axis.'),
    '#type' => 'checkbox',
    '#default_value' => $settings['x_axis_tick_centered'],
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function govcms_ckan_media_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $summary = array();
  // Use the form to populate the labels.
  $form_state = array();
  $config_form = govcms_ckan_media_field_formatter_settings_form($field, $instance, $view_mode, array(), $form_state);
  // Dynamically create summary from settings.
  foreach ($display['settings'] as $key => $value) {
    if (!empty($value)) {
      $summary[] = $config_form[$key]['#title'] . ': ' . filter_xss($value);
    }
  }
  return !empty($summary) ? implode('<br />', $summary) : t('Default display');
}
