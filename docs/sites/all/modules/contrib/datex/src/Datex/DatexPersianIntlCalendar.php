<?php

/**
 * @file
 * Persian implementation of DatexInterface.
 */

/**
 * Persian implementation of DatexInterface.
 */
final class DatexPersianIntlCalendar extends DatexIntlCalendar {

  /**
   * Check to see if date granularities given in the array are valid dates.
   */
  function validate(array $arr) {
    if ((!isset($arr['year']) || empty($arr['year'])) &&
      (!isset($arr['month']) || empty($arr['month'])) &&
      (!isset($arr['day']) || empty($arr['day']))) {
      return NULL;
    }
    $zero = TRUE;
    $year = intval($arr['year']);
    $month = intval($arr['month']);
    $day = intval($arr['day']);
    if ($year < 0 || $year === 0 && $zero) {
      return t('Year out of range');
    }
    if ($month < 0 || 12 < $month || $month === 0 && $zero) {
      return t('Month out of range');
    }
    if ($day === 0 && $zero || $day < 0 || 31 < $day || $month > 6 && $day > 30 || $month === 12 && $day > 29) {
      return t('Day out of range');
    }
    return FALSE;
  }

  /**
   * Creates a clone of this object.
   */
  function copy() {
    return new DatexPersianIntlCalendar($this->timezone, $this->calendar, $this->langCode);
  }

  protected function formatHook($format, $value) {
    return $value;
  }

  function getBaseYear() {
    return 1390;
  }

}
