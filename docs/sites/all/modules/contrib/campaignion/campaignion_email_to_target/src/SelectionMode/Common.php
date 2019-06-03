<?php

namespace Drupal\campaignion_email_to_target\SelectionMode;

/**
 * Common functionality for all/most selection module plugins.
 */
abstract class Common {

  protected $editable;

  /**
   * Construct a new selection mode plugin.
   */
  public function __construct($editable) {
    $this->editable = $editable;
  }

  /**
   * Get selection mode plugin to use when only one target is found.
   */
  public function singleMode() {
    return new Single($this->editable);
  }

  /**
   * Get message editing form for a single message.
   */
  protected function messageForm($target, $message) {
    $t = [
      '#type' => 'container',
      '#attributes' => ['class' => ['email-to-target-target']],
      '#message' => $message->toArray(),
      '#target' => $target,
    ];
    $t['subject'] = [
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#default_value' => $message->subject,
      '#disabled' => !$this->editable,
    ];
    $t['header'] = [
      '#prefix' => '<pre class="email-to-target-header">',
      '#markup' => check_plain($message->header),
      '#suffix' => '</pre>',
    ];
    $t['message'] = [
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#default_value' => $message->message,
      '#disabled' => !$this->editable,
    ];
    $t['footer'] = [
      '#prefix' => '<pre class="email-to-target-footer">',
      '#markup' => check_plain($message->footer),
      '#suffix' => '</pre>',
    ];
    return $t;
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
      $e = $element[$id];
      $values[] = serialize([
        'message' => $edited_message + $e['#message'],
        'target' => $e['#target'],
      ]);
    }
    return $values;
  }

}
