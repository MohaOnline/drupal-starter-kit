<?php

namespace Drupal\campaignion_email_to_target\Channel;

use Drupal\little_helpers\Webform\Submission;

use Drupal\campaignion_email_to_target\Message;

/**
 * Channel plugin for doing nothing.
 */
class NoOp {

  /**
   * Create a new instance based on some config.
   */
  public static function fromConfig(array $config) {
    return new static();
  }

  /**
   * Send email to one target.
   *
   * @param \Drupal\campaignion_email_to_target\Message $message
   *   The message to send with all tokens resolved.
   * @param \Drupal\little_helpers\Webform\Submission $submission
   *   The webform submission that’s being processed.
   *
   * @return bool
   *   TRUE if the message was accepted by the PHP mail function.
   */
  public function send(Message $message, Submission $submission) {
    return TRUE;
  }

  /**
   * Render form elements for a single target/message pair.
   */
  public function messageForm($target, $message) {
    return [
      '#type' => 'container',
      '#attributes' => ['class' => ['email-to-target-target']],
      '#target' => $target,
    ];
  }

  /**
   * Get values that should be serialized for a form element.
   */
  public function value($edited_message, $element) {
    return [
      'target' => $element['#target'],
    ];
  }

}
