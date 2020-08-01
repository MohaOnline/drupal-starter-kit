<?php

namespace Drupal\campaignion_wizard;

use Drupal\little_helpers\Webform\Webform;

/**
 * Extend the webform step to include validations for the email field.
 */
class WebformStepUnique extends WebformStep {

  /**
   * Check for a unique email field.
   */
  public function validateStep($form, &$form_state) {
    parent::validateStep($form, $form_state);

    $webform = new Webform($form['#node']);
    if (!($c = $webform->componentByKey('email'))) {
      if ($cs = $webform->componentsByType('email')) {
        $c = reset($cs);
      }
    }
    if ($c && !$this->hasCaptcha($form['#node'])) {
      if (empty($c['extra']['unique'])) {
        form_error($form, t('The email field must be unique. Change it by clicking on the field and then on "Validation".'));
      }
      if (empty($c['mandatory']) && empty($c['required'])) {
        form_error($form, t('The email field must be required. Change it by clicking on the field and then on "Validation".'));
      }
    }
  }

  /**
   * Check whether webforms for a node are captcha protected.
   *
   * @param object $node
   *   The node which’s captcha settings should be checked.
   *
   * @return bool
   *   TRUE if a captcha is set up for this node’s webform otherwise FALSE.
   */
  protected function hasCaptcha($node) {
    if (!module_exists('captcha')) {
      return FALSE;
    }
    $form_id = "webform_client_form_{$node->nid}";
    $base_form_id = 'webform_client_form';

    // Get CAPTCHA type and module for given form_id or base_form_id.
    module_load_include('inc', 'captcha');
    $captcha_point = captcha_get_form_id_setting($form_id) ?: captcha_get_form_id_setting($base_form_id);
    return $captcha_point && !empty($captcha_point->captcha_type);
  }

}
