<?php

/**
 * @file
 * Campaignion newsletter component for webform.
 */

use Drupal\campaignion_opt_in\Values;

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_opt_in() {
  return array(
    'name' => t('Opt-in'),
    'mandatory' => 0,
    'required' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => 0,
    'extra' => array(
      'items' => '',
      'multiple' => NULL,
      'aslist' => NULL,
      'description' => '',
      'private' => FALSE,
      'title_display' => 'none',
      'display' => 'checkbox',
      'radio_labels' => [t('No'), t('Yes, please contact me.')],
      'checkbox_label' => t('Yes, please contact me.'),
      'optin_statement' => '',
      'no_is_optout' => FALSE,
      'disable_optin' => FALSE,
      'channel' => NULL,
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_opt_in($component) {
  $form['behavior'] = [
    '#type' => 'fieldset',
    '#title' => t('Behavior'),
  ];

  $form['extra']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $component['extra']['description'],
    '#weight' => 0,
    '#parents' => ['extra', 'description'],
  );

  $channels = campaignion_opt_in_channel_info();
  $channel_id = drupal_html_id('opt-in-channel');
  $form['extra']['channel'] = array(
    '#type' => 'select',
    '#title' => t('Channel'),
    '#description' => t('The communication channel this is an opt-in for.'),
    '#options' => array_map(function ($x) {
      return $x['title'];
    }, $channels),
    '#required' => TRUE,
    '#default_value' => $component['extra']['channel'],
    '#access' => empty($component['extra']['fixed_channel']) || empty($component['extra']['channel']),
    '#parents' => ['extra', 'channel'],
    // Set a fixed HTML ID so other modules can easier use #states.
    '#id' => $channel_id,
  );

  $form['behavior']['value'] = array(
    '#type' => 'select',
    '#title' => t('Default value'),
    '#options' => [
      Values::NOT_SELECTED => t('Not selected'),
      Values::OPT_IN => t('Opt-in'),
      Values::NO_CHANGE => t('No change'),
      Values::OPT_OUT => t('Opt-out'),
    ],
    '#default_value' => $component['value'],
    '#parents' => ['value'],
  );

  $display_id = drupal_html_id('opt-in-display');
  $form['extra']['display'] = [
    '#type' => 'select',
    '#title' => t('Display opt-in as …'),
    '#options' => array(
      'checkbox' => t('Checkbox'),
      'radios' => t('Radios'),
      'checkbox-inverted' => t('Inverted Checkbox'),
    ),
    '#default_value' => $component['extra']['display'],
    '#id' => $display_id,
    '#parents' => ['extra', 'display'],
  ];

  $form['extra']['checkbox_label'] = [
    '#type' => 'textfield',
    '#title' => t('Label for the checkbox'),
    '#default_value' => $component['extra']['checkbox_label'],
    '#states' => ['invisible' => ["#$display_id" => ['value' => 'radios']]],
    '#parents' => ['extra', 'checkbox_label'],
  ];

  $form['extra']['radio_labels'] = [
    // Needed for form_builder.
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Labels for the radios.'),
    '#states' => ['visible' => ["#$display_id" => ['value' => 'radios']]],
    1 => [
      '#type' => 'textfield',
      '#title' => t('Yes'),
      '#default_value' => $component['extra']['radio_labels'][1],
    ],
    0 => [
      '#type' => 'textfield',
      '#title' => t('No'),
      '#default_value' => $component['extra']['radio_labels'][0],
    ],
    '#parents' => ['extra', 'radio_labels'],
  ];

  $form['extra']['optin_statement'] = [
    '#type' => 'textarea',
    '#title' => t('Opt-in statement'),
    '#description' => t('This opt-in statement will be recorded as part of the supporter record in Campaignion, so that you have a clear history of what the supporter has signed up to and when. Make sure it matches the visible text in the form!'),
    '#default_value' => $component['extra']['optin_statement'],
    '#parents' => ['extra', 'optin_statement'],
  ];

  $no_is_opt_out_id = drupal_html_id('no-is-opt-out');
  $js_file = drupal_get_path('module', 'campaignion_opt_in') . '/campaignion_opt_in.admin.js';
  $labels = [
    'radios' => t('Interpret ‘no’ option as opt-out.'),
    'checkbox' => t('Interpret an unchecked checkbox as opt-out.'),
    'checkbox-inverted' => t('Interpret a checked inverted checkbox as opt-out.'),
  ];
  $settings['campaignionOptIn']['labels']["#$no_is_opt_out_id"] = [
    'display_id' => "#$display_id",
    'labels' => $labels,
  ];
  $attached['js'][$js_file] = [];
  $attached['js'][] = ['data' => $settings, 'type' => 'setting'];
  $form['behavior']['no_is_optout'] = [
    '#type' => 'checkbox',
    '#title' => $labels[$component['extra']['display']],
    '#default_value' => !empty($component['extra']['no_is_optout']),
    '#parents' => ['extra', 'no_is_optout'],
    '#id' => $no_is_opt_out_id,
    '#attached' => $attached,
  ];
  $form['behavior']['disable_optin'] = [
    '#type' => 'checkbox',
    '#title' => t('Only allow opt-out.'),
    '#default_value' => !empty($component['extra']['disable_optin']),
    '#parents' => ['extra', 'disable_optin'],
    // Visibility of this element is handled in campaignion_opt_in.admin.js .
  ];

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_opt_in($component, $value = NULL, $filter = TRUE) {
  $value = $value ? $value : $component['value'];
  $value = Values::removePrefix($value);
  $element = array(
    '#required' => !empty($component['mandatory']) || !empty($component['required']),
    '#weight' => $component['weight'],
    '#theme_wrappers' => array('webform_element'),
    '#pre_render' => array(),
    '#title' => $component['name'],
    '#title_display' => $component['extra']['title_display'],
    '#description' => $component['extra']['description'],
    '#translatable' => array('title', 'description', 'options'),
  );

  switch ($component['extra']['display']) {
    case 'checkbox-inverted':
    case 'checkbox':
      list($checked, $unchecked) = Values::checkboxValues($component);
      $options[$checked] = $component['extra']['checkbox_label'];
      $element += [
        '#type' => 'checkboxes',
        '#default_value' => [$value => $value],
        '#options' => $options,
        '#attributes' => ['data-no-value' => $unchecked]
      ];
      break;

    case 'radios':
      $l = $component['extra']['radio_labels'];
      $no_value = !empty($component['extra']['no_is_optout']) ? Values::OPT_OUT : Values::NO_CHANGE;
      $options = [Values::OPT_IN => $l[1], $no_value => $l[0]];
      $element += [
        '#type' => 'radios',
        '#default_value' => isset($options[$value]) ? $value : '',
        '#options' => $options,
      ];
  }
  $element['#attributes']['data-prefix'] = $component['extra']['display'];

  return $element;
}

/**
 * Implements _webform_submit_COMPONENT().
 */
function _webform_submit_opt_in($component, $value) {
  return [Values::addPrefix($value, $component)];
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_opt_in($component, $value, $format = 'html') {
  $v['#markup'] = Values::labelByValue($value);
  return $v;
}

/**
 * Implements _webform_CALLBACK_TYPE().
 *
 * Implements _webform_form_builder_map_TYPE().
 *
 * This hook allows us to extend the list of properties defined in
 * hook_form_builder_element_types() specific for form_builder_webform.
 */
function _webform_form_builder_map_opt_in() {
  $map['form_builder_type'] = 'opt_in';
  return $map;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_opt_in($component, $value) {
  return Values::labelByValue($value);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_opt_in($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_opt_in($component, $export_options, $value) {
  return Values::labelByValue($value);
}

/**
 * Webform conditional comparison callback for 'equal'.
 */
function _webform_conditional_comparison_opt_in_equal(array $input_values, $rule_value, array $component) {
  if (!$input_values && $component['extra']['display'] == 'radios') {
    return $rule_value === 'radios:' . Values::NOT_SELECTED;
  }
  foreach ($input_values as $value) {
    $value = Values::addPrefix($value, $component);
    return $rule_value == $value;
  }
  return FALSE;
}

/**
 * Webform conditional comparison callback for 'not_equal'.
 */
function _webform_conditional_comparison_opt_in_not_equal(array $input_values, $rule_value, array $component) {
  return !_webform_conditional_comparison_opt_in_equal($input_values, $rule_value, $component);
}
