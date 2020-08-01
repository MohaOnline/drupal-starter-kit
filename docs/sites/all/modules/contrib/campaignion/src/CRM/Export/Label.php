<?php

namespace Drupal\campaignion\CRM\Export;

/**
 * Proxy class to annotate exported fields with labels.
 */
class Label extends Wrapper {

  protected $label;

  /**
   * Create a new instance.
   *
   * @param string $label
   *   The field’s label.
   * @param \Drupal\campaignion\CRM\Export\ExportMapperInterface $field
   *   The field that should be wrapped.
   */
  public function __construct($label, ExportMapperInterface $wrapped) {
    parent::__construct($wrapped);
    $this->label = $label;
  }

  /**
   * Get the header value for this field.
   *
   * @param int $row_num
   *   Get the header value for this header row.
   */
  public function header($row_num = 0) {
    if ($row_num == 0) {
      return $this->label;
    }
    return '';
  }

}
