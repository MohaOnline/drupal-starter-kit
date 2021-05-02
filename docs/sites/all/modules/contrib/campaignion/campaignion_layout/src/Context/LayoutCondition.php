<?php

namespace Drupal\campaignion_layout\Context;

use Drupal\little_helpers\Services\Container;
use Drupal\campaignion_layout\Lookup;

/**
 * Expose available layouts as a context condition.
 */
class LayoutCondition extends \context_condition {

  /**
   * Condition values.
   */
  public function condition_values() {
    return Container::get()->loadService('campaignion_layout.themes')->layoutOptions();
  }

  /**
   * Check whether the condition is met.
   *
   * @param string $active_layout_name
   *   Machine name of the active layout if one was set.
   */
  public function execute(string $active_layout_name) {
    foreach ($this->get_contexts($active_layout_name) as $context) {
      $this->condition_met($context, $active_layout_name);
    }
  }

}
