<?php

namespace Drupal\campaignion_wizard;

class ConfirmationEmail extends Email {

  /**
   * Add the request lifetime fieldset to the form.
   */
  protected function getEmailForm(&$form_state) {
    $form = parent::getEmailForm($form_state);

    form_load_include($form_state, 'module', 'webform_confirm_email', 'admin.inc');
    $e = webform_confirm_email_settings($form, $form_state, $this->node);
    $e['request_lifetime']['#collapsed'] = TRUE;
    $e['request_lifetime']['#weight'] = 20;
    unset($e['actions']);
    $form += $e;

    return $form;
  }

  /**
   * Additionally validate the request lifetime form.
   */
  public function validate($form, &$form_state) {
    parent::validate($form, $form_state);

    $element = $form[$this->form_id . '_email'];
    webform_confirm_email_settings_validate($element, $form_state);
  }

  /**
   * Additionally submit the request lifetime form.
   */
  public function submit($form, &$form_state, $email_type) {
    parent::submit($form, $form_state, $email_type);

    // Silence the message set by webform_confirm_email_settings_submit().
    $messages = isset($_SESSION['messages']) ? $_SESSION['messages'] : NULL;
    $element = $form[$this->form_id . '_email'];
    webform_confirm_email_settings_submit($element, $form_state);
    if (is_null($messages)) {
      unset($_SESSION['messages']);
    }
    else {
      $_SESSION['messages'] = $messages;
    }
  }

}
