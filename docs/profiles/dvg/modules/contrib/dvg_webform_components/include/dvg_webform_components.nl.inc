<?php

/**
 * @file
 * CCK Field for Dutch phone numbers.
 */

/**
 * Custom translation for language.
 *
 * @param string $phonenumber
 *   Phone number.
 *
 * @return string
 *   return custom translation string.
 */
function dvg_webform_components_nl_metadata($phonenumber) {
  return t('"@phone_number" is not a valid Dutch phone number!<br>Dutch phone numbers should contain only numbers and spaces and - and be like 099-9999999 with an optional prefix of +31.', array('@phone_number' => $phonenumber));
}

/**
 * Validate phone number.
 *
 * @param string $phonenumber
 *   Phone number.
 *
 * @return array
 *   Return matches of phone number.
 */
function _valid_nl_phone_match($phonenumber) {
  /*
   * Accepts:
   * 	06-12345678
   * 	06 123 456 78
   * 	010-1234567
   * 	+31 10-1234567
   * 	+31-10-1234567 (note: that's what phone v1.0-beta1 stored in all fields.)
   * 	+31 (0)10-1234567
   * 	+3110-1234567
   * 	020 123 4567
   * 	(020) 123 4567
   * 	0221-123456
   * 	0221 123 456
   * 	(0221) 123 456
   * Rejects:
   * 	010-12345678
   * 	05-12345678
   * 	061-2345678
   * 	(06) 12345678
   * 	123-4567890
   * 	123 456 7890
   * 	+31 010-1234567
   */

  // Area codes start with 0, +31 or the (now deprecated) '+31 (0)'.
  // Non-mobile area codes starting with 0 may be surrounded by brackets.
  foreach (
    array(
      '((?:\+31[-\s]?(?:\(0\))?\s?|0)6) # mobile
      [-\s]*
      ([1-9]\s*(?:[0-9]\s*){7})',

      '((?:\+31[-\s]?(?:\(0\))?\s?|0)[1-5789][0-9] # 3-digit area code
      | \(0[1-9][0-9]\))                        # possibly between brackets
      [-\s]*
      ([1-9]\s*(?:[0-9]\s*){6})',

      '((?:\+31[-\s]?(?:\(0\))?\s?|0)[1-5789][0-9]{2} # 3-digit area code
      |\(0[1-9][0-9]{2}\))                         # possibly between brackets
      [-\s]*
      ([1-9]\s*(?:[0-9]\s*){5})',
    ) as $regex) {

    if (preg_match('/^\s*' . $regex . '\s*$/x', $phonenumber, $matches)) {
      return $matches;
    }
  }
  return array();
}

/**
 * Check $phonenumber.
 *
 * Verifies that $phonenumber is a valid ten-digit Dutch phone number with
 * spaces and -.
 *
 * Regular expression adapted from Nico Lubbers's regex at RegExLib.com.
 *
 * @param string $phonenumber
 *   Phone number.
 *
 * @return bool
 *   FALSE if the phone number is not valid.
 */
function valid_nl_phone_number($phonenumber) {
  // Return true if valid, false otherwise.
  return (bool) _valid_nl_phone_match($phonenumber);
}

/**
 * Formatting for Dutch Phone Numbers.
 *
 * Formatting for Dutch Phone Numbers into standard area-phonenumber or with
 * extra country code +31 international format.
 *
 * @param string $phonenumber
 *   Phone number.
 *
 * @return string
 *   Formatted phone number.
 */
function format_nl_phone_number($phonenumber, $country_code, $field) {
  if ($matches = _valid_nl_phone_match($phonenumber)) {
    // $matches[1] is now 0XXX, +31 XXX, +31 (0)XXX, or (0XXX).
    $areacode = preg_replace('/(?:\+31|[-\s\(\)])/', '', $matches[1]);
    if ($areacode[0] !== '0') {
      $areacode = '0' . $areacode;
    }
    $localnumber = preg_replace('/ /', '', $matches[2]);
    $phonenumber = $areacode . '-' . $matches[2];

    // Add Country code if needed.
    if ($field) {
      $phonenumber = '+31-' . substr($areacode, 1) . '-' . $localnumber;
    }
  }

  return $phonenumber;
}
