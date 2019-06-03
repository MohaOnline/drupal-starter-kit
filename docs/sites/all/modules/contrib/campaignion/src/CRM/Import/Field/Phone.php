<?php

namespace Drupal\campaignion\CRM\Import\Field;

class Phone extends Field {

  protected function normalizePhoneNumber($number) {
    $number = trim($number);
    if (strpos($number, '+') === 0) {
      $number = '00' . substr($number, 1);
    }
    $number = preg_replace('/[^0-9]/', '', $number);
    return ltrim($number, '0');
  }

  protected function phoneNumbersEqual($short, $long) {
    $short = $this->normalizePhoneNumber($short);
    $long = $this->normalizePhoneNumber($long);
    if (strlen($short) > strlen($long)) {
      list($short, $long) = array($long, $short);
    }
    return substr($long, strlen($long)-strlen($short)) == $short;
  }

  // returns the array offset when it finds the number
  // or NULL otherwise
  public function storeValue($entity, $newNumber) {
    try {
      foreach ($entity->{$this->field}->value() as $delta => $storedNumber) {
        if ($this->phoneNumbersEqual($storedNumber, $newNumber)) {
          return FALSE;
        }
      }
    } catch (\EntityMetadataWrapperException $e) {
      watchdog('campaignion', 'Searched data in a non-existing field "!field".', array('!field' => $this->field), WATCHDOG_WARNING);
      return TRUE;
    }

    // the number wasn't found
    return TRUE;
  }

  public function setValue(\EntityMetadataWrapper $entity, $value) {
    $field = $entity->{$this->field};
    $values = $field->value();
    $values[] = $value;
    $field->set($values);
    return TRUE;
  }
}
