<?php

namespace Drupal\campaignion_opt_in;

use Drupal\campaignion_activity\WebformSubmission as SubmissionActivity;
use Drupal\little_helpers\Webform\Submission;

/**
 * Test whether recording opt-ins based on webform submissions works.
 */
class OptInRecrodTest extends \DrupalUnitTestCase {

  /**
   * Create a stub submission and activity.
   */
  public function setUp() {
    parent::setUp();
    $node_stub = (object) [
      'nid' => 1001,
      'webform' => [
        'components' => [
          1 => [
            'cid' => 1,
            'pid' => 0,
            'channel' => 'phone',
            'form_key' => 'phone_opt_in',
            'type' => 'opt_in',
            'extra' => ['channel' => 'phone'],
          ],
          2 => [
            'cid' => 2,
            'pid' => 0,
            'form_key' => 'newsletter',
            'type' => 'opt_in',
            'extra' => ['channel' => 'email'],
          ],
        ]
      ],
    ];
    foreach ($node_stub->webform['components'] as &$component) {
      webform_component_defaults($component);
    }

    $submission_stub = (object) [
      'nid' => 1001,
      'sid' => 1001,
      'data' => [
        1 => ['checkbox:opt-in'],
        2 => ['radios:opt-in'],
      ],
    ];
    $this->submission = new Submission($node_stub, $submission_stub);

    $this->activity = new SubmissionActivity([
      'contact_id' => 1001,
      'nid' => 1001,
      'sid' => 1001,
      'confirmed' => REQUEST_TIME,
    ]);
    $this->activity->save();
  }

  /**
   * Remove the activity and all opt-in records.
   */
  public function tearDown() {
    $this->activity->delete();
    db_delete('campaignion_opt_in')->execute();
    parent::tearDown();
  }

  /**
   * Test a webform submission with an opt_in and a newsletter component.
   */
  public function testTwoOptIns() {
    $s = $this->submission;
    campaignion_opt_in_campaignion_action_taken($s->node, $s);

    $rows = db_select('campaignion_opt_in', 'o')->fields('o')->execute()->fetchAll();
    $this->assertCount(2, $rows);
    $this->assertEqual('phone', $rows[0]->channel);
    $this->assertEqual('email', $rows[1]->channel);
  }

}
