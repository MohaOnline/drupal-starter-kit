<?php

/**
 * @file
 * Webform module phone number component.
 */

require_once drupal_get_path('module', 'webform') . '/components/textfield.inc';

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_phone_number() {
  $defaults = _webform_defaults_textfield();
  $defaults['extra'] += [
    'implies_optin' => FALSE,
    'optin_statement' => '',
  ];
  return $defaults;
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_phone_number($component) {
  $form = _webform_edit_textfield($component);

  $implies_id = drupal_html_id('implies-optin');
  $form['extra']['implies_optin'] = [
    '#type' => 'checkbox',
    '#title' => t('Giving a phone number implies a phone opt-in.'),
    '#default_value' => !empty($component['extra']['implies_optin']),
    '#parents' => ['extra', 'implies_optin'],
    '#id' => $implies_id,
  ];

  $form['extra']['optin_statement'] = [
    '#type' => 'textarea',
    '#title' => t('Opt-in statement'),
    '#description' => t('This opt-in statement will be recorded as part of the supporter record in Campaignion, so that you have a clear history of what the supporter has signed up to and when. Make sure it matches the visible text in the form!'),
    '#default_value' => $component['extra']['optin_statement'],
    '#parents' => ['extra', 'optin_statement'],
    '#states' => ['visible' => ["#$implies_id" => ['checked' => TRUE]]],
  ];

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_phone_number($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  return _webform_render_textfield($component, $value, $filter, $submission);
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_phone_number($component, $value, $format = 'html', $submission = array()) {
  return _webform_display_textfield($component, $value, $format, $submission);
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_phone_number($component, $sids = array(), $single = FALSE, $join = NULL) {
  return _webform_analysis_textfield($component, $sids, $single, $join);
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_phone_number($component, $value) {
  return _webform_table_textfield($component, $value);
}

/**
 * Implements _webform_action_set_component().
 */
function _webform_action_set_phone_number($component, &$element, &$form_state, $value) {
  _webform_action_set_textfield($component, $element, $form_state, $value);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_phone_number($component, $export_options) {
  return _webform_csv_headers_textfield($component, $export_options);
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_phone_number($component, $export_options, $value) {
  return _webform_csv_data_textfield($component, $export_options, $value);
}

/**
 * Implements _webform_opt_in_component().
 */
function _webform_opt_in_phone_number($component, $values) {
  if (!empty($component['extra']['implies_optin']) && reset($values)) {
    return [
      'value' => 'opt-in',
      'raw_value' => 'phone-number:opt-in',
      'channel' => 'phone',
      'statement' => $component['extra']['optin_statement'],
    ];
  }
}
