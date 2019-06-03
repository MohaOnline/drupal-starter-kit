<?php

namespace Drupal\campaignion_wizard;

use \Drupal\little_helpers\Webform\Webform;

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
    if ($c) {
      if (empty($c['extra']['unique'])) {
        form_error($form, t('The email field must be unique. Change it by clicking on the field and then on "Validation".'));
      }
      if (empty($c['mandatory']) && empty($c['required'])) {
        form_error($form, t('The email field must be required. Change it by clicking on the field and then on "Validation".'));
      }
    }
  }

}
