<?php

namespace Drupal\campaignion_wizard;

/**
 * Wizard step for configuring email protest emails.
 *
 * NOTE: Needs webform_confirm_email to work.
 */
class EmailProtestEmailStep extends EmailStep {

  const WIZARD_PROTEST_EID = 4;

  public function submitStep($form, &$form_state) {
    parent::submitStep($form, $form_state);

    $email_data = array();

    foreach ($this->wizard->node->webform['components'] as $cid => $component) {
      switch ($component['form_key']) {
        case 'email_protest_target':
          $email_data['email'] = $cid;
          break;

        case 'email_subject':
          $email_data['subject'] = $cid;
          break;

        case 'email_body':
          $email_data['template'] = $cid;
          break;

        case 'email':
          $email_data['from_address'] = $cid;
          break;
      }
    }

    $email_data['eid']  = self::WIZARD_PROTEST_EID;
    $email_data['nid']  = $this->wizard->node->nid;
    $email_data['html'] = FALSE;

    $email = new Email($this->wizard->node, 'protest_email', self::WIZARD_PROTEST_EID);
    $type = ($form_state['values']['confirmation_request_toggle']['confirmation_request_check'] == 1) ? 2 : 0;

    $email->submitData($email_data, $type);
  }
}
