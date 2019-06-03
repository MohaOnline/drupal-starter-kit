<?php

namespace Drupal\campaignion_newsletters;

class ApiError extends \Exception {

  public $backend = '';
  public $link;

  public function __construct($backend, $message = '', $variables = array(), $code = 0, $link = NULL, \Exception $previous = NULL) {
    $this->backend = $backend;
    $message = format_string($message, $variables + ['@code' => $code]);
    $this->link = $link;
    parent::__construct($message, $code, $previous);
  }

  public function log() {
    \watchdog_exception($this->backend, $this, NULL, [], WATCHDOG_ERROR, $this->link);
  }

  /**
   * Check whether this error is persistent.
   *
   * Persistent errors won't go away by just retrying to send the same data.
   *
   * @return bool
   */
  public function isPersistent() {
    return FALSE;
  }

}
