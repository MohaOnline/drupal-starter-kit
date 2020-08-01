<?php

namespace Drupal\campaignion_action;

class ActionType {

  /**
   * Content-type
   */
  protected $type;
  /**
   * Parameters
   */
  public $parameters;

  public function __construct($type, array $parameters = array()) {
    $this->type = $type;
    $this->parameters = $parameters + [
      'email_protest' => FALSE,
      'donation' => FALSE,
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function isDonation() {
    return $this->parameters['donation'];
  }

  /**
   * {@inheritdoc}
   */
  public function isEmailProtest() {
    return !empty($this->parameters['email_protest']);
  }

}
