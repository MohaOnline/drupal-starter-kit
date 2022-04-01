<?php

namespace Drupal\campaignion_email_to_target\Channel;

use Drupal\campaignion_email_to_target\Message;
use Drupal\little_helpers\Webform\Submission;

/**
 * Test the “Email” channel.
 */
class EmailTest extends \DrupalUnitTestcase {

  /**
   * Test that escaping is only done for #markup attributes.
   */
  public function testRenderEscaping() {
    list($target, $message) = [
      ['id' => 't1', 'salutation' => 'T1', 'constituency' => ['name' => 'C1']],
      new Message([
        'subject' => "Subject's string",
        'header' => "Header's string",
        'message' => "Message's string",
        'footer' => "Footer's string",
      ]),
    ];
    $channel = new Email();
    $element = $channel->messageForm($target, $message, TRUE);

    $this->assertEqual("Subject's string", $element['subject']["#default_value"]);
    $this->assertEqual("Header&#039;s string", $element['header']["#markup"]);
    $this->assertEqual("Message's string", $element['message']["#default_value"]);
    $this->assertEqual("Footer&#039;s string", $element['footer']["#markup"]);

    $form_state = form_state_defaults();
    drupal_prepare_form('e2t_component_element', $element, $form_state);
    drupal_process_form('e2t_component_element', $element, $form_state);
    $rendered = drupal_render($element);
    $this->assertTrue(strpos($rendered, "'") === FALSE, 'Unescaped output strings leaked to HTML output.');
    $this->assertTrue(strpos($rendered, '&amp;') === FALSE, 'Some strings were double-escaped.');
  }

  /**
   * Test rendering the form when editing is disabled.
   */
  public function testRenderNonEditable() {
    list($target, $message) = [
      ['id' => 't1', 'salutation' => 'T1', 'constituency' => ['name' => 'C1']],
      new Message([
        'subject' => "Subject's string",
        'header' => "Header's string",
        'message' => "Message's string",
        'footer' => "Footer's string",
      ]),
    ];
    $channel = new Email();
    $element = $channel->messageForm($target, $message, FALSE);

    $form_state = form_state_defaults();
    drupal_prepare_form('e2t_component_element', $element, $form_state);
    drupal_process_form('e2t_component_element', $element, $form_state);
    $rendered = drupal_render($element);
    $this->assertStringContainsString('<p class="email-to-target-subject"><strong>Subject&#039;s string</strong></p>', $rendered);
  }

  /**
   * Test filterPairs() removes targets without email address.
   */
  public function testFilterPairsWithoutEmail() {
    $pairs = [
      [['email' => 'test1@example.com', 'name' => 'One'], []],
      [['Name' => 'No email'], []],
      [['email' => 'test2@example.com', 'name' => 'Two'], []],
    ];
    $channel = new Email();
    $submission = $this->createMock(Submission::class);
    $submission->method('valueByKey')->willReturn('test-mode@example.com');
    $new_pairs = $channel->filterPairs($pairs, $submission, FALSE);
    $this->assertEqual([$pairs[0], $pairs[2]], $new_pairs);
  }

  /**
   * Test filterPairs() replaces email addresses of targets in test-mode.
   *
   * Targets without email are still excluded.
   */
  public function testFilterPairsTestMode() {
    $pairs = [
      [
        ['email' => 'test1@example.com', 'name' => 'One'],
        new Message(['toAddress' => 'test1@example.com']),
      ],
      [['Name' => 'No email'], []],
      [
        ['email' => 'test2@example.com', 'name' => 'Two'],
        new Message(['toAddress' => 'test2@example.com']),
      ],
    ];
    $channel = new Email();
    $submission = $this->createMock(Submission::class);
    $test_email = 'test-mode@example.com';
    $submission->method('valueByKey')->willReturn($test_email);
    $new_pairs = $channel->filterPairs($pairs, $submission, TRUE);
    $this->assertEqual([
      ['email' => $test_email, 'name' => 'One'],
      ['email' => $test_email, 'name' => 'Two'],
    ], array_map(function ($p) { return $p[0]; }, $new_pairs));
    foreach ($new_pairs as $pair) {
      $message = $pair[1];
      $to = $message->to();
      $this->assertEqual("<$test_email>", substr($to, -(strlen($test_email)+2)));
    }
  }

}
