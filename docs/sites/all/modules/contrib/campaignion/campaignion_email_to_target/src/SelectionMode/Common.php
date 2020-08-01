<?php

namespace Drupal\campaignion_email_to_target\SelectionMode;

/**
 * Common functionality for all/most selection module plugins.
 */
abstract class Common {

  protected $editable;
  protected $channel;

  /**
   * Construct a new selection mode plugin.
   */
  public function __construct($editable, $channel) {
    $this->editable = $editable;
    $this->channel = $channel;
  }

  /**
   * Get selection mode plugin to use when only one target is found.
   */
  public function singleMode() {
    return new Single($this->editable, $this->channel);
  }

  /**
   * Get message editing form for a single message.
   */
  protected function messageForm($target, $message) {
    return $this->channel->messageForm($target, $message, $this->editable);
  }

  /**
   * Get form element for choosing targets and previewing/editing messages.
   */
  public function formElement(array $pairs) {
    foreach ($pairs as $p) {
      list($target, $message) = $p;
      $element[$target['id']] = $this->messageForm($target, $message);
    }
    return $element;
  }

  /**
   * Validate messages and unify component value.
   */
  public function getValues(array $element, $original_values) {
    $values = [];
    foreach ($original_values as $id => $edited_message) {
      $values[] = serialize($this->channel->value($edited_message, $element[$id]));
    }
    return $values;
  }

}
