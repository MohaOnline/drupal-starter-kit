<?php
/**
 * @file
 * A specification for the custom tabbed interface entity that is part of
 * WetKit Widgets.
 */


/**
 * Implements hook_field_formatter_info().
 */
function wetkit_widgets_field_formatter_info() {
  return array(
    'file_wetkit_video' => array(
      'label' => t('WetKit Video'),
      'description' => t('Render the file using an HTML5 video tag'),
      'field types' => array('file'),
      'settings' => array(
        'controls' => TRUE,
        'autoplay' => FALSE,
        'loop' => FALSE,
        'muted' => FALSE,
        'size' => array(
          'width' => NULL,
          'height' => NULL,
        ),
        'multiple_file_behaviour' => 'sources',
      ),
    ),
    'wetkit_widgets_lightbox' => array(
      'label' => t('WetKit Lightbox'),
      'field types' => array('image'),
      'settings' => array(
        'image_style_destination' => 'large',
        'image_style_source' => 'thumbnail',
        'custom_class' => '',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function wetkit_widgets_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  if ($display['type'] === 'file_wetkit_video') {
    $summary_items = array();
    if (isset($settings['multiple_file_behaviour'])) {
      $summary_items[] = t('Multiple files: %multiple', array('%multiple' => $settings['multiple_file_behaviour']));
    }
    if (isset($settings['controls'])) {
      $summary_items[] = t('Controls: %controls', array('%controls' => $settings['controls'] ? 'visible' : 'hidden'));
    }
    if (isset($settings['autoplay'])) {
      $summary_items[] = t('Autoplay: %autoplay', array('%autoplay' => $settings['autoplay'] ? t('yes') : t('no')));
    }
    if (isset($settings['loop'])) {
      $summary_items[] = t('Loop: %loop', array('%loop' => $settings['loop'] ? t('yes') : t('no')));
    }
    if (isset($settings['muted'])) {
      $summary_items[] = t('Muted: %muted', array('%muted' => $settings['muted'] ? t('yes') : t('no')));
    }
    if ($settings['size']['width'] && $settings['size']['height']) {
      $summary_items[] = t('Size: %width x %height', array('%width' => $settings['size']['width'], '%height' => $settings['size']['height']));
    }
    $summary = implode('<br />', $summary_items);
  }
  elseif ($display['type'] == 'wetkit_widgets_lightbox') {
    // Destination image style.
    $value = empty($settings['image_style_destination']) ? '<em>' . t('Empty') . '</em>' : $settings['image_style_destination'];
    $value = is_array($value) ? implode(', ', array_filter($value)) : $value;
    $summary .= "Destination image style: {$value}<br />";
    // Source image style.
    $value = empty($settings['image_style_source']) ? '<em>' . t('Empty') . '</em>' : $settings['image_style_source'];
    $value = is_array($value) ? implode(', ', array_filter($value)) : $value;
    $summary .= "Source image style: {$value}<br />";
    // Custom Class.
    $value = empty($settings['custom_class']) ? '<em>' . t('Empty') . '</em>' : $settings['custom_class'];
    $value = is_array($value) ? implode(', ', array_filter($value)) : $value;
    $summary .= "Custom Class: {$value}<br />";
  }

  return $summary;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function wetkit_widgets_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  if ($display['type'] == 'file_wetkit_video') {
    $element['controls'] = array(
      '#title' => t('Show video controls'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['controls']) ? $settings['controls'] : TRUE,
    );
    $element['autoplay'] = array(
      '#title' => t('Autoplay'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['autoplay']) ? $settings['autoplay'] : FALSE,
    );
    $element['loop'] = array(
      '#title' => t('Loop'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['loop']) ? $settings['loop'] : FALSE,
    );
    $element['size'] = array(
      '#type' => 'item',
      '#title' => t('Size'),
      '#field_prefix' => '<div class="container-inline">',
      '#field_suffix' => '</div>',
      '#description' => t('The size expressed as WIDTHxHEIGHT (for example, 640x480). Leave blank to use the video file to determine the size of the player.'),
    );
    $element['size']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#title_display' => 'invisible',
      '#default_value' => isset($settings['size']['width']) ? $settings['size']['width'] : NULL,
      '#size' => 5,
      '#maxlength' => 5,
      '#field_suffix' => ' x ',
    );
    $element['size']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#title_display' => 'invisible',
      '#default_value' => isset($settings['size']['height']) ? $settings['size']['height'] : NULL,
      '#size' => 5,
      '#maxlength' => 5,
      '#field_suffix' => ' ' . t('pixels'),
    );
    $element['multiple_file_behaviour'] = array(
      '#title' => t('Display of multiple files'),
      '#type' => 'radios',
      '#options' => array(
        'sources' => t('Use multiple sources within a single <video> tag'),
        'tags' => t('Use multiple <video> tags, each with a single source'),
      ),
      '#default_value' => isset($settings['multiple_file_behaviour']) ? $settings['multiple_file_behaviour'] : 'sources',
    );

  }
  elseif ($display['type'] == 'wetkit_widgets_lightbox') {
    $element['image_style_destination'] = array(
      '#weight' => '1',
      '#default_value' => $settings['image_style_destination'],
      '#multiple_toggle' => '1',
      '#key_type_toggled' => '1',
      '#options' => image_style_options(),
      '#type' => 'select',
      '#title' => t('Destination image style'),
    );
    $element['image_style_source'] = array(
      '#options' => image_style_options(),
      '#default_value' => $settings['image_style_source'],
      '#type' => 'select',
      '#weight' => '1',
      '#multiple_toggle' => '1',
      '#title' => t('Source image style'),
      '#key_type_toggled' => '1',
    );
    $element['custom_class'] = array(
      '#title' => t('Custom Class'),
      '#default_value' => $settings['custom_class'],
      '#required' => '0',
      '#type' => 'textfield',
      '#description' => t('Should you wish to define a custom class for this element such as a span please declare it here.'),
      '#weight' => '2',
      '#size' => '20',
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_view().
 */
function wetkit_widgets_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];
  $formatter = $display['type'];

  if ($formatter == 'file_wetkit_video') {
    $multiple_file_behaviour = isset($settings['multiple_file_behaviour']) ? $settings['multiple_file_behaviour'] : 'sources';

    // Prevent 'empty' fields from causing a WSOD.
    $items = array_filter($items);

    // Build an array of sources for each <video> element.
    $source_lists = array();
    if ($multiple_file_behaviour == 'tags') {
      foreach ($items as $delta => $item) {
        if ($item['type'] == 'wetkit_video') {
          $source_lists[] = array($item);
        }
      }
    }
    else {
      foreach ($items as $delta => $item) {
        if ($item['type'] == 'wetkit_video') {
          $source_lists[0][] = $item;
        }
      }
    }

    // Render each source list as an <video> element.
    foreach ($source_lists as $delta => $sources) {
      $width = isset($settings['size']['width']) ? $settings['size']['width'] : NULL;
      $height = isset($settings['size']['height']) ? $settings['size']['height'] : NULL;
      $element[$delta] = array(
        '#theme' => 'wetkit_widgets_file_video',
        '#files' => $sources,
        '#controls' => isset($settings['controls']) ? $settings['controls'] : TRUE,
        '#autoplay' => isset($settings['autoplay']) ? $settings['autoplay'] : FALSE,
        '#loop' => isset($settings['loop']) ? $settings['loop'] : FALSE,
        '#muted' => isset($settings['muted']) ? $settings['muted'] : FALSE,
        '#width' => ($width && $height) ? $width : NULL,
        '#height' => ($width && $height) ? $height : NULL,
      );
    }
  }
  elseif ($display['type'] == 'wetkit_widgets_lightbox') {
    // Build variables array for formatter.
    $variables = array(
      '#obj_type' => $entity_type,
      '#object' => $entity,
      '#field' => $field,
      '#instance' => $instance,
      '#langcode' => $langcode,
      '#items' => $items,
      '#display' => $display,
    );

    if (function_exists($function = "{$display['module']}_field_formatter_{$display['type']}")) {
      $element[0] = array(
        '#markup' => $function($variables),
      );
    }
  }

  return $element;
}

/**
 * Field Formatter; WetKit Lightbox.
 */
function wetkit_widgets_field_formatter_wetkit_widgets_lightbox($variables) {
  // Formatter settings.
  $settings = $variables['#display']['settings'];
  $output = '';
  foreach (element_children($variables['#items']) as $delta) {
    $item = $variables['#items'][$delta];

    // Take into account multilingual.
    $file = entity_load('file', array($item['fid']));
    global $language;
    foreach ($file as $single_file) {
      $field_file_alt = field_get_items('file', $single_file, 'field_file_image_alt_text', $language->language);
      if (isset($field_file_alt)) {
        $alt = $field_file_alt[0]['value'];
      }
      $field_file_title = field_get_items('file', $single_file, 'field_file_image_title_text', $language->language);
      if (isset($field_file_title)) {
        $title = $field_file_title[0]['value'];
      }
    }

    $source = theme('image_style', array(
      'alt' => $alt,
      'style_name' => $settings['image_style_source'],
      'path' => $item['uri'],
    ));
    $destination = $item['uri'];
    $output .= '<li>' . l($source, file_create_url($destination), array(
      'html' => TRUE,
      'attributes' => array(
        'class' => array('lb-item'),
        'title' => isset($title) ? $title : '',
      ),
    )) . '</li>';
  }
  $class = $settings['custom_class'];
  return '<section class="wb-lbx lbx-gal ' . $class . '"><ul class="list-inline">' . $output . '</ul></section>';
}
