<?php

/**
 * @file
 * DVG Webform component postal code.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_readonly() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'disabled' => FALSE,
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_readonly() {
  return array(
    'webform_display_readonly' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_readonly($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 1028,
    '#weight' => -1,
  );
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_readonly($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
  $value = $filter ? _webform_filter_values($component['value'], $node, NULL, NULL, FALSE) : $component['value'];
  $element = array(
    '#type' => 'textfield',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_readonly',
    '#required' => $component['required'],
    '#format' => 'html',
    '#default_value' => $value,
    '#maxlength' => 1028,
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'description'),
  );

  $element['#value_callback'] = 'dvg_webform_components_readonly_value';

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_readonly($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_readonly',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => !isset($value[0]) ? '' : $value[0],
    '#translatable' => array('title'),
  );
}

/**
 * Custom Theme function for collected postal code data.
 */
function theme_webform_display_readonly($variables) {
  $element = $variables['element'];
  $value = nl2br($element['#value']);
  return $value;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_readonly($component, $sids = array()) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid']);

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $nonblanks = 0;
  $submissions = 0;

  // This could probably be extended to count submissions by
  // country using the readonly_validation API.
  $result = $query->execute();
  foreach ($result as $data) {
    if (drupal_strlen(trim($data['data'])) > 0) {
      $nonblanks++;
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);
  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_readonly($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_readonly($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_readonly($component, $export_options, $value) {
  return !isset($value[0]) ? '' : $value[0];
}

/**
 * Change value to default value before save.
 */
function dvg_webform_components_readonly_value($element, $input = FALSE, $form_state = FALSE) {
  $value = preg_replace('/\<br(\s*)?\/?\>/i', "\n", $element['#default_value']);
  return check_plain($value);
}
