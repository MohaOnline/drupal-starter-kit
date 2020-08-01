<?php

namespace Drupal\campaignion\CRM\Export;

use Drupal\campaignion\CRM\ExporterInterface;

/**
 * Simple exporter that wraps another exporter.
 *
 * Sub-classes will likely override at least the value method.
 */
class Wrapper implements ExportMapperInterface {

  protected $wrapped;

  /**
   * Create a new wrapper instance.
   */
  public function __construct(ExportMapperInterface $wrapped) {
    $this->wrapped = $wrapped;
  }

  /**
   * Get value(s) for the wrapped exporter.
   */
  public function value($delta = 0) {
    return $this->wrapped->value($delta);
  }

  /**
   * Set the reference to the exporter object.
   *
   * This is usually called while constructing the Exporter.
   * We later use the exporter's functions getContact() and getWrappedContact()
   * to access the contact's values.
   *
   * @param \Drupal\campaignion\ExporterInterface $export
   *   The exporter that's used to access the contact objects.
   */
  public function setExporter(ExporterInterface $export) {
    $this->wrapped->setExporter($export);
  }

  /**
   * Forward all other function calls to the wrapped exporter.
   */
  public function __call($method, $params) {
    return call_user_func_array([$this->wrapped, $method], $params);
  }

}
