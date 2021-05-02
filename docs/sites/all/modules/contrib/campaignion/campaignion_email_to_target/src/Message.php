<?php

namespace Drupal\campaignion_email_to_target;

/**
 * Common datastructure for handling protest messages.
 */
class Message extends MessageTemplateInstance {
  public $toName;
  public $toAddress;
  public $fromName;
  public $fromAddress;
  public $subject;
  public $header;
  public $message;
  public $footer;
  public $display;
  protected $tokenEnabledFields = [
    'toName',
    'toAddress',
    'fromName',
    'fromAddress',
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
      'fromName' => '[submission:values:first_name] [submission:values:last_name]',
      'fromAddress' => '[submission:values:email]',
      'toName' => '[email-to-target:contact.title] [email-to-target:contact.first_name] [email-to-target:contact.last_name]',
      'toAddress' => '[email-to-target:contact.email]',
      'display' => '[email-to-target:contact.display_name]',
    ];
    parent::__construct($data);
  }

  /**
   * Quote strings for use in address headers.
   *
   * Quote backslashes and any double quotation marks by prepending a
   * backslash.
   * See https://tools.ietf.org/html/rfc5322#section-3.2.4
   */
  protected function quoteMail($string) {
    $quoted1 = preg_replace("/\\\\/", "\\\\\\\\", $string);
    $quoted2 = preg_replace('/"/', '\\\\"', $quoted1);
    return $quoted2;
  }

  /**
   * Get full To: address.
   */
  public function to() {
    return '"' . trim($this->quoteMail($this->toName)) . '" <' . $this->toAddress . '>';
  }

  /**
   * Get full From: address.
   */
  public function from() {
    return '"' . trim($this->quoteMail($this->fromName)) . '" <' . $this->fromAddress . '>';
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
