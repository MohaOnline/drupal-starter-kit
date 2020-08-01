<?php

namespace Drupal\campaignion\CRM\Export;

use Drupal\campaignion\CRM\ExporterInterface;

/**
 * Exporter that always gives a fixed value.
 */
class FixedValue implements ExportMapperInterface {

  /**
   * Create a new export instance.
   */
  public function __construct($label, $value) {
    $this->label = $label;
    $this->value = $value;
  }

  /**
   * Return the header label for a specific header row.
   *
   * @param int $row_num
   *   The header row that should be rendered.
   */
  public function header($row_num = 0) {
    if ($row_num == 0) {
      return $this->label;
    }
    return '';
  }

  /**
   * Get value(s) for this exporter.
   *
   * @param int|null $delta
   *   Specify the array key to return.
   *
   * @return mixed
   *   - A single array item if the value is an array and $delta is not NULL.
   *   - The value passed to the constructor (default).
   */
  public function value($delta = NULL) {
    return !is_null($delta) && is_array($this->value) ? ($this->value[$delta] ?? NULL) : $this->value;
  }

  /**
   * Set the exporter.
   */
  public function setExporter(ExporterInterface $export) {
    // No need to access the contact’s data so nothing to do here.
  }

}
