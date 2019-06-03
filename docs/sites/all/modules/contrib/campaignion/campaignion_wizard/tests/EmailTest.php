<?php

namespace Drupal\campaignion_wizard;

/**
 * Test generating an email form.
 */
class EmailTest extends \DrupalUnitTestCase {

  /**
   * Test whether generating a form yields any errors.
   */
  public function testForm() {
    $node = (object) ['nid' => NULL, 'type' => 'webform', 'title' => 'Test'];
    node_object_prepare($node);
    $email = new Email($node, 'test', NULL);
    $form_state = form_state_defaults();
    $message = [
      'form_id' => 'confirmation_or_thank_you',
      'type' => 0,
      'eid' => 1,
      'toggle_title' => t('Enable a thank you email'),
      'email_title'  => t('Thank you email'),
    ];
    $form = $email->form($message, $form_state);
    $this->assertEqual(['test_toggle', 'test_email'], array_keys($form));
  }

}
