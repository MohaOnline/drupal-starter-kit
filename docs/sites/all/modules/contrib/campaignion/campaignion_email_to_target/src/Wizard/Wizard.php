<?php

namespace Drupal\campaignion_email_to_target\Wizard;

use \Drupal\campaignion_wizard\NodeWizard;

/**
 * A wizard for email_to_target forms.
 *
 * NOTE: Needs form_builder_webform and webform_confirm_email to work.
 */
class Wizard extends NodeWizard {
  public $steps = array(
    'content' => '\\Drupal\\campaignion_email_to_target\\Wizard\\ContentStep',
    'target'  => '\\Drupal\\campaignion_email_to_target\\Wizard\\TargetStep',
    'message' => '\\Drupal\\campaignion_email_to_target\\Wizard\\MessageStep',
    'form'    => '\\Drupal\\campaignion_email_to_target\\Wizard\\FormStep',
    'emails'  => 'EmailStep',
    'thank'   => 'ThankyouStep',
    'confirm' => '\\Drupal\\campaignion_email_to_target\\Wizard\\ConfirmationStep',
  );
}
