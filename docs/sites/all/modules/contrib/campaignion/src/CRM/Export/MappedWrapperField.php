<?php

namespace Drupal\campaignion\CRM\Export;

/**
 * Wrapper field that allows mapping of values.
 *
 * @deprecated since 2.9. Use a WrapperField inside a Map instead.
 */
class MappedWrapperField extends WrapperField {
  protected $map;
  protected $filter;

  public function __construct($key, $map, $filter_unmapped = TRUE) {
    parent::__construct($key);
    $this->map = $map;
    $this->filter = $filter_unmapped;
  }

  public function value($delta = 0) {
    $value = parent::value($delta);
    if (isset($this->map[$value])) {
      return $this->map[$value];
    }
    elseif (!$this->filter) {
      return $value;
    }
  }
}
