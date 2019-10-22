<?php

namespace Drupal\campaignion\CRM\Import\Field;

/**
 * Importer for phone numbers.
 */
class Phone extends Field {

  /**
   * Remove extra characters from a phone number for comparison.
   */
  protected function normalizePhoneNumber($number) {
    $number = trim($number);
    if (strpos($number, '+') === 0) {
      $number = '00' . substr($number, 1);
    }
    $number = preg_replace('/[^0-9]/', '', $number);
    return ltrim($number, '0');
  }

  /**
   * Determine whether two strings mean the same phone number.
   */
  protected function phoneNumbersEqual($short, $long) {
    $short = $this->normalizePhoneNumber($short);
    $long = $this->normalizePhoneNumber($long);
    if (strlen($short) > strlen($long)) {
      list($short, $long) = array($long, $short);
    }
    return substr($long, strlen($long) - strlen($short)) == $short;
  }

  /**
   * Determine whether a new value should be stored.
   */
  public function storeValue($entity, $new_number) {
    return TRUE;
  }

  /**
   * Update the field value.
   *
   * New or confirmed values are moved to the top.
   */
  public function setValue(\EntityMetadataWrapper $entity, $value) {
    $field = $entity->{$this->field};
    $values = $field->value();
    $first = TRUE;
    foreach ($values as $delta => $stored_value) {
      if ($this->phoneNumbersEqual($stored_value, $value)) {
        if ($first) {
          return FALSE;
        }
        unset($values[$delta]);
        $new_values = array_values($values);
        array_unshift($new_values, $stored_value);
        $field->set($new_values);
        return TRUE;
      }
      $first = FALSE;
    }
    array_unshift($values, $value);
    $field->set($values);
    return TRUE;
  }

}
