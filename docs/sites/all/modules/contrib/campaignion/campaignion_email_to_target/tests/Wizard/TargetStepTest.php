<?php

namespace Drupal\campaignion_email_to_target\Wizard;

use Drupal\campaignion_action\Loader;
use Drupal\campaignion_email_to_target\Api\Client;
use Drupal\little_helpers\Services\Container;
use Upal\DrupalUnitTestCase;

/**
 * Test whether the wizard target step works.
 */
class TargetStepTest extends DrupalUnitTestCase {

  /**
   * Set some e2t-api connection data.
   */
  public function setUp() : void {
    parent::setUp();
    Container::get()->inject('campaignion_email_to_target.api.Client', $this->createMock(Client::class));
  }

  /**
   * Remove test API connection.
   */
  public function tearDown() : void {
    drupal_static_reset(Container::class);
    parent::tearDown();
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
