<?php

namespace Drupal\campaignion_email_to_target\SelectionMode;

/**
 * Selection mode plugin that allows selecting one or more targets.
 */
class OneOrMore extends Common {

  /**
   * Get message edit form for a single target/message pair.
   */
  protected function messageForm($target, $message) {
    $t = parent::messageForm($target, $message);
    $t['send'] = [
      '#type' => 'checkbox',
      '#title' => $message->display,
      '#default_value' => TRUE,
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

  /**
   * Validate messages and unify component value.
   */
  public function getValues(array $element, $original_values) {
    $original_values = array_filter($original_values, function ($edited) {
      return !empty($edited['send']);
    });
    $values = parent::getValues($element, $original_values);
    if (empty($values)) {
      form_error($element, t('Please select at least one target'));
    }
    return $values;
  }

}
