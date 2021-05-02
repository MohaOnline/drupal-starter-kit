<?php

namespace Drupal\campaignion\CRM\Import\Field;

use \Drupal\campaignion\CRM\Import\Source\SourceInterface;

/**
 * Import values into fields.
 */
class Field {
  protected $source;
  protected $field;
  public function __construct($field, $source = NULL) {
    $this->field = $field;
    $this->source = $source ? $source : $field;
    if (!is_array($this->source)) {
      $this->source = array($this->source);
    }
  }

  protected static function valueFromSource(SourceInterface $source, $keys) {
    foreach ($keys as $key) {
      if($value = $source->value($key)) {
        return $value;
      }
    }
  }
  protected function getValue(SourceInterface $source) {
    return static::valueFromSource($source, $this->source);
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
      if (($value = $this->getValue($source)) && ($value = $this->preprocessField($value))) {
        if ($this->storeValue($entity, $value)) {
          return $this->setValue($entity, $value);
        }
        return FALSE;
      }
    }
    catch (\EntityMetadataWrapperException $e) {
      watchdog_exception('campaignion', $e, 'Error when importing into !field', ['!field' => $this->field], WATCHDOG_WARNING);
    }
    return FALSE;
  }

  protected function setValue(\EntityMetadataWrapper $entity, $value) {
    $entity->{$this->field}->set($value);
    return TRUE;
  }

  protected function preprocessField($value) {
    return $value;
  }

  protected function storeValue($entity, $value) {
    return $entity->{$this->field}->value() != $value;
  }
}
