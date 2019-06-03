<?php

namespace Drupal\campaignion\CRM\Export;

/**
 * Exporter for a single tag field.
 */
class TagField extends WrapperField {

  /**
   * Get the value of this field.
   *
   * @return string|null
   *   The name of the tag if one is set otherwise NULL.
   */
  public function value() {
    $w = $this->exporter->getWrappedContact();
    $tag = $w->{$this->key}->value();
    if ($tag) {
      return $tag->name;
    }
  }

}
