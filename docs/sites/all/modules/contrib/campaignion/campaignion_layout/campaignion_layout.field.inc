<?php

/**
 * @file
 * Hook implementations for the our custom field type and field widget.
 */

use Drupal\little_helpers\ArrayConfig;
use Drupal\little_helpers\Services\Container;

/**
 * Implements hook_field_info().
 */
function campaignion_layout_field_info() {
  $info['layout_selection'] = [
    'label' => t('Layout selection'),
    'description' => t('This field stores a theme and layout selection.'),
    'default_widget' => 'layout_selector',
  ];
  return $info;
}

/**
 * Implements hook_field_widget_info().
 */
function campaignion_layout_field_widget_info() {
  $info['layout_selector'] = [
    'label' => t('Theme and layout selector'),
    'field types' => ['layout_selection'],
    'behaviors' => [
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'default value' => FIELD_BEHAVIOR_DEFAULT,
    ],
  ];
  return $info;
}

/**
 * Implements hook_field_widget_form().
 */
function campaignion_layout_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element += [
    '#type' => 'fieldset',
    '#element_validate' => ['_campaignion_layout_field_widget_validate'],
  ];
  $item = $items[$delta] ?? [];
  ArrayConfig::mergeDefaults($item, [
    'theme' => NULL,
    'layout' => '',
    'reversed' => 0,
  ]);
  $themes = Container::get()->loadService('campaignion_layout.themes');
  $enabled_themes = $themes->enabledThemes();
  $selected_theme = isset($enabled_themes[$item['theme']]) ? $item['theme'] : NULL;
  $toggle_id = drupal_html_id('layout_toggle');
  $element['enabled'] = [
    '#type' => 'checkbox',
    '#title' => t('Custom theme and layout.'),
    '#default_value' => $enabled_themes && $selected_theme,
    '#id' => $toggle_id,
  ];
  $toggle_states['visible']["#$toggle_id"]['checked'] = TRUE;

  // Container for the values so that they can be hidden with #states.
  $element['values'] = [
    '#type' => 'container',
    '#states' => $toggle_states,
  ];

  $theme_options = array_map(function ($theme) {
    return $theme->title();
  }, $enabled_themes);
  $theme_id = drupal_html_id('campaignion-layout-theme');
  $element['values']['theme'] = [
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('This theme is used when displaying the content.'),
    '#options' => $theme_options,
    '#default_value' => $selected_theme,
    '#id' => $theme_id,
  ];
  $layout_available_in_theme = [];
  foreach ($enabled_themes as $theme_name => $theme) {
    foreach (array_keys($theme->layouts()) as $layout) {
      $layout_available_in_theme[$layout][$theme_name] = $theme_name;
    }
  }

  $html_id = drupal_html_id('campaignion-layout_layout');
  $element['values']['layout'] = [
    '#type' => 'radios',
    '#title' => t('Layout'),
    '#description' => t('This layout is used for displaying the content.'),
    '#id' => $html_id,
  ];
  $fields = [];
  $available_layouts = array_intersect_key($themes->declaredLayouts(), $layout_available_in_theme);
  $options = [];
  $reversable_layouts = [];
  foreach ($available_layouts as $layout => $info) {
    $options[$layout] = $info['title'];
    foreach (array_keys(array_filter($info['fields'])) as $field_name) {
      $fields[$field_name]["#$html_id input"][] = $layout;
    }
    $element['values']['layout'][$layout]['#disabled'] = TRUE;
    foreach ($layout_available_in_theme[$layout] ?? [] as $theme_name) {
      $element['values']['layout'][$layout]['#states']['enabled']["#$theme_id"][]['value'] = $theme_name;
    }
    if (!empty($info['reversable'])) {
      $reversable_layouts[$layout] = $layout;
    }
  }
  $element['values']['layout'] += [
    '#options' => $options,
    '#default_value' => isset($options[$item['layout']]) ? $item['layout'] : NULL,
  ];
  $element['values']['reversed'] = [
    '#type' => 'radios',
    '#title' => t('Order on the mobile page'),
    '#options' => [
      0 => t('Show form above the main page content'),
      1 => t('Show form below the main page content'),
    ],
    '#default_value' => $item['reversed'],
  ];
  foreach ($reversable_layouts as $layout) {
    $element['values']['reversed']['#states']['enabled']["#$html_id input"][]['value'] = $layout;
  }
  $form_state['campaignion_layout_fields'] = $fields;
  if (!$enabled_themes) {
    $element['#access'] = FALSE;
  }
  return $element;
}

/**
 * Validation callback for the field widget.
 */
function _campaignion_layout_field_widget_validate($element, &$form_state, $form) {
  $values = [
    'theme' => NULL,
    'layout' => NULL,
    'reversed' => 0,
  ];
  if ($element['enabled']['#value']) {
    $values['theme'] = $element['values']['theme']['#value'];
    $values['layout'] = $element['values']['layout']['#value'];
    $values['reversed'] = $element['values']['reversed']['#value'];
  }
  form_set_value($element, $values, $form_state);
}

/**
 * Implements hook_field_is_empty().
 */
function campaignion_layout_field_is_empty($item, $field) {
  return empty($item['theme']);
}
