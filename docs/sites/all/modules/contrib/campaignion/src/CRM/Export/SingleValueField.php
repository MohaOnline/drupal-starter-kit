<?php

namespace Drupal\campaignion\CRM\Export;

use Drupal\campaignion\CRM\ExporterInterface;

class SingleValueField implements ExportMapperInterface {

  protected $exporter;
  protected $key;

  public function __construct($key) {
    $this->key = $key;
  }

  /**
   * Get the value of this field.
   *
   * @param int|null $delta
   *   This parameter is ignored for this exporter.
   *
   * @return mixed
   *   Value of the entity attribute or NULL if it doesn’t exist.
   */
  public function value($delta = 0) {
    $c = $this->exporter->getContact();
    return isset($c->{$this->key}) ? $c->{$this->key} : NULL;
  }

  public function setExporter(ExporterInterface $exporter) {
    $this->exporter = $exporter;
  }

}
