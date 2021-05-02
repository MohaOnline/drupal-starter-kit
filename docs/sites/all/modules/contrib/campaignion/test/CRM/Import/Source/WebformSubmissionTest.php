<?php

namespace Drupal\campaignion\CRM\Import\Source;

use Upal\DrupalUnitTestCase;

/**
 * Unit test for the webform submission import source.
 */
class WebformSubmissionTest extends DrupalUnitTestCase {

  /**
   * Test hasKey() for webform tracking values.
   */
  public function testHasKeyForWebformTracking() {
    $node = (object) ['webform' => ['components' => []]];
    $submission = (object) ['tracking' => (object) ['tags' => ['foo']]];
    $submission = new WebformSubmission($node, $submission);
    $this->assertTrue($submission->hasKey('tracking.tags'));
    $this->assertFalse($submission->hasKey('tracking.unknown'));
  }

  /**
   * Test value() for webform tracking values.
   */
  public function testValueForWebformTracking() {
    $node = (object) ['webform' => ['components' => []]];
    $submission = (object) ['tracking' => (object) ['tags' => ['foo']]];
    $submission = new WebformSubmission($node, $submission);
    $this->assertEquals(['foo'], $submission->value('tracking.tags'));
    $this->assertNull($submission->value('tracking.unknown'));
  }

}
