<?php

namespace Drupal\campaignion\CRM\Export;

use \Drupal\campaignion\CRM\ExporterInterface;

class WrapperField implements ExportMapperInterface {
  protected $exporter;
  protected $key;
  public function __construct($key) {
    $this->key = $key;
  }

  /**
   * Get value(s) for this field.
   *
   * @return mixed
   *   - NULL if the field doesn’t exist.
   *   - A single field value if $delta is given and not NULL.
   *   - All values as an array if $delta is NULL.
   */
  public function value($delta = 0) {
    $w = $this->exporter->getWrappedContact();
    if (!$w->__isset($this->key)) {
      return NULL;
    }
    $field = $w->{$this->key};
    if ($field instanceof \EntityListWrapper) {
      return is_null($delta) ? $field->value() : $field->get($delta)->value();
    }
    else {
      return is_null($delta) ? [$field->value()] : $field->value();
    }
  }

  public function setExporter(ExporterInterface $exporter) {
    $this->exporter = $exporter;
  }

  public function getFieldName() {
    return $this->key;
  }

}
