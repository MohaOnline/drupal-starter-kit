<?php

use Drupal\campaignion_opt_in\Values;
use Drupal\little_helpers\Webform\Submission;

require_once drupal_get_path('module', 'webform') . '/includes/webform.components.inc';

/**
 * Tracker tests.
 */
class TrackerTest extends \DrupalWebTestCase {

  /**
   * Create test node with a webform with 1 component.
   */
  public function setUp() : void {
    parent::setUp();
    $node['nid'] = 1;
    $node['type'] = 'petition';
    $node['title'] = 'Test';
    $node['language'] = 'und';
    $node['webform'] = webform_node_defaults();
    $node['webform']['webform_ajax'] = WEBFORM_AJAX_NO_CONFIRM;
    $component = [
      'type' => 'email',
      'form_key' => 'email',
      'cid' => 1,
      'page_num' => 1,
    ];
    webform_component_defaults($component);
    $node['webform']['components'][1] = $component;
    $this->petition = (object) $node;
  }

  /**
   * Test that adding the js file works.
   */
  public function testAddingJavascript() {
    $page = ['content' => []];
    campaignion_tracking_page_build($page);
    $this->assertNotEmpty($page['content']['#attached']['js']);
  }

  /**
   * Test adding node context when full view.
   */
  public function testNodeContextFull() {
    campaignion_tracking_node_view($this->petition, 'full', NULL);
    $this->assertArraySubset(['type' => 'setting'], $this->petition->content['#attached']['js'][0]);
    $this->assertArraySubset(
      [
        'node' => [
          'nid' => $this->petition->nid,
          'type' => $this->petition->type,
          'is_donation' => FALSE,
        ],
      ],
      $this->petition->content['#attached']['js'][0]['data']['campaignion_tracking']['context']
    );
  }

  /**
   * Test not adding node context when not full view.
   */
  public function testNodeContextTeaser() {
    campaignion_tracking_node_view($this->petition, 'teaser', NULL);
    $this->assertFalse(isset($this->petition->content['#attached']['js'][0]));
    $this->assertFalse(isset($this->petition->content['#attached']['js'][0]['data']['campaignion_tracking']['context']));
  }

  /**
   * Test adding webform context.
   */
  public function testNodeContextWebform() {
    $form = drupal_get_form('webform_client_form_' . $this->petition->nid, $this->petition);
    $found = FALSE;
    $found_key = NULL;
    foreach ($form['#attached']['js'] as $key => $attached) {
      if (isset($attached['data']['campaignion_tracking'])) {
        $found = TRUE;
        $found_key = $key;
      }
    }
    $this->assertTrue($found);
    $this->assertArraySubset(['type' => 'setting'], $form['#attached']['js'][$found_key]);
    $this->assertArraySubset([
      'webform' => [
        'total_steps' => 1,
        'current_step' => 1,
        'last_completed_step' => 0,
      ],
    ], $form['#attached']['js'][$found_key]['data']['campaignion_tracking']['context']);
  }

  /**
   * Test adding donation context.
   *
   * TODO: testing donation context.
   */
  public function testNodeContextDonation() {
  }

  /**
   * Test adding redirect codes for tracking.
   *
   * TODO: testing donation codes.
   */
  public function testNodeContextRedirectCodes() {
    $this->petition->title = "Donation test & stuff";
    $submission['sid'] = 1;
    $submission = new Submission($this->petition, (object) $submission);
    $submission->opt_in = new Values($submission);
    $nid = $submission->node->nid;

    $redirect = new stdClass();
    $redirect->fragment = '';
    campaignion_tracking_webform_redirect_alter($redirect, $submission);
    $this->assertEqual('t:t=s;w:nid=' . $nid . '&sid=1&title=Donation%20test%20%26%20stuff', $redirect->fragment);

    $redirect->fragment = 'something';
    campaignion_tracking_webform_redirect_alter($redirect, $submission);
    $this->assertEqual('something;t:t=s;w:nid=' . $nid . '&sid=1&title=Donation%20test%20%26%20stuff', $redirect->fragment);
  }

  /**
   * Test adding optins to tracking fragment.
   */
  public function testSubmissionWithOptin() {
    $component = [
      'form_key' => 'newsletter',
      'type' => 'opt_in',
      'cid' => 1,
      'page_num' => 1,
      'extra' => [
        'channel' => 'email',
        'optin_statement' => 'Opt-in statement',
      ],
    ];
    webform_component_defaults($component);
    $this->petition->webform['components'][1] = $component;
    $submission['sid'] = 1;
    $submission['data'][1] = ['radios:opt-in'];
    $submission = new Submission($this->petition, (object) $submission);
    $submission->opt_in = new Values($submission);

    $redirect = new stdClass();
    $redirect->fragment = '';
    campaignion_tracking_webform_redirect_alter($redirect, $submission);
    $this->assertEqual('t:t=s;w:nid=1&sid=1&title=Test&optin[email]=opt-in', $redirect->fragment);
  }

}
