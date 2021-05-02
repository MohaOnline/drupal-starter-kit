<?php

namespace Drupal\campaignion_wizard;

/**
 * Integration test for the ConfirmStep.
 */
class ConfirmStepTest extends \DrupalUnitTestCase {

  /**
   * Test getting the stepForm using the wizard.
   */
  public function testShowStepUsingWizard() {
    $wizard = new PetitionWizard([], NULL, 'petition');
    $wizard->setStep('confirm');
    // Needed in order for ThankYouStep::status() to don’t throw warnings.
    $wizard->node->nid = 0;
    $wizard->node->field_thank_you_pages[LANGUAGE_NONE] = [
      0 => ['type' => 'redirect', 'node_reference_nid' => NULL],
      1 => ['type' => 'redirect', 'node_reference_nid' => NULL],
    ];
    $form = $wizard->wizardForm();
    $this->assertArrayHasKey('return', $form['confirm_container']['buttons']);
    $this->assertArrayHasKey('draft', $form['confirm_container']['buttons']);
  }

}
