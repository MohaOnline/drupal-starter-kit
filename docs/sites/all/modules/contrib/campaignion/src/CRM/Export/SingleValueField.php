<?php

namespace Drupal\campaignion\CRM\Export;

use \Drupal\campaignion\CRM\ExporterInterface;

class SingleValueField implements ExportMapperInterface {
  protected $exporter;
  protected $key;
  public function __construct($key) {
    $this->key = $key;
  }

  public function value() {
    $c = $this->exporter->getContact();
    return isset($c->{$this->key}) ? $c->{$this->key} : NULL;
  }

  public function setExporter(ExporterInterface $exporter) {
    $this->exporter = $exporter;
  }
}
