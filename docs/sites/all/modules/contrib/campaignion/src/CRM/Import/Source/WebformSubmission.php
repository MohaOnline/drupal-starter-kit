<?php

namespace Drupal\campaignion\CRM\Import\Source;

use Drupal\little_helpers\Webform\Submission;

/**
 * A SourceInterface compatible submission class.
 */
class WebformSubmission extends Submission implements SourceInterface {

  /**
   * Check whether a string starts with a prefix and get the unprefixed value.
   *
   * @param string $str
   *   The string to check.
   * @param string $prefix
   *   The prefix to test for.
   *
   * @return bool|string
   *   The unprefixed string if the input string starts with the prefix,
   *   otherwise FALSE
   */
  protected static function unprefix(string $str, string $prefix) {
    $l = strlen($prefix);
    if (substr($str, 0, $l) == $prefix) {
      return substr($str, $l);
    }
    return FALSE;
  }

  /**
   * Check whether the submission provides a value for the specified key.
   */
  public function hasKey($key) {
    if (($tracking = $this->submission->tracking ?? NULL) && ($property = static::unprefix($key, 'tracking.'))) {
      return property_exists($tracking, $property);
    }
    // Check whether a webform component with this key exists.
    return (bool) $this->webform->componentByKey($key);
  }

  /**
   * Get a value by its key.
   */
  public function value($key) {
    if (($tracking = $this->submission->tracking ?? NULL) && ($property = static::unprefix($key, 'tracking.'))) {
      return $tracking->{$property} ?? NULL;
    }
    return $this->valueByKey($key);
  }

  /**
   * Get the source’s language if available.
   *
   * @return string
   *   The language code of the source object.
   */
  public function getLanguageCode() {
    return $this->node->language;
  }

}
