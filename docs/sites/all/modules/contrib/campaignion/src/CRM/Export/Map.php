<?php

namespace Drupal\campaignion\CRM\Export;

/**
 * Wrap any exporter and map the returned values.
 */
class Map extends Wrapper {

  protected $mapping;
  protected $keepUnknown;

  /**
   * Create a new map exporter.
   *
   * @param \Drupal\campaiginon\CRM\ExportMapperInterface $wrapped
   *   The wrapped exporter.
   * @param array $mapping
   *   An array mapping values of the wrapped exporter to new values.
   * @param bool $keep_unknown
   *   If this is set to TRUE unmapped values will be returned as-is, otherwise
   *   they will be replaced with NULL.
   */
  public function __construct(ExportMapperInterface $wrapped, array $mapping, $keep_unknown = FALSE) {
    parent::__construct($wrapped);
    $this->mapping = $mapping;
    $this->keepUnknown = $keep_unknown;
  }

  /**
   * Replace values according to the mapping.
   */
  public function value($delta = 0) {
    $value = parent::value($delta);
    if (isset($this->mapping[$value])) {
      return $this->mapping[$value];
    }
    return $this->keepUnknown ? $value : NULL;
  }

}
