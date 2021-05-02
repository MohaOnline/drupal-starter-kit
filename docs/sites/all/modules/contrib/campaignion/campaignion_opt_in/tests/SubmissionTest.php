<?php

namespace Drupal\campaignion_opt_in;

use Upal\DrupalUnitTestCase;

/**
 * Test submission creation and loading.
 */
class SubmissionTest extends DrupalUnitTestCase {

  /**
   * Create a test node.
   */
  public function setUp() : void {
    parent::setUp();
    $node = (object) ['type' => 'webform'];
    node_object_prepare($node);
    node_save($node);
    $this->node = node_load($node->nid);
  }

  /**
   * Delete the test node.
   */
  public function tearDown() : void {
    node_delete($this->node->nid);
    parent::tearDown();
  }

  /**
   * Test creating a submission adds the $submission->opt_in property.
   */
  public function testCreate() {
    $account = drupal_anonymous_user();
    $form_state['values']['submitted'] = [];
    module_load_include('submissions.inc', 'webform', 'includes/webform');
    $submission = webform_submission_create($this->node, $account, $form_state);
    $this->assertNotEmpty($submission->opt_in);
    $this->assertInstanceOf(Values::class, $submission->opt_in);
  }

  /**
   * Test loading a submission adds the $submission->opt_in property.
   */
  public function testLoad() {
    $submission = (object) ['nid' => $this->node->nid];
    $submissions = [$submission];
    campaignion_opt_in_webform_submission_load($submissions);
    $this->assertNotEmpty($submission->opt_in);
    $this->assertInstanceOf(Values::class, $submission->opt_in);
  }

}
