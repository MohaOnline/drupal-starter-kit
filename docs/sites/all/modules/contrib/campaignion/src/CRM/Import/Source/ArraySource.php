<?php

namespace Drupal\campaignion\CRM\Import\Source;

class ArraySource implements SourceInterface {

  protected $data;

  /**
   * A pre configured language code.
   *
   * @var string|null
   */
  protected $language;

  public function __construct($data, string $language = null) {
    $this->data = $data;
    $this->language = $language;
  }

  public function hasKey($key) {
    return array_key_exists($key, $this->data);
  }

  public function value($key) {
    return isset($this->data[$key]) ? $this->data[$key] : NULL;
  }

  /**
   * Return the configured language.
   *
   * @return string|null
   *   The language code of the configured language.
   */
  public function getLanguageCode() {
    return $this->language;
  }

}
