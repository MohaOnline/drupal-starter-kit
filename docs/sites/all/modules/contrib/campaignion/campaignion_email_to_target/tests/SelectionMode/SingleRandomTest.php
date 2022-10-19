<?php

namespace Drupal\campaignion_email_to_target\SelectionMode;

use Drupal\campaignion_email_to_target\Message;
use Drupal\campaignion_email_to_target\Channel\Email;
use Upal\DrupalUnitTestCase;

/**
 * Test the “SingleRandom” selection plugin.
 */
class SingleRandomTest extends DrupalUnitTestCase {

  /**
   * Test choosing randomly between two targets/messages.
   */
  public function testTwoTargets() {
    $mode = new SingleRandom(TRUE, new Email());
    $pairs = [
      [['id' => 'target1'], new Message([])],
      [['id' => 'target2'], new Message([])],
    ];
    $element = $mode->formElement($pairs);
    $target_element_keys = element_children($element);
    $this->assertCount(1, $target_element_keys);
    $this->assertContains($target_element_keys[0], ['target1', 'target2']);
  }

}
