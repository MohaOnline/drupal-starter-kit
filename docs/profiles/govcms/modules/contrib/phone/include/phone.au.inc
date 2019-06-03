<?php

/**
 * @file
 * CCK Field for Australian phone numbers.
 */

function phone_au_metadata() {
  return array(
    'error' => '"%value" is not a valid Australian phone number<br>Australian phone numbers should contain only numbers with an optional prefix of "+61"',
  );
}

/**
 * Verification for Australian Phone Numbers.
 * According to http://www.itu.int/itudoc/itu-t/number/a/aus/70772.html
 *    (Released 2005/10/03, retrieved 2008/04/14)
 *
 * @param string $phonenumber
 * @return boolean Returns boolean FALSE if the phone number is not valid.
 */
function valid_au_phone_number($phonenumber) {

  $phonenumber = trim($phonenumber);

  // strip formatting chars
  $phonenumber = preg_replace('/[\-() ]/', '', $phonenumber);

  // strip optional '+61' or '0' prefixes
  $is_internationsl = preg_replace('/^(\+61|0)/', '0', $phonenumber);
  // strip optional '+61' or '0' prefixes
  $phonenumber = preg_replace('/^(\+61|0)/', '', $phonenumber);

  if (!is_numeric($phonenumber)) {
    return FALSE;
  }

  //$rules[] = array("Prefix","Minimum length","Maximum length");

  // 13 xx xx && 1300 xxx xxx
  $rules[] = array("13", 6, 6);
  $rules[] = array("13", 10, 10);
  // 18 xx xx && 1800 xxx xxx
  $rules[] = array("18", 6, 6);
  $rules[] = array("18", 10, 10);
  // 19 xx xx && 1900 xxx xxx
  $rules[] = array("19", 6, 6);
  $rules[] = array("19", 10, 10);

  // International land-line
//  $rules[] = array("2", 7, 7);
//  $rules[] = array("3", 7, 7);
//  $rules[] = array("7", 7, 7);
//  $rules[] = array("8", 7, 7);

  // Land-line
  $rules[] = array("2", 9, 9);
  $rules[] = array("3", 9, 9);
  $rules[] = array("7", 9, 9);
  $rules[] = array("8", 9, 9);

  // Mobiles
  $rules[] = array("4", 9, 9);

  foreach ($rules as $rule) {
    if (preg_match('/^'.$rule[0].'/', $phonenumber) && strlen($phonenumber) >= $rule[1] && strlen($phonenumber) <= $rule[2]) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Formatting for Australian Phone Numbers. Based upon ITU-T E.123 (but let's not get too crazy)
 *
 * @param string $phonenumber
 * @return string Returns a string containing the phone number with some formatting.
 */
function format_au_phone_number($phonenumber, $field = FALSE) {
  $prefix = '';
  $extension = '';
  // strip old formatting chars
  $phonenumber = preg_replace('/[\-() ]/', '', $phonenumber);

  /*
   * strip and save the +61 prefix if found
   */
  if (preg_match('/^\+61/', $phonenumber, $match)) {
    $prefix = '+61 ';
    $phonenumber = str_replace('+61', '', $phonenumber);
  }

  /*
   * strip and save the extension (x9999) postfix if found
   */
  if (preg_match('/(x[0-9]+)$/', $phonenumber, $match)) {
    $extension = ' ('.$match[1].')';
    $phonenumber = preg_replace('/x[0-9]+$/', '', $phonenumber);
  }

  /*
   * geographic numbers and UPT
   * Eg. (02) 9999 9999 or +61 (2) 9999 9999
   */
  if (preg_match('/^(0{0,1}[23578])([0-9]{4})([0-9]{4})$/', $phonenumber, $match)) {
    return $prefix . '(' . $match[1] . ') ' . $match[2] . ' ' . $match[3] . $extension;
  }

  /*
   * mobile numbers
   * Eg. 0423 999 999 or +61 423 999 999
   */
  if (preg_match('/^(0{0,1}4[0-9]{2})([0-9]{3})([0-9]{3})$/', $phonenumber, $match)) {
    return $prefix . $match[1] . ' ' . $match[2] . ' ' . $match[3] . $extension;
  }

  /*
   * 10 digit numbers
   * Eg. 1800 999 999
   */
  if (preg_match('/^([0-9]{4})([0-9]{3})([0-9]{3})$/', $phonenumber, $match)) {
    return $prefix . $match[1] . ' ' . $match[2] . ' ' . $match[3] . $extension;
  }

  /*
   * 9 digit satellite or dialup data numbers
   * Eg. 1471 99999
   */
  if (preg_match('/^(14[0-9]{2}|1983)([0-9]{5})$/', $phonenumber, $match)) {
    return $prefix . $match[1] . ' ' . $match[2] . $extension;
  }

  /*
   * 6 digit numbers
   * Eg. 13 99 99
   */
  if (preg_match('/^([0-9]{2})([0-9]{2})([0-9]{2})$/', $phonenumber, $match)) {
    return $prefix . $match[1] . ' ' . $match[2] . ' ' . $match[3] . $extension;
  }

  // default
  return $prefix . $phonenumber . $extension;
}

