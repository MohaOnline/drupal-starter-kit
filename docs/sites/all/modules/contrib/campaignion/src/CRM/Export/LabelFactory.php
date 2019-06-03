<?php

namespace Drupal\campaignion\CRM\Export;

use Drupal\little_helpers\Field\Bundle;

/**
 * Factory to create column labels based on field labels.
 */
class LabelFactory {

  public function __construct($entity_type, $bundle) {
    $this->entity_type = $entity_type;
    $this->bundle = $bundle;
  }

  public function fromField($field_name, $exporter) {
    $instance = field_info_instance($this->entity_type, $field_name, $this->bundle);
    return new Label($instance['label'], $exporter);
  }

  public function fromExporter($exporter) {
    return $this->fromField($exporter->getFieldName(), $exporter);
  }

}
