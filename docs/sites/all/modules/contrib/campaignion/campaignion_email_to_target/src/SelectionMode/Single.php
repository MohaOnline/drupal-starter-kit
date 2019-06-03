<?php

namespace Drupal\campaignion_email_to_target\SelectionMode;

/**
 * Selection mode plugin used when there is only one target.
 */
class Single extends Common {

  /**
   * Get message edit form for a single target/message pair.
   */
  protected function messageForm($target, $message) {
    $t = parent::messageForm($target, $message);
    $t['#attributes']['class'][] = 'email-to-target-single';
    $t['send'] = [
      '#type' => 'markup',
      '#markup' => "<p class=\"target\">{$message->display} </p>",
      '#weight' => -1,
    ];
    return $t;
  }

}
