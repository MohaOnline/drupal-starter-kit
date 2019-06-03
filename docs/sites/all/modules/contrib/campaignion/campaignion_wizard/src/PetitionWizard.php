<?php

namespace Drupal\campaignion_wizard;

/**
 * A wizard for petition forms.
 *
 * NOTE: Needs form_builder_webform and webform_confirm_email to work.
 */
class PetitionWizard extends NodeWizard {
  public $steps = array(
    'content' => 'ContentStep',
    'form'    => 'PetitionWebformStep',
    'emails'  => 'EmailStep',
    'thank'   => 'ThankyouStep',
    'confirm' => 'ConfirmStep',
  );
}
