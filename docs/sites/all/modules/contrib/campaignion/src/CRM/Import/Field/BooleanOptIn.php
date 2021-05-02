<?php

namespace Drupal\campaignion\CRM\Import\Field;

use Drupal\campaignion\CRM\Import\Source\SourceInterface;
use Drupal\campaignion_opt_in\Values;

/**
 * Importer for importing an opt-in component into a boolean field.
 */
class BooleanOptIn extends Field {

  /**
   * Normalize values to boolean values.
   */
  protected function preprocessField($value) {
    $value = Values::removePrefix($value);
    if (in_array($value, [Values::OPT_IN, Values::OPT_OUT])) {
      return $value == Values::OPT_IN ? TRUE : FALSE;
    }
  }

  /**
   * Decide whether the value needs storing.
   */
  protected function storeValue($entity, $value) {
    return !is_null($value) && $entity->{$this->field}->value() !== $value;
  }

  /**
   * Imports data from source into entity.
   *
   * @param DataSource $source
   *   Source to import from
   * @param EntityMetadataWrapper $entity
   *   Entity that stores imported data
   *
   * @return bool
   *   TRUE if at least one value of the entity was changed.
   */
  public function import(SourceInterface $source, \EntityMetadataWrapper $entity) {
    try {
      $value = $this->preprocessField($this->getValue($source));
      if ($this->storeValue($entity, $value)) {
        return $this->setValue($entity, $value);
      } else {
        return FALSE;
      }
    }
    catch (\EntityMetadataWrapperException $e) {
      watchdog_exception('campaignion', $e, 'Error when importing into !field', ['!field' => $this->field], WATCHDOG_WARNING);
    }
    return FALSE;
  }

}
