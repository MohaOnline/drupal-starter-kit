<?php

/**
 * @file
 * Base implementation of DatexInterface.
 */

/**
 * Base implementation of DatexInterface.
 */
abstract class DatexPartialImplementation implements DatexInterface {

  protected $origin;

  protected $timezone;

  protected $calendar;

  protected $langCode;

  /**
   * Creates a new DatexPartialImplementation.
   */
  function __construct($tz, $calendar, $lang_code) {
    $this->timezone = $tz;
    $this->origin = new DateTime('now', $this->timezone);
    $this->calendar = $calendar;
    $this->langCode = $lang_code;
  }

  /**
   * Get name of the calendar, such as gregorian or persian.
   */
  final function getCalendarName() {
    return $this->calendar;
  }

  /**
   * Get various list options, such as month names for a form select element.
   */
  function listOptions($name, $required) {
    $none = ['' => ''];
    $year = $this->getBaseYear();
    switch ($name) {
      case 'monthNames':
        $m = [];
        for ($i = 1; $i < 13; $i++) {
          $this->setDateLocale($year, $i, 1);
          $m[$i] = $this->format('F');
        }
        return !$required ? $none + $m : $m;

      case 'monthNamesAbbr':
        $m = [];
        for ($i = 1; $i < 13; $i++) {
          $this->setDateLocale($year, $i, 1);
          $m[$i] = $this->format('M');
        }
        return !$required ? $none + $m : $m;
    }
  }

  abstract function getBaseYear();

  /**
   * Get two letter language code of this object.
   */
  function getLangcode() {
    return $this->langCode;
  }

  protected function getOrigin() {
    return $this->origin;
  }

  // ------------------------------------ FORMAT

  /**
   * Format date time, in gregorian.
   *
   * @param $format
   *
   * @return string
   */
  final function xFormat($format) {
    return $this->origin->format($format);
  }

  /**
   * Put all day and time parts in an array, in gregorian.
   *
   * @return array
   */
  final function xFormatArray() {
    return [
      'year' => intval($this->origin->format('Y')),
      'month' => intval($this->origin->format('n')),
      'day' => intval($this->origin->format('j')),
      'hour' => intval($this->origin->format('G')),
      'minute' => intval($this->origin->format('i')),
      'second' => intval($this->origin->format('s')),
    ];
  }

  /**
   * Set date (in Gregorian) on this object.
   */
  final function xSetDate($y, $m, $d) {
    $this->origin->setDate($y, $m, $d);
    return $this;
  }

  /**
   * Set timestamp on this object.
   */
  final function setTimestamp($timestamp) {
    $this->origin->setTimestamp($timestamp);
    return $this;
  }

  /**
   * Get timestamp of this object.
   */
  final function getTimestamp() {
    return $this->origin->getTimestamp();
  }

  /**
   * To be implemented by subclasses.
   */
  function validate(array $arr) {
    return NULL;
  }

  /**
   * Set time on this object.
   */
  final function setTime($hour, $minute, $second) {
    $this->origin->setTime($hour, $minute, $second);
    return $this;
  }

  /**
   * Format date parts into an array.
   */
  final function formatArray() {
    return [
      'year' => $this->format('Y'),
      'month' => $this->format('n'),
      'day' => $this->format('j'),
      'hour' => $this->format('G'),
      'minute' => $this->format('i'),
      'second' => $this->format('s'),
    ];
  }

  final protected function tz($tz) {
    $this->origin = new DateTime('@' . $this->origin->getTimestamp(), $tz);
  }

}
