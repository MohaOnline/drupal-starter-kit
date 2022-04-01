<?php

namespace Drupal\campaignion_action;

use Upal\DrupalUnitTestCase;

/**
 * Test for the node hook implementations.
 */
class NodeFormTest extends DrupalUnitTestCase {

  /**
   * Test form alter with an empty form and node.
   */
  public function testFormAlterEmptyForm() {
    $form['#node'] = (object) [];
    $form_state = [];
    $original_form = $form;
    campaignion_action_form_node_form_alter($form, $form_state);
    $this->assertEquals($original_form, $form);
  }

  /**
   * Test form alter on non-webform node.
   */
  public function testFormAlterNoWebform() {
    $form['#node'] = (object) [];
    $form['action_closed_text'] = [];
    $form_state = [];
    $original_form = $form;
    campaignion_action_form_node_form_alter($form, $form_state);
    $this->assertEquals($original_form, $form);
  }

  /**
   * Test form alter with field and webform.
   */
  public function testFormAlter() {
    $form['#node'] = (object) ['webform' => ['status' => 1]];
    $form['action_closed_text']['#language'] = LANGUAGE_NONE;
    $form['action_closed_text'][LANGUAGE_NONE][] = [];
    $form_state = [];
    $expected_wrapper = $form['action_closed_text'];
    campaignion_action_form_node_form_alter($form, $form_state);
    $this->assertArrayHasKey('toggle', $form['action_closed_text']);
    $this->assertEquals('checkbox', $form['action_closed_text']['toggle']['#type']);

    $this->assertEquals('container', $form['action_closed_text'][LANGUAGE_NONE]['#type']);
    $states['visible']["#{$form['action_closed_text']['toggle']['#id']}"]['checked'] = TRUE;
    $this->assertEquals($states, $form['action_closed_text'][LANGUAGE_NONE]['#states']);
  }

  /**
   * Test the node submit handler without value.
   */
  public function testNodeSubmitWithoutValue() {
    $node = (object) ['webform' => ['status' => 2]];
    $form_state = [];
    campaignion_action_node_submit($node, [], $form_state);
    $this->assertEquals(2, $node->webform['status']);
  }

  /**
   * Test the node submit handler with a value.
   */
  public function testNodeSubmitWithValue() {
    $node = (object) ['webform' => ['status' => 2]];
    $form_state['values']['action_closed_toggle'] = 'trueish';
    campaignion_action_node_submit($node, [], $form_state);
    $this->assertEquals(0, $node->webform['status']);
  }

  /**
   * Test hook_node_view() on open form.
   */
  public function testNodeView() {
    $node = (object) [
      'status' => NODE_PUBLISHED,
      'webform' => ['status' => 1],
      'content' => ['action_closed_text' => ['#markup' => 'content']],
    ];
    campaignion_action_node_view($node);
    $this->assertArrayNotHasKey('action_closed_text', $node->content);
  }

}
