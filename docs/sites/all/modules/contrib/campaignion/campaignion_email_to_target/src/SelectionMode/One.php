<?php

namespace Drupal\campaignion_email_to_target\SelectionMode;

/**
 * Selection mode plugin that requires to select exactly one target.
 */
class One extends Common {

  /**
   * Get form element for choosing targets and previewing/editing messages.
   */
  public function formElement(array $pairs) {
    $selector_id = drupal_html_id('select-one-target');
    $target_options = [];

    foreach ($pairs as $p) {
      list($target, $message) = $p;
      $t = $this->messageForm($target, $message);
      $t['#states']['visible']["#$selector_id"]['value'] = $target['id'];
      $element[$target['id']] = $t;
      $target_options[$target['id']] = $message->display;
    }

    $element['selector'] = [
      '#type' => 'select',
      '#title' => t('Select where to send your message'),
      '#id' => $selector_id,
      '#options' => $target_options,
      '#required' => TRUE,
      '#weight' => -1,
    ];
    return $element;
  }

  /**
   * Validate messages and unify component value.
   */
  public function getValues(array $element, $original_values) {
    $selected_id = $original_values['selector'];
    $original_values = [$selected_id => $original_values[$selected_id]];
    return parent::getValues($element, $original_values);
  }

}
