<?php

namespace Drupal\campaignion_email_to_target;

use \Drupal\campaignion_action\TypeBase;
use \Drupal\campaignion_email_to_target\Api\Client;
use \Drupal\little_helpers\Webform\Submission;
use \Drupal\little_helpers\Webform\Webform;

class ComponentTest extends \DrupalUnitTestCase {

  /**
   * New Component with all methods mocked that would need database access.
   */
  protected function mockComponent($pairs, $options = []) {
    $action = $this->getMockBuilder(Action::class)
      ->disableOriginalConstructor()
      ->setMethods(['getOptions', 'targetMessagePairs'])
      ->getMock();
    $action->method('getOptions')->willReturn($options + [
      'dataset_name' => 'mp',
      'user_may_edit' => TRUE,
      'selection_mode' => 'one_or_more',
    ]);
    $action->method('targetMessagePairs')->willReturn($pairs);
    $node = (object) [
      'type' => 'email_to_target',
      'tnid' => NULL,
      'nid' => NULL,
      'uuid' => 'stub-uuid',
      'action' => $this->createMock(Action::class)
    ];
    node_object_prepare($node);
    $submission_o = $this->getMockBuilder(Submission::class)
      ->disableOriginalConstructor()
      ->getMock();
    $webform = $this->createMock(Webform::class);
    $webform->method('formStateToSubmission')->willReturn($submission_o);
    $submission_o->webform = $webform;
    $submission_o->node = $webform->node = $node;
    $component = $this->getMockBUilder(Component::class)
      ->setMethods(['saveSubmission', 'mail'])
      ->setConstructorArgs([
        ['name' => 'e2t', 'extra' => ['description' => 'e2t'], 'cid' => 7],
        $webform,
        $action,
      ])
      ->getMock();
    $component->method('saveSubmission')->willReturn($submission_o);
    return [$component, $submission_o];
  }

  /**
   * Test that escaping is only done for #markup attributes.
   */
  public function testRenderEscaping() {
    list($componentObj, $submission_o) = $this->mockComponent([
      [['id' => 't1', 'salutation' => 'T1', 'constituency' => ['name' => 'C1']], new Message([
        'subject' => "Subject's string",
        'header' => "Header's string",
        'message' => "Message's string",
        'footer' => "Footer's string",
      ])]
    ]);
    $component = webform_component_invoke('e2t_selector', 'defaults') + [
      'type' => 'e2t_selector',
    ];
    $element = webform_component_invoke('e2t_selector', 'render', $component);
    $form = [];
    $form_state = form_state_defaults();
    $componentObj->render($element, $form, $form_state);

    $this->assertEqual("Subject's string", $element['t1']['subject']["#default_value"]);
    $this->assertEqual("Header&#039;s string", $element['t1']['header']["#markup"]);
    $this->assertEqual("Message's string", $element['t1']['message']["#default_value"]);
    $this->assertEqual("Footer&#039;s string", $element['t1']['footer']["#markup"]);

    drupal_prepare_form('e2t_component_element', $element, $form_state);
    drupal_process_form('e2t_component_element', $element, $form_state);
    // Workaround to accommodate template_preprocess_webform_element().
    $element['#parents'] = ['workaround'];
    $rendered = drupal_render($element);
    $this->assertTrue(strpos($rendered, "'") === FALSE, 'Unescaped output strings leaked to HTML output.');
    $this->assertTrue(strpos($rendered, '&amp;') === FALSE, 'Some strings were double-escaped.');
  }

  /**
   * Test whether render with selection_mode 'all' works for single targets.
   */
  public function testRenderSelectionAllSingleTarget() {
    list($component, $submission_o) = $this->mockComponent([
      [['id' => 't1', 'salutation' => 'T1', 'constituency' => ['name' => 'C1']], new Message([
        'subject' => "Subject's string",
        'header' => "Header's string",
        'message' => "Message's string",
        'footer' => "Footer's string",
      ])]
    ], ['selection_mode' => 'all']);
    $element = [];
    $form = [];
    $form_state = form_state_defaults();
    $component->render($element, $form, $form_state);

    $this->assertEqual($element['t1']['send']['#type'], 'markup');
  }

  /**
   * Test whether render with selection_mode 'all' works for multiple targets.
   */
  public function testRenderSelectionAllMultipleTargets() {
    list($component, $submission_o) = $this->mockComponent([
      [['id' => 't1', 'salutation' => 'T1', 'constituency' => ['name' => 'C1']], new Message([
        'subject' => "Subject's string",
        'header' => "Header's string",
        'message' => "Message's string",
        'footer' => "Footer's string",
      ])],
      [['id' => 't2', 'salutation' => 'T2', 'constituency' => ['name' => 'C1']], new Message([
        'subject' => "Subject's string",
        'header' => "Header's string",
        'message' => "Message's string",
        'footer' => "Footer's string",
      ])],
    ], ['selection_mode' => 'all']);
    $element = [];
    $form = [];
    $form_state = form_state_defaults();
    $component->render($element, $form, $form_state);

    $this->assertEqual($element['t1']['send']['#type'], 'markup');

    $form_state['values'] = ['t1' => [], 't2' => []];
    $element['#parents'] = [];
    $component->validate($element, $form_state);
    $this->assertEqual(count($form_state['values']), 2);
  }

  /**
   * Test that display is used as label for the targets.
   */
  public function testRenderDisplayName() {
    list($component, $submission_o) = $this->mockComponent([
      [['id' => 't1'], new Message(['display' => 'D1'])],
    ], ['selection_mode' => 'all']);
    $element = [];
    $form = [];
    $form_state = form_state_defaults();
    $component->render($element, $form, $form_state);
    $this->assertContains('D1', $element['t1']['send']['#markup']);
  }

  /**
   * Test sending emails.
   */
  public function testSendEmail() {
    $serialized_messages = array_map(function ($m) {
      return serialize(['message' => $m->toArray(), 'target' => []]);
    }, [
      new Message(['to' => 'test1@example.com']),
    ]);
    list($component, $submission) = $this->mockComponent([]);
    $component->expects($this->once())->method('mail');
    $component->sendEmails($serialized_messages, $submission);
  }

}
