<?php

namespace Drupal\campaignion\CRM\Export;

class AddressField extends WrapperField {

  protected $mappedSubkeys;

  public function __construct($key, $subkeys_map = NULL) {
    parent::__construct($key);
    $this->mappedSubkeys = (isset($subkeys_map) && is_array($subkeys_map)) ? $subkeys_map : NULL;
  }

  /**
   * Create an array based on a field item and the mapping config.
   */
  protected function map(array $item) {
    $values = [];
    if (!empty($this->mappedSubkeys)) {
      foreach ($this->mappedSubkeys as $mapped_key => $key) {
        if (isset($item[$key])) {
          $mapped_key = is_numeric($mapped_key) ? $key : $mapped_key;
          $values[$mapped_key] = $item[$key];
        }
        else {
          $values[$key] = '';
        }
      }
    }
    else {
      $values = $item;
      unset($values['data']);
      unset($values['first_name']);
      unset($values['last_name']);
      unset($values['organisation_name']);
      unset($values['name_line']);
    }
    return $values;
  }

  /**
   * Get mapped value for one or multiple address field items.
   */
  public function value($delta = 0) {
    $items = parent::value($delta);
    if ($items) {
      return is_null($delta) ? array_map([$this, 'map'], $items) : $this->map($items);
    }
    return is_null($delta) ? [] : NULL;
  }

}
