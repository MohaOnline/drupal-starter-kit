<?php

namespace Drupal\campaignion\CRM;

/**
 * Exporter specialised for producing rows for a table (ie. CSV file).
 */
class CsvExporter extends ExporterBase {

  /**
   * Get an options array for choosing which columns of the CSV to display.
   *
   * @return string[]
   *   Array of column labels keyed by column keys.
   */
  public function columnOptions() {
    $options = [];
    foreach ($this->map as $k => $l) {
      list($h0, $h1) = [$l->header(0), $l->header(1)];
      $options[$k] = $h1 ? "$h0 ($h1)" : $h0;
    }
    return $options;
  }

  /**
   * Remove all columns from the map that are not in $fields.
   *
   * @param array $fields
   *   Associative array with keys matching a selection of column keys.
   */
  public function filterColumns(array $fields) {
    $this->map = isset($fields) ? array_intersect_key($this->map, $fields) : $this->map;
  }

  /**
   * Get header row.
   *
   * @param int $row_num
   *   Row number.
   *
   * @return string[]
   *   The header row.
   */
  public function header($row_num = 0) {
    $row = [];
    foreach ($this->map as $k => $l) {
      $row[$k] = $l->header($row_num);
    }
    return $row;
  }

  /**
   * Generate the row for the current contact.
   *
   * @return string[]
   *   Stringified field values for the current contact.
   */
  public function row() {
    $row = [];
    foreach (array_keys($this->map) as $k) {
      $row[$k] = $this->value($k);
    }
    return $row;
  }

}
