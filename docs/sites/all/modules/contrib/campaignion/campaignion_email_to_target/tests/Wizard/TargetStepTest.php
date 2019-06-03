<?php

namespace Drupal\campaignion_email_to_target\Wizard;

use Drupal\campaignion_action\Loader;
use Drupal\campaignion_email_to_target\Api\Client;

/**
 * Test whether the wizard target step works.
 */
class TargetStepTest extends \DrupalUnitTestCase {

  /**
   * Set some e2t-api connection data.
   */
  public function setUp() {
    $GLOBALS['conf']['campaignion_email_to_target_credentials'] = [
      'url' => 'http://mocked',
      'public_key' => 'pk',
      'secret_key' => 'sk',
    ];
  }

  /**
   * Remove test API connection.
   */
  public function tearDown() {
    unset($GLOBALS['conf']['campaignion_email_to_target_credentials']);
  }

  /**
   * Test that creating a form works.
   */
  public function testStepForm() {
    $wizard = Loader::instance()->wizard('email_to_target', NULL);
    $step = new TargetStep($wizard, $this->createMock(Client::class));
    $wizard->stepHandlers['target'] = $step;
    $page = $wizard->run('target');
    $form = $page[0];
    $this->assertNotEmpty($form);
    $this->assertNotEmpty($form['#attached']['js']);
  }

}
