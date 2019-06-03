<?php

namespace Drupal\campaignion_wizard;

/**
 * Modify the donation email-step to don't allow confirmation emails.
 *
 * NOTE: Needs webform_confirm_email to work.
 */
class DonationEmailStep extends EmailStep {

  public function __construct($wizard) {
    parent::__construct($wizard);
    unset($this->emailInfo['confirmation']);
  }

}
