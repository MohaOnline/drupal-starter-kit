<?php

/**
 * @file
 * CCK Field for International phone numbers.
 */

/**
 * Custom translation for language.
 *
 * @param string $phone_number
 *   Phone number.
 *
 * @return string
 *   return custom translation string.
 */
function dvg_webform_components_international_metadata($phone_number) {
  $args = array('@phone_number' => $phone_number);
  return t('"@phone_number" is not a valid phone number!<br>Phone numbers should contain only numbers and spaces and - and be like +3299-9999999 with an optional country prefix e.g. +32.', $args);
}


/**
 * Verifies that $phone_number is a valid ten-digit phone number with spaces and -.
 *
 * Regular expression adapted from Nico Lubbers's regex at RegExLib.com.
 *
 * @param string $phone_number
 *   Phone number.
 *
 * @return bool
 *   FALSE if the phone number is not valid.
 */
function valid_international_phone_number($phone_number) {
  // Return true if valid, false otherwise.
  $result = _dvg_webform_components_international_format_and_validate($phone_number);
  return $result['valid'];
}

/**
 * Formatting for international phone numbers.
 *
 * @param string $phonenumber
 *   Phone number.
 *
 * @return string
 *   Formatted phone number.
 */
function format_international_phone_number($phonenumber, $country_code, $field) {
  if ($phonenumber !== '') {
    $result = _dvg_webform_components_international_format_and_validate($phonenumber);
    if ($result['valid']) {
      if ($result['country_code'] === '+31' && !$field) {
        return '0' . _dvg_webform_components_international_pretty_print_localpart($result['local_part']);
      }
      else {
        return $result['country_code'] . ' ' . _dvg_webform_components_international_pretty_print_localpart($result['local_part']);
      }
    }
  }
  return $phonenumber;
}

/**
 * Prints the local part in groups of three.
 *
 * @param string $local_part
 *   Local part of the phone number.
 *
 * @return string
 */
function _dvg_webform_components_international_pretty_print_localpart($local_part) {
  $parts = str_split($local_part, 3);
  if ($parts) {
    return implode(' ', $parts);
  }
  return $local_part;
}

/**
 * Format and validate a phone number.
 *
 * @param string $number
 *   User input, to validate as a phone number.
 *
 * @return array
 *   An array of data keyed by valid, country_code and local_part.
 */
function _dvg_webform_components_international_format_and_validate($number) {
  $number = trim($number);

  $country_code = '';
  $has_country_code = FALSE;

  // Replace optional 0 indication.
  $number = preg_replace('#\(0\)#', '', $number);

  if (strpos($number, '+') === 0) {
    $has_country_code = TRUE;
    // Strip +.
    $number = substr($number, 1);
  }
  elseif (strpos($number, '00') === 0) {
    $has_country_code = TRUE;
    // Strip 00.
    $number = substr($number, 2);
  }
  $number = preg_replace('#[^0-9]#', '', $number);

  if ($has_country_code) {
    $number = '+' . $number;
  }

  $matches = array();
  $regex = '#^\+(9[976]\d|8[987530]\d|6[987]\d|5[90]\d|42\d|3[875]\d|2[98654321]\d'
    . '|9[8543210]|8[6421]|6[6543210]|5[87654321]|4[987654310]|3[9643210]|2[70]|7|1)(.*)#';
  $match = preg_match($regex, $number, $matches);

  // Country code will be in 1, rest in 2.
  if ($match) {
    $country_code = '+'. $matches[1];
    $local_part = $matches[2];
  }
  else {
    // The number has no (valid) country code.
    if (!$has_country_code) {
      $country_code = '+31';
    }
    $local_part = $number;
    if (strpos($local_part, '0') === 0) {
      $local_part = substr($local_part, 1);
    }
  }

  // Determine basic validity.
  $valid = TRUE;
  // Length.
  if ($country_code === '+357') {
    $valid = $valid && strlen($local_part) === 8;
  }
  else {
    $valid = $valid && strlen($local_part) === 9;
  }
  // No 0 as start of the local part in the international notation,
  // except for Italy.
  if ($country_code !== '+39') {
    $valid = $valid && $local_part[0] !== '0';
  }

  return array(
    'valid' => $valid,
    'country_code' => $country_code,
    'local_part' => $local_part,
  );
}
