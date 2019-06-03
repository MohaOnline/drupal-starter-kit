<?php

namespace Drupal\campaignion_email_to_target;

/**
 * Common datastructure for handling protest messages.
 */
class Message extends MessageTemplateInstance {
  public $to;
  public $from;
  public $subject;
  public $header;
  public $message;
  public $footer;
  public $display;
  protected $tokenEnabledFields = [
    'to',
    'from',
    'subject',
    'header',
    'message',
    'footer',
    'display',
  ];

  /**
   * Create a message instance by passing the data as object or array.
   *
   * @param mixed $data
   *   The data to initialize the message with.
   */
  public function __construct($data) {
    $data += [
      'from' => '[submission:values:first_name] [submission:values:last_name] <[submission:values:email]>',
      'to' => '[email-to-target:contact.title] [email-to-target:contact.first_name] [email-to-target:contact.last_name] <[email-to-target:contact.email]>',
      'display' => '[email-to-target:contact.display_name]',
    ];
    parent::__construct($data);
  }

  /**
   * Return an with all public variables.
   *
   * @return array
   *   Associative array containing all public variables.
   */
  public function toArray() {
    return call_user_func('get_object_vars', $this);
  }

}
