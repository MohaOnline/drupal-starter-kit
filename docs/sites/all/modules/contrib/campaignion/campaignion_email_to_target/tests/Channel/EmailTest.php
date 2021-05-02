<?php

namespace Drupal\campaignion_email_to_target\Channel;

use Drupal\campaignion_email_to_target\Message;

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

}
