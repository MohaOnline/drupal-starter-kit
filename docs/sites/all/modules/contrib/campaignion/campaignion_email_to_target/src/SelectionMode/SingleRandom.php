<?php

namespace Drupal\campaignion_email_to_target\SelectionMode;

/**
 * Selection mode plugin that selects one random target.
 */
class SingleRandom extends Single {

  /**
   * Get form element for a randomly selected target.
   */
  public function formElement(array $pairs) {
    $random_key = array_rand($pairs);
    return parent::formElement([$random_key => $pairs[$random_key]]);
  }

}
