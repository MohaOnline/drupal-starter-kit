<?php

namespace Drupal\campaignion_email_to_target\Channel;

use Drupal\little_helpers\Webform\Submission;

use Drupal\campaignion_email_to_target\Message;

/**
 * Channel plugin for sending emails to targets.
 */
class Email {

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
  public function send($data, Submission $submission) {
    $message = new Message($data['message']);
    $message->replaceTokens(NULL, $submission);

    $node = $submission->webform->node;
    $root_node = $node->tnid ? node_load($node->tnid) : $node;

    // Set the HTML property based on availablity of MIME Mail.
    $email['html'] = FALSE;
    // Pass through the theme layer.
    $t = 'campaignion_email_to_target_mail';
    $theme_d = ['message' => $message, 'submission' => $submission];
    $email['message'] = theme([$t, $t . '_' . $node->nid], $theme_d);

    $email['from'] = $message->from();
    $email['subject'] = $message->subject;

    $email['headers'] = [
      'X-Mail-Domain' => variable_get('site_mail_domain', 'supporter.campaignion.org'),
      'X-Action-UUID' => $root_node->uuid,
    ];

    // Verify that this submission is not attempting to send any spam hacks.
    if (_webform_submission_spam_check($message->to(), $email['subject'], $email['from'], $email['headers'])) {
      watchdog('campaignion_email_to_target', 'Possible spam attempt from @remote !message',
              ['@remote' => ip_address(), '!message' => "<br />\n" . nl2br(htmlentities($email['message']))]);
      drupal_set_message(t('Illegal information. Data not submitted.'), 'error');
      return FALSE;
    }

    $language = $GLOBALS['language'];
    $mail_params = [
      'message' => $email['message'],
      'subject' => $email['subject'],
      'headers' => $email['headers'],
      'submission' => $submission,
      'email' => $email,
    ];

    // Mail the submission.
    $m = $this->mail($message->to(), $language, $mail_params, $email['from']);
    return $m['result'];
  }

  /**
   * Wrapper for drupal_mail().
   */
  protected function mail($to, $language, $mail_params, $from) {
    return drupal_mail('campaignion_email_to_target', 'email_to_target', $to, $language, $mail_params, $from);
  }

  /**
   * Render form elements for a single target/message pair.
   */
  public function messageForm($target, $message, $editable) {
    $t = [
      '#type' => 'container',
      '#attributes' => ['class' => ['email-to-target-target']],
      '#message' => $message->toArray(),
      '#target' => $target,
      '#editable' => $editable,
      '#theme' => 'campaignion_email_to_target_email_message_form',
    ];
    $t['subject'] = [
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#default_value' => $message->subject,
      '#disabled' => !$editable,
    ];
    $t['header']['#markup'] = check_plain($message->header);
    $t['message'] = [
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#default_value' => $message->message,
      '#disabled' => !$editable,
    ];
    $t['footer']['#markup'] = check_plain($message->footer);
    return $t;
  }

  /**
   * Get values that should be serialized for a form element.
   */
  public function value($edited_message, $element) {
    return [
      'message' => $edited_message + $element['#message'],
      'target' => $element['#target'],
    ];
  }

}
