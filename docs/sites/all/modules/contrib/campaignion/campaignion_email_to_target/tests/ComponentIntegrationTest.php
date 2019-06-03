<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\campaignion_action\Loader;
use Drupal\little_helpers\Webform\Submission;

use Drupal\campaignion_email_to_target\Api\Client;

/**
 * Tests for the component that require a fully configured and saved node.
 */
class ComponentIntegrationTest extends \DrupalUnitTestCase {

  /**
   * Create test node.
   */
  public function setUp() {
    parent::setUp();
    $components[1] = ['type' => 'textfield', 'form_key' => 'text'];
    $components[2]['type'] = 'pagebreak';
    $components[3]['type'] = 'e2t_selector';
    $this->node = $this->createNode($components, ['webform_ajax' => WEBFORM_AJAX_NO_CONFIRM]);
  }

  /**
   * Remove test node.
   */
  public function tearDown() {
    if (!empty($this->node->nid)) {
      node_delete($this->node->nid);
    }
    parent::tearDown();
  }

  /**
   * Create a new email_to_target node.
   */
  protected function createNode($components, $settings = []) {
    module_load_include('components.inc', 'webform', 'includes/webform');
    $node = (object) ['type' => 'email_to_target'];
    $client = $this->createMock(Client::class);
    $type = Loader::instance()->type($node->type);
    $node->action = $this->getMockBuilder(Action::class)
      ->setConstructorArgs([$type, $node, $client])
      ->setMethods(['getOptions', 'targetMessagePairs'])
      ->getMock();
    node_object_prepare($node);
    $node->webform = $settings + $node->webform;
    $node->webform['components'] = $components;
    foreach ($node->webform['components'] as $cid => &$component) {
      webform_component_defaults($component);
      $component['cid'] = $cid;
    }
    node_save($node);
    $action = $node->action;
    $node = node_load($node->nid);
    $node->action = $action;
    return $node;
  }

  /**
   * Test rendering the component with a redirect.
   */
  public function testRenderExclusionAjaxRedirect() {
    $this->node->action->method('getOptions')->willReturn([
      'dataset_name' => 'mp',
      'user_may_edit' => TRUE,
      'selection_mode' => 'one_or_more',
    ]);
    $this->node->action->method('targetMessagePairs')->willReturn(
      new Exclusion(['url' => 'http://example.com'])
    );
    $GLOBALS['conf']['webform_tracking_mode'] = 'none';

    $form_state['build_info']['args'] = [$this->node];
    $form_state['webform']['page_num'] = 2;
    $first_step = drupal_build_form('webform_client_form', $form_state);

    $form_state['values']['submitted']['text'] = 'some input';
    $form_state['values']['op'] = 'next';
    $form_state['clicked_button']['#parents'] = ['next'];
    webform_client_form_pages($first_step, $form_state);

    // $form_state['values'] is reset by drupal_process_form() so we use
    // references to get the new sid out.
    $form_state['sid'] = &$form_state['values']['details']['sid'];
    $second_step = drupal_build_form('webform_client_form', $form_state);
    $submission = Submission::load($this->node->nid, $form_state['sid']);
    $this->assertNotEmpty($submission);
    $this->assertEquals('some input', $submission->valueByKey('text'));
    $this->assertEquals('http://example.com', $form_state['redirect'][0]);
    $this->assertTrue($form_state['webform_completed']);
  }


}
