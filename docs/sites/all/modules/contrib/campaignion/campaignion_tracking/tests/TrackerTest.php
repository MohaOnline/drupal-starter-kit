<?php

require_once drupal_get_path('module', 'webform') . '/includes/webform.components.inc';

/**
 * Tracker tests.
 */
class TrackerTest extends \DrupalWebTestCase {

  /**
   * Create test nodes.
   */
  public function setUp() {
    parent::setUp();
    $this->petition = entity_create('node', ['type' => 'petition']);
    $this->petition->webform = webform_node_defaults();
    $component = [
      'type' => 'email',
      'form_key' => 'email',
      'cid' => 1,
      'page_num' => 1,
    ];
    webform_component_defaults($component);
    $this->petition->webform['webform_ajax'] = WEBFORM_AJAX_NO_CONFIRM;
    $this->petition->webform['components'][1] = $component;
    node_save($this->petition);
  }

  /**
   * Delete test nodes.
   */
  public function tearDown() {
    node_delete($this->petition->nid);
    parent::tearDown();
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
    $redirect = new stdClass();
    $redirect->fragment = '';
    $submission = new stdClass();
    $submission->node = $this->petition;
    $submission->node->title = "Donation test & stuff";
    $submission->sid = 1;
    $nid = $submission->node->nid;
    campaignion_tracking_webform_redirect_alter($redirect, $submission);
    $this->assertEqual('t:t=s;w:nid=' . $nid . '&sid=1&title=Donation%20test%20%26%20stuff', $redirect->fragment);

    $redirect->fragment = 'something';
    campaignion_tracking_webform_redirect_alter($redirect, $submission);
    $this->assertEqual('something;t:t=s;w:nid=' . $nid . '&sid=1&title=Donation%20test%20%26%20stuff', $redirect->fragment);
  }

}
