<?php

/**
 * @file
 * DVG Webform component phone number.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_phone_number() {
  return array(
    'name'      => '',
    'form_key'  => NULL,
    'required'  => FALSE,
    'pid'       => 0,
    'weight'    => 0,
    'value'     => '',
    'extra'     => array(
      'width'                      => '',
      'disabled'                   => FALSE,
      'placeholder'                => '',
      'description'                => '',
      'private'                    => FALSE,
      'country'                    => 'nl',
      'phone_country_code'         => 0,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_phone_number() {
  return array(
    'webform_display_phone_number' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_phone_number($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#size' => 10,
    '#weight' => -1,
  );
  $form['display']['placeholder'] = array(
    '#type' => 'textfield',
    '#title' => t('Placeholder'),
    '#default_value' => $component['extra']['placeholder'],
    '#description' => t('The placeholder will be shown in the field until the user starts entering a value.'),
    '#weight' => 1,
    '#parents' => array('extra', 'placeholder'),
  );
  $form['display']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $component['extra']['width'],
    '#description' => t('Width of the phone number field.') . ' ' . t('Leaving blank will use the default size.'),
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
  $form['extra']['country'] = array(
    '#type'          => 'select',
    '#title'         => t('Country'),
    '#options'       => dvg_webform_components_phone_number_countries(),
    '#default_value' => $component['extra']['country'],
    '#description'   => t('Which country-specific rules should this field be validated against and formatted according to.'),
    '#required'      => TRUE,
  );
  $form['extra']['phone_country_code'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Add the country code if not filled by the user'),
    '#default_value' => $component['extra']['phone_country_code'],
  );
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_phone_number($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'textfield',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => $filter ? webform_replace_tokens($component['value'], $node, NULL, NULL, FALSE) : $component['value'],
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

  // Handle placeholder.
  if ($component['extra']['placeholder']) {
    $element['#attributes']['placeholder'] = $component['extra']['placeholder'];
  }

  $element['#value_callback'] = 'dvg_webform_components_phone_number_value';

  $element['#element_validate'] = array('dvg_webform_components_phone_number_validate');

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
function _webform_display_phone_number($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_phone_number',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => $value[0],
    '#translatable' => array('title'),
  );
}

/**
 * Custom Theme function for collected phone number data.
 */
function theme_webform_display_phone_number($variables) {
  $element = $variables['element'];
  $value = $element['#value'];
  return $value;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_phone_number($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_phone_number($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_phone_number($component, $export_options, $value) {
  return !isset($value[0]) ? '' : $value[0];
}

/**
 * Validate phone number.
 */
function dvg_webform_components_phone_number_validate($element, &$form_state) {
  if (empty($element['#value']) && trim($element['#value'], '0') === $element['#value']) {
    return;
  }

  $result = _phone_number_validation_validate($element['#value'], $element['#webform_component']['extra']['country']);
  if (!empty($result)) {
    form_error($element, $result);
  }
}

/**
 * Format phone number before save.
 *
 * @codingStandardsIgnoreStart
 */
function dvg_webform_components_phone_number_value($element, $input = FALSE, $form_state) {
  // @codingStandardsIgnoreEnd
  // Check for input.
  if ($input !== FALSE && is_scalar($input)) {
    // Strip whitespace from phone number.
    $input = str_replace(array('.', ',', ' '), '', trim($input));
    $ccode = trim($element['#webform_component']['extra']['country']);
    return format_phone_number($ccode, $input, $element['#webform_component']['extra']['phone_country_code']);
  }
}

/**
 * Check phone number.
 *
 * @param string $phone_nr
 *   Phone number.
 * @param string $ccode
 *   Country code.
 *
 * @return string
 *   Error message or empty string
 */
function _phone_number_validation_validate($phone_nr, $ccode) {
  $phone_nr = str_replace(array('.', ',', ' '), '', trim($phone_nr));
  $ccode = trim($ccode);

  // Phone number: check length.
  $phone_length = strlen($phone_nr);
  if ($phone_length < 10 && $phone_length > 0) {
    return t('The entered phone number "@phone_number" has to contain at least 10 digits.', array('@phone_number' => $phone_nr));
  }

  // Check if country code exist.
  if (!dvg_webform_components_phone_number_supported_countrycode($ccode)) {
    return dvg_webform_components_phone_number_country_info($ccode, $phone_nr);
  }

  // Run through 'countries phone' validation.
  if ($phone_length > 0 && !valid_phone_number($ccode, $phone_nr)) {
    return t('The entered phone number "@phone_number" does not to be appear valid.', array('@phone_number' => $phone_nr));
  }
  return '';
}

/**
 * Returns an array of supported country codes.
 *
 * @return array
 *   Returns an array of supported country codes.
 */
function dvg_webform_components_phone_number_countries() {
  // @todo Expose via hook.
  return array(
    // @todo Convert to proper classes (with inheritance) instead of obscurely crafted function calls.
    'nl' => t('Netherlands'),
    'international' => t('International'),
  );
}

/**
 * Verification for Phone Numbers.
 *
 * @param string $countrycode
 *   Country number.
 * @param string $phonenumber
 *   Phone number.
 *
 * @return bool
 *   Returns boolean FALSE if the phone number is not valid.
 */
function valid_phone_number($countrycode, $phonenumber) {
  if (dvg_webform_components_phone_number_supported_countrycode($countrycode)) {
    $valid_phone_function = 'valid_' . $countrycode . '_phone_number';
    module_load_include('inc', 'dvg_webform_components', 'include/dvg_webform_components.' . $countrycode);

    if (function_exists($valid_phone_function)) {
      return $valid_phone_function($phonenumber);
    }
  }
  // Country not taken into account yet.
  return FALSE;
}

/**
 * Formatting for Phone Numbers.
 *
 * @param string $countrycode
 *   Country code.
 * @param string $phonenumber
 *   Phone number.
 *
 * @return bool
 *   Returns boolean FALSE if the phone number is not valid.
 */
function format_phone_number($countrycode, $phonenumber, $field) {
  if (dvg_webform_components_phone_number_supported_countrycode($countrycode)) {
    $format_phone_function = 'format_' . $countrycode . '_phone_number';
    module_load_include('inc', 'dvg_webform_components', 'include/dvg_webform_components.' . $countrycode);

    if (function_exists($format_phone_function)) {
      return $format_phone_function($phonenumber, $countrycode, $field);
    }
  }
  // Country not taken into account yet.
  return FALSE;
}

/**
 * Country supported?
 *
 * @param string $countrycode
 *   Country code.
 *
 * @return bool
 *   TRUE if the countrycode is supported.
 */
function dvg_webform_components_phone_number_supported_countrycode($countrycode) {
  return (bool) dvg_webform_components_phone_number_country_info($countrycode);
}

/**
 * Get a country meta info.
 *
 * @param string $countrycode
 *   Country code.
 *
 * @param string|null $phone_nr
 *   Phone number passed to the country code's info function.
 *   Currently does nothing.
 *
 * @return string|bool
 *   Returns a string containing a error message if the country code exists
 *   (and as if the phone number is invalid).
 *   Return FALSE if country does not exist.
 *
 * @todo Rename + refactor this function to better describe what it is used for
 *   and make its use more logical. Currently checks if a country code is known.
 *   If it is known, returns an error message assuming the $phone_nr is invalid.
 */
function dvg_webform_components_phone_number_country_info($countrycode, $phone_nr = NULL) {
  $countrycode = trim($countrycode);

  if (isset(dvg_webform_components_phone_number_countries()[$countrycode])) {
    // @todo Rename this function to better describe what it is used for.
    //   It currently simply returns an error message as if the phone_nr is invalid,
    //   without actually validating the phone_nr.
    $phone_info_function = 'dvg_webform_components_' . $countrycode . '_metadata';
    module_load_include('inc', 'dvg_webform_components', 'include/dvg_webform_components.' . $countrycode);

    if (function_exists($phone_info_function)) {
      return $phone_info_function($phone_nr);
    }
  }

  // Unknown country code.
  return FALSE;
}
