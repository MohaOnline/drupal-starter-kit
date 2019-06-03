<?php

namespace Drupal\campaignion\CRM\Export;

/**
 * An exporter for boolean fields with configurable representation values.
 */
class BooleanField extends WrapperField {

  /**
   * Representation the boolean values in the export.
   *
   * @var string[]
   */
  protected $values;

  /**
   * Construct a new instance.
   *
   * @param string $field_name
   *   Field name of the contact field that’s being exported.
   * @param string[] $values
   *   Array with two items:
   *   1. The value that represents TRUE.
   *   2. The value that represents FALSE.
   */
  public function __construct($field_name, array $values = ['Yes', 'No']) {
    parent::__construct($field_name);
    $this->values = $values;
  }

  /**
   * Get the value for the field.
   *
   * @param int $delta
   *   Delta for multi-value fields.
   *
   * @return mixed
   *   Value representing the current state of boolean field.
   */
  public function value($delta = 0) {
    return parent::value($delta) ? $this->values[0] : $this->values[1];
  }

}
