<?php

namespace Drupal\campaignion_wizard;

/**
 * A wizard for generic forms.
 *
 * NOTE: Needs form_builder_webform and webform_confirm_email to work.
 */
class WebformWizard extends NodeWizard {
  public $steps = array(
    'content' => 'ContentStep',
    'form'    => 'WebformStep',
    'emails'  => 'EmailStep',
    'thank'   => 'ThankyouStep',
    'confirm' => 'ConfirmStep',
  );
}
