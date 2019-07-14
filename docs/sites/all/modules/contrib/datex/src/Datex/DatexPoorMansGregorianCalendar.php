<?php

/**
 * @file
 * Fallback calendar implementation in case php-intl is not available.
 */

final class DatexPoorMansGregorianCalendar extends DatexPartialImplementation implements DatexInterface {

  public function __construct($tz, $lang_code) {
    $lang_code = $lang_code !== 'fa' && $lang_code !== 'en' ? 'en' : $lang_code;
    parent::__construct($tz, 'gregorian', $lang_code);
  }

  public function format($format) {
    return date_format(parent::getOrigin(), $format);
  }

  public function setDateLocale($y = 1, $m = 1, $d = 1) {
    $this->xSetDate($y, $m, $d);
    return $this;
  }

  public function copy() {
    return new DatexPoorMansGregorianCalendar($this->timezone, $this->langCode);
  }

  public function validate(array $arr) {
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
    if ($day === 0 && $zero || $day < 0 || 31 < $day) {
      return t('Day out of range');
    }
    return FALSE;
  }

  public function parse($value, $format) {
    $dt = DateTime::createFromFormat($format, $value);
    if (!$dt) {
      return FALSE;
    }
    $this->setTimestamp($dt->getTimestamp());
    return TRUE;
  }

  function getBaseYear() {
    return 2018;
  }

}
