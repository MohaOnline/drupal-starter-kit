<?php

namespace Drupal\campaignion\CRM\Export;

use Drupal\campaignion\CRM\ExporterInterface;

/**
 * Common interface for all field exporters.
 */
interface ExportMapperInterface {

  /**
   * Get value(s) for this field.
   *
   * @param int|null $delta
   *   Specify the field item to return.
   *
   * @return mixed
   *   - NULL if the field doesn’t exist.
   *   - A single field value if $delta is given and not NULL.
   *   - All values as an array if $delta is NULL.
   */
  public function value($delta = NULL);

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
