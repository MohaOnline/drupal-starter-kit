<?php

namespace Drupal\campaignion_manage\Filter;

abstract class Base implements FilterInterface {
  /**
   * {@inheritdoc}
   */
  public function defaults() { return array(); }
  /**
   * {@inheritdoc}
   */
  public function isApplicable($current) {
    // By default filters can only be used once.
    return empty($current);
  }
  /**
   * {@inheritdoc}
   */
  public function intermediateResult(array $values) {
    return FALSE;
  }
}
