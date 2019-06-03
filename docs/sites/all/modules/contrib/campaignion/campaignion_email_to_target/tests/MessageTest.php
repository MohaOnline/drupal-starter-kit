<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\little_helpers\Webform\Submission;

module_load_include('inc', 'webform', 'includes/webform.components');

/**
 * Test the message objects.
 */
class MessageTest extends \DrupalUnitTestCase {

  /**
   * Test replacing a non-constituency nested value.
   */
  public function testReplaceTokensWithNestedValues() {
    $target = ['trust' => ['country' => 'Wales']];
    $message = new Message(['message' => '[email-to-target:trust.country]']);
    $message->replaceTokens($target);
    $this->assertEqual('Wales', $message->message);
  }

  /**
   * Test replacing a constituency nested value.
   */
  public function testReplaceTokensWithConstituencyValues() {
    $target = ['constituency' => ['country' => 'Wales']];
    $message = new Message(['message' => '[email-to-target:constituency.country]']);
    $message->replaceTokens($target);
    $this->assertEqual('Wales', $message->message);
  }

  /**
   * Test rendering display_name token with fallback to salutation.
   */
  public function testRenderDisplayNameToken() {
    $target = ['salutation' => 'S'];
    $message = new Message([]);
    $this->assertContains('contact.display_name', $message->display);
    $message->replaceTokens($target);
    $this->assertEqual('S', $message->display);

    $target = ['display_name' => 'D', 'salutation' => 'S'];
    $message = new Message([]);
    $message->replaceTokens($target);
    $this->assertEqual('D', $message->display);
  }

  /**
   * Test replacing tokens from a hidden component.
   */
  public function testReplaceTokensWithHiddenComponent() {
    $components[1] = [
      'cid' => 1,
      'type' => 'textfield',
      'form_key' => 'text',
      'page_num' => 1,
    ];
    $components[2] = [
      'cid' => 2,
      'type' => 'textfield',
      'form_key' => 'other',
      'page_num' => 1,
    ];
    foreach ($components as &$c) {
      webform_component_defaults($c);
    }
    $conditionals[1]['andor'] = 'and';
    $conditionals[1]['rules'][] = [
      'source_type' => 'component',
      'source' => 2,
      'operator' => 'equal',
      'value' => 'foo',
    ];
    $conditionals[1]['actions'][] = [
      'target_type' => 'component',
      'target' => 1,
      'action' => 'show',
      'invert' => FALSE,
    ];
    $data[1] = ['text'];
    $data[2] = ['not-foo'];
    $submission = (object) ['data' => $data, 'completed' => 1, 'sid' => 1];
    $node_array = ['nid' => 1, 'type' => 'webform', 'status' => 1];
    $node_array['webform'] = [
      'components' => $components,
      'conditionals' => $conditionals,
    ];
    $submission = new Submission((object) $node_array, $submission);
    $message = new Message(['message' => '[submission:values:text]']);
    $message->replaceTokens([], $submission, TRUE);
    $this->assertEqual('', $message->message);
  }

  /**
   * Test serializing and unserializing the message.
   */
  public function testToArrayFromArray() {
    foreach (array_keys(get_class_vars(Message::class)) as $key) {
      $data[$key] = 'not the default';
    }
    $m1 = new Message($data);
    $m2 = new Message($m1->toArray());
    $this->assertEqual($m1, $m2);
  }

}
