<?php

namespace Drupal\campaignion\CRM\Export;

/**
 * Proxy class to annotate exported fields with labels.
 */
class Label {

  protected $label;
  protected $field;

  /**
   * Create a new instance.
   *
   * @param string $label
   *   The field’s label.
   * @param object $field
   *   The field that should be wrapped.
   */
  public function __construct($label, $field) {
    $this->label = $label;
    $this->field = $field;
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

  /**
   * Forward all other calls to the field.
   */
  public function __call($method, $params) {
    return call_user_func_array([$this->field, $method], $params);
  }

}
