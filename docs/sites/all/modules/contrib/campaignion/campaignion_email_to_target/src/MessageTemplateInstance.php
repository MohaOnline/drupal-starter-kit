<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\little_helpers\Webform\Submission;

/**
 * Common code for messages and exclusions.
 */
abstract class MessageTemplateInstance {

  /**
   * Create a new instance.
   *
   * @param mixed $data
   *   Object or array to read data from.
   */
  public function __construct($data) {
    foreach ($data as $k => $v) {
      $this->$k = $v;
    }
  }

  /**
   * Replace tokens in all token enabled properties.
   *
   * @param array $target
   *   Target as received from the API.
   * @param \Drupal\little_helpers\Webform\Submission $submission
   *   A webform submission object.
   * @param bool $clear
   *   Whether to remove tokens that couldn’t be replaced.
   */
  public function replaceTokens(array $target = NULL, Submission $submission = NULL, $clear = FALSE) {
    if (empty($target['display_name']) && !empty($target['salutation'])) {
      $target['display_name'] = $target['salutation'];
    }
    $data['target'] = $target;
    $data['webform-submission'] = $submission;
    if ($submission) {
      $data['node'] = $submission->node;
    }
    // It's ok to not sanitize values here. We will sanitize them later
    // when it's clear whether we use it in a plain text email (no escaping)
    // or in HTML output (check_plain).
    $options['sanitize'] = FALSE;
    $options['clear'] = $clear;
    foreach ($this->tokenEnabledFields as $f) {
      $this->{$f} = token_replace($this->{$f}, $data, $options);
    }
  }

}
