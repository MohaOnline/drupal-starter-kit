<?php

namespace Drupal\campaignion\CRM\Export;

/**
 * Get a sub-value of a multi-column field (ie. address field).
 */
class SubField extends Wrapper {

  protected $key;
  protected $label;

  /**
   * Construct a new sub-field.
   *
   * @param \Drupal\campaignion\CRM\Export\ExportMapperInterface $wrapped
   *   The field that should be wrapped.
   * @param string $key
   *   Export this sub-key of the field’s value array.
   * @param string $label
   *   Translated label for the column (if exported to a table).
   */
  public function __construct(ExportMapperInterface $wrapped, $key, $label = '') {
    parent::__construct($wrapped);
    $this->key = $key;
    $this->label = $label;
  }

  /**
   * Get the sub-key value for the current contact.
   *
   * @param int $delta
   *   Delta of a multi-value field.
   */
  public function value($delta = 0) {
    $all_values = parent::value($delta);
    return isset($all_values[$this->key]) ? $all_values[$this->key] : NULL;
  }

  /**
   * Return the header label for a specific header row.
   *
   * @param int $row_num
   *   The header row that should be rendered.
   */
  public function header($row_num = 0) {
    if ($row_num == 1) {
      return $this->label;
    }
    return $this->wrapped->header($row_num);
  }

}
