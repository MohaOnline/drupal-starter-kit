<?php

namespace Drupal\campaignion\CRM\Import\Field;

use Drupal\campaignion\CRM\Import\Source\SourceInterface;

class Address extends Field {

  public function __construct($field, $mapping) {
    foreach ($mapping as $target => $keys) {
      $mapping[$target] = is_array($keys) ? $keys : array($keys);
    }
    parent::__construct($field, $mapping);
  }

  /**
   * Trim and normalize the source value.
   */
  protected static function valueFromSource(SourceInterface $source, $keys) {
    if ($value = parent::valueFromSource($source, $keys)) {
      return preg_replace('/\s+/', ' ', trim($value));
    }
  }

  public function getValue(SourceInterface $source) {
    $address = array();
    foreach ($this->source as $target => $keys) {
      $value = static::valueFromSource($source, $keys);
      if ($value) {
        $address[$target] = $value;
      }
    }
    if (empty($address)) {
      return FALSE;
    }
    $countryList = country_get_list();
    $empty_or_unknown_country = empty($address['country']) || !isset($countryList[$address['country']]);
    if ($empty_or_unknown_country && ($c = variable_get('site_default_country', 'AT'))) {
      $address['country'] = $c;
    }
    return $address;
  }

  public function storeValue($entity, $new_address) {
    return TRUE;
  }

  public function import(SourceInterface $source, \EntityMetadataWrapper $entity) {
    try {
      if (($value = $this->getValue($source)) && ($value = $this->preprocessField($value))) {
        if ($this->storeValue($entity, $value)) {
          return $this->setValue($entity, $value);
        } else {
          return FALSE;
        }
      }
    } catch (\EntityMetadataWrapperException $e) {
      watchdog('campaignion', 'Tried to import into a non-existing field "!field".', array('!field' => $this->field), WATCHDOG_WARNING);
    }
    return FALSE;
  }

  public function setValue(\EntityMetadataWrapper $entity, $new_address) {
    $field = $entity->{$this->field};
    if ($field instanceof \EntityListWrapper) {
      return $this->setValueMultiple($field, $new_address);
    }
    else {
      return $this->setValueSingle($field, $new_address);
    }
  }

  /**
   * Merges a new address into an existing single-value address field.
   *
   * @param \EntityListWrapper $item
   *   The metadata wrapper for the singe-value address field.
   * @param array $address
   *   Associative array representing the address to be merged.
   *
   * @param bool
   *   TRUE if any field value has been changed, FALSE if no changes were made.
   */
  protected function setValueSingle(\EntityStructureWrapper $item, array $address) {
    $stored = $item->value();
    if ($stored && $this->addressIsMergeable($stored, $address)) {
      return $this->mergeAddress($item, $address);
    }
    // Existing address contradicts the new one. So just set the new one.
    $item->set($address);
    return TRUE;
  }

  /**
   * Merges a new address into a multi-address field.
   *
   * @param \EntityListWrapper $items
   *   The metadata wrapper for the multi-value address field.
   * @param array $address
   *   Associative array representing the address to be merged.
   *
   * @param bool
   *   TRUE if any field value has been changed, FALSE if no changes were made.
   */
  protected function setValueMultiple(\EntityListWrapper $items, array $address) {
    foreach($items as $item) {
      if ($this->addressIsMergeable($item->value(), $address)) {
        return $this->mergeAddress($item, $address);
      }
    }
    // We found no matching address so we add it as a new one.
    $items[] = $address;
    return TRUE;
  }

  /**
   * Check whether the second address can be merged into the first address item.
   *
   * @param array $a1
   *   The first address.
   * @param array $a2
   *   The scond address.
   *
   * @return bool
   *   TRUE when the first address contains no non-NULL values that differ from
   *   the second address.
   */
  protected function addressIsMergeable($a1, $a2) {
    foreach ($a2 as $key => $value) {
      if (isset($a1[$key]) && $a1[$key] != $value) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Merges a new address into an existing address field item.
   *
   * @param \EntityListWrapper $item
   *   The metadata wrapper for the address field item.
   * @param array $address
   *   Associative array representing the address to be merged.
   *
   * @param bool
   *   TRUE if any field value has been changed, FALSE if no changes were made.
   */
  protected function mergeAddress(\EntityStructureWrapper $item, $address) {
    $stored = $item->value();
    if (!array_diff($address, $stored)) {
      // New address doesn’t add new information. Nothing to do.
      return FALSE;
    }
    $item->set(array_merge($stored, $address));
    return TRUE;
  }

}
