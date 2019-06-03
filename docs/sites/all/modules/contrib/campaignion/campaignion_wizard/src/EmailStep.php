<?php

namespace Drupal\campaignion_wizard;

/**
 * Standard email configuration wizard step.
 *
 * NOTE: Needs webform_confirm_email to work.
 */
class EmailStep extends WizardStep {

  const WIZARD_THANK_YOU_EID = 1;
  const WIZARD_CONFIRMATION_REQUEST_EID = 2;
  const WIZARD_NOTIFICATION_EID = 3;

  protected $step  = 'emails';
  protected $title = 'Emails';
  protected $emails = array();
  protected $emailInfo = array();

  public function __construct($wizard) {
    parent::__construct($wizard);
    $this->emailInfo = array(
      'confirmation' => array(
        'class' => ConfirmationEmail::class,
        'form_id' => 'confirmation_request',
        'type' => 1,
        'eid' => self::WIZARD_CONFIRMATION_REQUEST_EID,
        'toggle_title' => t('Enable email confirmation (double opt in)'),
        'email_title'  => t('Email confirmation'),
      ),
      'thank_you' => array(
        'form_id' => 'confirmation_or_thank_you',
        'type' => 0,
        'eid' => self::WIZARD_THANK_YOU_EID,
        'toggle_title' => t('Enable a thank you email'),
        'email_title'  => t('Thank you email'),
      ),
      'notification' => array(
        'class' => '\\Drupal\\campaignion_wizard\\NotificationEmail',
        'type' => 0,
        'form_id' => '',
        'eid' => self::WIZARD_NOTIFICATION_EID,
        'toggle_title' => t('Enable a notification email'),
        'email_title'  => t('Notification email'),
      ),
    );
  }

  protected function loadIncludes() {
    module_load_include('inc', 'webform', 'includes/webform.emails');
    module_load_include('inc', 'webform', 'includes/webform.components');
  }

  public function stepForm($form, &$form_state) {

    $form = parent::stepForm($form, $form_state);
    $node = $this->wizard->node;

    $form['#tree'] = TRUE;
    $form['wizard_head']['#tree'] = FALSE;

    $ors = 0;
    foreach ($this->emailInfo as $name => $info) {
      $info += array(
        'class' => '\\Drupal\\campaignion_wizard\\Email',
      );
      if ($ors++) {
        $form['or' . $ors] = array(
          '#type'   => 'markup',
          '#markup' => '<div class="thank-you-outer-or"><span class="thank-you-line-or">&nbsp;</span></div>',
        );
      }
      $class = $info['class'];
      $this->emails[$name] = $email = new $class($node, $info['form_id'], $info['eid']);
      $form += $email->form($this->emailInfo[$name], $form_state);
    }

    return $form;
  }

  public function checkDependencies() {
    return isset($this->wizard->node->nid);
  }

  public function validateStep($form, &$form_state) {
    foreach ($this->emails as $email) {
      $email->validate($form, $form_state);
    }
  }

  public function submitStep($form, &$form_state) {
    $node = $this->wizard->node;
    $values =& $form_state['values'];

    // If we want an confirmation the thank you has to be a conditional email.
    // Otherwise it's sent immediately.
    if (isset($this->emailInfo['confirmation'])) {
      if ($values['confirmation_request_toggle']['confirmation_request_check'] == 1) {
        $this->emailInfo['thank_you']['type'] = 2;
      }
    }

    foreach ($this->emailInfo as $name => $info) {
      $this->emails[$name]->submit($form, $form_state, $info['type']);
    }
  }

  public function status() {
    return array(
      'caption' => t('Thank you email'),
      'message' => t("You've set up a \"thank you\" email that will be sent to your supporters."),
    );
  }
}
