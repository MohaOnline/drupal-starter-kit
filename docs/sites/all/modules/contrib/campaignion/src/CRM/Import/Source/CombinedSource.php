<?php

namespace Drupal\campaignion\CRM\Import\Source;

class CombinedSource implements SourceInterface {
  protected $a;
  protected $b;
  public function __construct(SourceInterface $a, SourceInterface $b) {
    $this->a = $a;
    $this->b = $b;
  }

  public function hasKey($key) {
    return $this->a->hasKey() || $this->b->hasKey();
  }

  public function value($key) {
    if (!is_null($value = $this->a->value($key))) {
      return $value;
    }
    if (!is_null($value = $this->b->value($key))) {
      return $value;
    }
  }

  /**
   * Get the first non-empty language from any of the sources.
   *
   * @return string
   *   The language code of the source object.
   */
  public function getLanguageCode() {
    return $this->a->getLanguageCode() ?? $this->b->getLanguageCode();
  }

}
