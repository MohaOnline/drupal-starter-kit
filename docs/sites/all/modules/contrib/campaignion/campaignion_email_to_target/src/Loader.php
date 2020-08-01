<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\little_helpers\Services\Container;

/**
 * Loader for selection mode plugins.
 */
class Loader extends Container {

  /**
   * Get options array for choosing a selection mode.
   *
   * @return string[]
   *   Plugin titles keyed by plugin ID.
   */
  public function options() {
    return array_map(function ($info) {
      return $info['title'];
    }, $this->specs);
  }

}
