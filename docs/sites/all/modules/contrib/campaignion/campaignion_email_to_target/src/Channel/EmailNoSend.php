<?php

namespace Drupal\campaignion_email_to_target\Channel;

use Drupal\little_helpers\Webform\Submission;

use Drupal\campaignion_email_to_target\Channel\Email;

/**
 * Channel that allows to compose an email but don’t send it.
 */
class EmailNoSend extends Email {

  /**
   * Don’t do anything when the form is submitted.
   */
  public function send($data, Submission $submission) {
    return TRUE;
  }

  /**
   * Don’t filter targets and messages.
   */
  public function filterPairs(array $pairs, Submission $submission, bool $test_mode) {
    return $pairs;
  }

  /**
   * Check whether to enable the test-mode for this channel.
   *
   * @return bool
   *   Whether to enable the test-mode for this channel. Always FALSE.
   */
  public function testModeAvailable() {
    return FALSE;
  }

}
