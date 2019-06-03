<?php

namespace Drupal\campaignion_wizard;

use Drupal\oowizard\WizardStep as _WizardStep;

/**
 * Generic step for campaignion action wizards.
 */
abstract class WizardStep extends _WizardStep {

  /**
   * Form constructor for the current form step.
   *
   * @ingroup forms
   */
  public function stepForm($form, &$form_state) {
    $form['#theme'] = 'campaignion_wizard_form';
    $form['trail'] = $this->wizard->trail();
    $form['wizard_advanced'] = array(
      '#type' => 'container',
      '#weight' => 2000,
    );

    $form['#attributes']['class'][] = 'wizard-form';
    $form['#attributes']['class'][] = 'wizard-main-container';

    $form['buttons']['#tree'] = FALSE;
    $form['buttons']['#weight'] = -20;
    $form['buttons']['next']['#value'] = t('Next');

    if (isset($form['buttons']['return'])) {
      $label = (isset($this->wizard->node->status) && $this->wizard->node->status) ? t('Save & return') : t('Save as draft');
      $form['buttons']['return']['#value'] = $label;
    }
    return $form;
  }

  /**
   * Render the status message for this step.
   *
   * @return null|array
   *   If a status message is provided it must be an array with the keys:
   *   - caption: A title for the status message.
   *   - message: Some description of the status.
   */
  public function status() {
    return NULL;
  }

}
