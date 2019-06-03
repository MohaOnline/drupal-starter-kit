<?php

/**
 * @file
 * DVG Webform component postal code.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_postal_code() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'required' => FALSE,
    'extra' => array(
      'width' => '7',
      'disabled' => FALSE,
      'description' => '',
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_postal_code() {
  return array(
    'webform_display_postal_code' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_postal_code($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#size' => 10,
    '#weight' => -1,
  );
  $form['display']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $component['extra']['width'],
    '#description' => t('Width of the postal code field.') . ' ' . t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#weight' => 0,
    '#parents' => array('extra', 'width'),
  );
  $form['display']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#return_value' => 1,
    '#description' => t('Make this field non-editable. Useful for setting an unchangeable default value.'),
    '#weight' => 1,
    '#default_value' => $component['extra']['disabled'],
    '#parents' => array('extra', 'disabled'),
  );
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_postal_code($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'textfield',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => $filter ? _webform_filter_values($component['value'], $node, NULL, NULL, FALSE) : $component['value'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'description'),
  );

  // Handle disabling.
  if ($component['extra']['disabled']) {
    if ($filter) {
      $element['#attributes']['readonly'] = 'readonly';
    }
    else {
      $element['#disabled'] = TRUE;
    }
  }

  $element['#value_callback'] = 'dvg_webform_components_postal_code_value';

  $element['#element_validate'] = array('dvg_webform_components_postal_code_validate');

  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $element['#size'] = $component['extra']['width'];
  }

  if (isset($value)) {
    $element['#default_value'] = $value[0];
  }
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_postal_code($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_postal_code',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => $value[0],
    '#translatable' => array('title'),
  );
}

/**
 * Custom Theme function for collected postal code data.
 */
function theme_webform_display_postal_code($variables) {
  $element = $variables['element'];
  $value = $element['#value'];
  return $value;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_postal_code($component, $sids = array()) {
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
  // country using the postal_code_validation API.
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
function _webform_table_postal_code($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_postal_code($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_postal_code($component, $export_options, $value) {
  return !isset($value[0]) ? '' : $value[0];
}

/**
 * Validate postal code.
 */
function dvg_webform_components_postal_code_validate($element, &$form_state) {

  if (empty($element['#value'])) {
    return;
  }

  $result = _postal_code_validation_validate_NL($element['#value']);
  if (isset($result['error'])) {
    form_error($element, $result['error']);
  }

}

/**
 * Postal code validation.
 *
 * @codingStandardsIgnoreStart
 */
function _postal_code_validation_validate_NL($postal_code) {
  // @codingStandardsIgnoreEnd
  $return = array('country' => 'NL');
  if (preg_match('/^([1-9][0-9]{3}) ?([A-Za-z]{2})$/', $postal_code, $matches) &&
    !in_array($matches[2], array('SS', 'SD', 'SA'), TRUE)) {
    $return['postal_code'] = $matches[1] . ' ' . $matches[2];
  }
  else {
    $return['error'] = t('Invalid postal code. Postal codes in the Netherlands are like "9999 AA". They never start with zero and the letters are never "SS", "SD", or "SA".');
  }
  return $return;
}

/**
 * Filter postal code before save.
 *
 * @codingStandardsIgnoreStart
 */
function dvg_webform_components_postal_code_value($element, $input = FALSE, $form_state) {
  // @codingStandardsIgnoreEnd
  // Check for input.
  if ($input) {
    // Strip whitespace from postal codes.
    return strtoupper(str_replace(' ', '', $input));
  }
}
