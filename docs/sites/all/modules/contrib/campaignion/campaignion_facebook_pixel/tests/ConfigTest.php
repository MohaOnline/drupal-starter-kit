<?php

namespace Drupal\campaignion_facebook_pixel;

use Drupal\campaignion_opt_in\Values;
use Drupal\little_helpers\Webform\Submission;

/**
 * Test the config class.
 */
class ConfigTest extends \DrupalUnitTestCase {

  /**
   * Test attaching PageView event.
   */
  public function testNodeAttach() {
    $config = new Config([1 => 'testcode']);
    $node = (object) ['nid' => 1, 'type' => 'thank_you_page', 'content' => []];
    $config->attach($node);

    $this->assertNotEmpty($node->content['#attached']['js']);
    $this->assertCount(1, $node->content['#attached']['js']);
    $s = $node->content['#attached']['js'][0];
    $this->assertEqual('setting', $s['type']);
    $this->assertArrayHasKey('testcode', $s['data']['campaignion_facebook_pixel']['pixels']);
    $events = ['PageView'];
    $this->assertEqual($events, $s['data']['campaignion_facebook_pixel']['pixels']['testcode']);
  }

  /**
   * Test sending 'Lead' and 'CompleteRegistration' for opt-in submissions.
   */
  public function testSendLeadWithOptin() {
    $s_node['nid'] = 1;
    $s_node['webform']['components'][1] = [
      'form_key' => 'newsletter',
      'type' => 'opt_in',
      'cid' => 1,
      'extra' => [
        'channel' => 'email',
        'optin_statement' => 'Opt-in statement',
      ],
    ];
    $s_submission['data'][1] = ['radios:opt-in'];
    $submission = new Submission((object) $s_node, (object) $s_submission);
    $submission->opt_in = new Values($submission);

    $config = new Config([1 => 'testcode']);
    $this->assertEqual('fbq:testcode=r,l', $config->submissionFragment($submission));
  }

}
