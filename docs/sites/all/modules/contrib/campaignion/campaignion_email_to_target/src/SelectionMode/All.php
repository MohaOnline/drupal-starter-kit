<?php

namespace Drupal\campaignion_email_to_target\SelectionMode;

/**
 * Selection mode that requires that all messages are sent.
 */
class All extends Common {

  /**
   * Get message edit form for a single target/message pair.
   */
  protected function messageForm($target, $message) {
    $t = parent::messageForm($target, $message);
    $t['#attributes']['class'][] = 'email-to-target-all';
    $t['send'] = [
      '#type' => 'markup',
      '#markup' => "<p class=\"target\">{$message->display} </p>",
      '#weight' => -1,
    ];
    return $t;
  }

  /**
   * Get form element for choosing targets and previewing/editing messages.
   */
  public function formElement(array $pairs) {
    $element = parent::formElement($pairs);
    $element['#attached']['js'] = [drupal_get_path('module', 'campaignion_email_to_target') . '/js/target_selector.js'];
    return $element;
  }

}
