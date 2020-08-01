<?php

namespace Drupal\campaignion\CRM\Export;

/**
 * Get a sub-value of a multi-column field (ie. address field).
 */
class DateFormat extends Wrapper {

  /**
   * Construct a new sub-field.
   *
   * @param \Drupal\campaignion\CRM\Export\ExportMapperInterface $wrapped
   *   The field that should be wrapped.
   * @param string $format
   *   The date format to use on the value.
   */
  public function __construct(ExportMapperInterface $wrapped, $format) {
    parent::__construct($wrapped);
    $this->format = $format;
  }

  /**
   * Format the date specified.
   *
   * @param int $delta
   *   Delta of a multi-value field.
   */
  public function value($delta = 0) {
    $format = function ($timestamp) {
      return strftime($this->format, $timestamp);
    };
    $value = parent::value($delta);
    if (isset($value)) {
      return is_null($delta) ? array_map($format, $value) : $format($value);
    }
  }

}

