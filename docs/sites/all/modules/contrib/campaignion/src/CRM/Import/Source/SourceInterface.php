<?php

namespace Drupal\campaignion\CRM\Import\Source;

/**
 * Interface for objects, that can be used to import data from.
 */
interface SourceInterface {

  /**
   * Check whether the source knows a certain $key.
   *
   * @param string $key
   *   The key to check for.
   *
   * @return bool
   *   Whether the key exists.
   */
  public function hasKey($key);

  /**
   * Get the value for a key.
   *
   * @param string $key
   *   The key to get a value for.
   */
  public function value($key);

  /**
   * Get the source’s language if available.
   *
   * @return string
   *   The language code of the source object.
   */
  public function getLanguageCode();

}
