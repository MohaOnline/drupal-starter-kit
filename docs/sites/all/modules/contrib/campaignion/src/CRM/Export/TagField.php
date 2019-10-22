<?php

namespace Drupal\campaignion\CRM\Export;

/**
 * Exporter for a single tag field.
 */
class TagField extends WrapperField {

  /**
   * Get the value of this field.
   *
   * @param int|null $delta
   *   This parameter is ignored for this exporter.
   *
   * @return string|null
   *   The name of the tag if one is set otherwise NULL.
   */
  public function value($delta = 0) {
    if ($tag = parent::value()) {
      return $tag->name;
    }
  }

}
