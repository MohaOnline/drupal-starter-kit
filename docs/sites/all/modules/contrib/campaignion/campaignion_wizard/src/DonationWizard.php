<?php

namespace Drupal\campaignion_wizard;

/**
 * A wizard for donation forms.
 *
 * NOTE: Needs form_builder_webform and webform_confirm_email to work.
 */
class DonationWizard extends NodeWizard {
  public $steps = array(
    'content' => 'ContentStep',
    'form'    => 'WebformStep',
    'emails'  => 'DonationEmailStep',
    'thank'   => 'ThankyouStep',
    'confirm' => 'ConfirmStep',
  );
}
