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
   * {@inheritdoc}
   */
  public function value($delta = 0) {
    $field = $this->exporter->getWrappedContact();
    foreach (explode('.', $this->key) as $part) {
      if (!$field->__isset($part)) {
        return is_null($delta) ? [] : NULL;
      }
      $field = $field->{$part};
      // Check for missing values and stop recursion if we encounter one.
      try {
        $field->value();
      }
      catch (\EntityMetadataWrapperException $e) {
        return is_null($delta) ? [] : NULL;
      }
    }
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
