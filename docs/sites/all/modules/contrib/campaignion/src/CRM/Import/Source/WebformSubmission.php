<?php

namespace Drupal\campaignion\CRM\Import\Source;

use Drupal\little_helpers\Webform\Submission;

/**
 * A SourceInterface compatible submission class.
 */
class WebformSubmission extends Submission implements SourceInterface {

  public function hasKey($key) {
    // Check whether a webform component with this key exists.
    return (bool) $this->webform->componentByKey($key);
  }

  public function value($key) {
    return $this->valueByKey($key);
  }

}
