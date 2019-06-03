<?php

namespace Drupal\campaignion\CRM\Export;

use \Drupal\campaignion\CRM\ExporterInterface;

interface ExportMapperInterface {
  /**
   * Get the export-value of the mapped values.
   *
   * @return mixed
   */
  public function value();
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
  public function setExporter(ExporterInterface $export);
}
