<?php

namespace Drupal\campaignion\CRM\Export;

class TagsField extends WrapperField {

  protected $filterable;

  /**
   * Create a new instance of this exporter.
   */
  public function __construct($key, $filterable = FALSE) {
    parent::__construct($key);
    $this->filterable = $filterable;
  }

  /**
   * Get all tags in this field as comma separated string.
   *
   * @param int|null $delta
   *   This parameter is ignored for this exporter.
   *
   * @return string
   *   Comma separated tags.
   */
  public function value($delta = 0) {
    $names = array();
    foreach (parent::value(NULL) as $tag) {
      $names[] = str_replace(',', '', $tag->name);
    }
    $result = implode(',', $names);
    if ($this->filterable && $result) {
      $result = ",$result,";
    }
    return $result;
  }

}
