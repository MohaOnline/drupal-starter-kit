<?php

namespace Drupal\campaignion_email_to_target\SelectionMode;

use Drupal\campaignion_email_to_target\Message;
use Drupal\campaignion_email_to_target\Channel\Email;

/**
 * Test the “OneOrMore” selection plugin.
 */
class OneOrMoreTest extends \DrupalUnitTestcase {

  /**
   * Test run with two of targets/messages selected.
   */
  public function testTwoOfThreeTargets() {
    $mode = new OneOrMore(TRUE, new Email());
    $pairs = [
      [['id' => 'target1'], new Message([])],
      [['id' => 'target2'], new Message([])],
      [['id' => 'target3'], new Message([])],
    ];
    $element = $mode->formElement($pairs);
    $this->assertEqual('checkbox', $element['target1']['send']['#type']);

    $values = $mode->getValues($element, [
      'target1' => ['send' => TRUE],
      'target2' => ['send' => FALSE],
      'target3' => ['send' => TRUE],
    ]);
    // Two values were selected.
    $this->assertCount(2, $values);
  }

}
