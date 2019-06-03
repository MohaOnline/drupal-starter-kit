<?php

namespace Drupal\campaignion\CRM\Export;

class AddressField extends WrapperField {
  protected $mappedSubkeys;

  public function __construct($key, $subkeys_map = NULL) {
    parent::__construct($key);
    $this->mappedSubkeys = (isset($subkeys_map) && is_array($subkeys_map)) ? $subkeys_map : NULL;
  }

  public function value() {
    $all_values = parent::value();
    unset($all_values['data']);
    unset($all_values['first_name']);
    unset($all_values['last_name']);
    unset($all_values['organisation_name']);
    unset($all_values['name_line']);

    $values = array();
    if (!empty($this->mappedSubkeys)) {
      foreach ($this->mappedSubkeys as $mapped_key => $key) {
        if (isset($all_values[$key])) {
          if (!is_numeric($mapped_key)) {
            $values[$mapped_key] = $all_values[$key];
          }
          else {
            $values[$key] = $all_values[$key];
          }
        }
        else {
          $values[$key] = '';
        }
      }
    }
    else {
      $values = $all_values;
    }

    return $values;
  }
}
