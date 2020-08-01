<?php

namespace Drupal\campaignion_email_to_target\Wizard;

use Drupal\campaignion_action\Loader;
use Drupal\campaignion_email_to_target\Action;
use Drupal\form_builder\Loader as FormBuilderLoader;
use Drupal\little_helpers\Rest\HttpError;
use Drupal\little_helpers\Services\Container;
use Upal\DrupalUnitTestCase;

/**
 * Test validation on the wizard form step.
 */
class FormSteptest extends DrupalUnitTestCase {

  /**
   * Create a test node.
   */
  public function setUp() {
    parent::setUp();
    $mock_loader = $this->getMockBuilder(Loader::class)
      ->disableOriginalConstructor()
      ->getMock();
    Container::get()->inject('campaignion_action.loader', $mock_loader);
    $mock_action = $this->getMockBuilder(Action::class)
      ->disableOriginalConstructor()
      ->getMock();
    $mock_loader->method('actionFromNode')->willReturn($mock_action);
    $this->mock_action = $mock_action;

    $this->node = (object) [
      'type' => 'email_to_target',
      'title' => __CLASS__,
    ];
    node_object_prepare($this->node);
    $this->node->webform['components'] = [
      1 => [
        'type' => 'email',
        'extra' => ['unique' => TRUE],
        'required' => TRUE,
      ],
      2 => ['type' => 'e2t_selector'],
    ];
    module_load_include('inc', 'webform', 'includes/webform.components');
    foreach ($this->node->webform['components'] as &$c) {
      webform_component_defaults($c);
    }
    node_save($this->node);

    // Create an internal state from the permanently stored form.
    $loader = FormBuilderLoader::instance();
    $this->form_obj = $loader->fromStorage('webform', $this->node->nid);
    $this->form_obj->save();
  }

  /**
   * Delete the test node.
   */
  public function tearDown() {
    $this->form_obj->delete();
    node_delete($this->node->nid);
    drupal_static_reset('form_set_error');
    $_SESSION['messages'] = [];
    Container::get()->inject('campaignion_action.loader', NULL);
    parent::tearDown();
  }

  /**
   * Test form validation for unavailable datasets.
   */
  public function testValidateUnavailableDataset() {
    $error = new HttpError((object) [
      'code' => 401,
      'error' => 'Access denied',
    ]);
    $this->mock_action->method('dataset')->will($this->throwException($error));
    $form['#node'] = $this->node;
    $form['#parents'] = [];
    $form_state = form_state_defaults();
    $step = new FormStep(NULL);
    $step->validateStep($form, $form_state);
    $this->assertTrue(isset($_SESSION['messages']['error'][0]));
    $this->assertCount(1, $_SESSION['messages']['error']);
    $this->assertContains('inaccessible', $_SESSION['messages']['error'][0]);
  }

}
