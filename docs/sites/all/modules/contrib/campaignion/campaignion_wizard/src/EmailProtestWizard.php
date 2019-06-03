<?php

namespace Drupal\campaignion_wizard;

/**
 * A wizard for email protest actions.
 *
 * NOTE: Needs form_builder_webform and webform_confirm_email to work.
 */
class EmailProtestWizard extends NodeWizard {
  public $steps = array(
    'content' => 'ContentStep',
    'target'  => 'EmailProtestTargetStep',
    'form'    => 'WebformStepUnique',
    'emails'  => 'EmailProtestEmailStep',
    'thank'   => 'ThankyouStep',
    'confirm' => 'ConfirmStep',
  );
}
