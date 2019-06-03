<?php

namespace Drupal\campaignion_manage;

use Drupal\campaignion_manage\BulkOp\ContentPublish;
use Drupal\campaignion_manage\BulkOp\ContentUnpublish;

/**
 * Contains tests for the ContentPublish and ContentUnpublish classes.
 */
class UnPublishTest extends \DrupalUnitTestCase {
  protected $draftNode = NULL;
  protected $publicNode = NULL;

  /**
   * Prepares a `Campaign` node for testing.
   */
  public function setUp() {
    parent::setUp(['campaignion_test']);

    $draft_campaign = (object) [
      'title' => 'Testcampaign',
      'type' => 'campaign',
      'status' => 0,
    ];
    node_save($draft_campaign);
    $this->draftNode = $draft_campaign;

    $public_campaign = (object) [
      'title' => 'A campaign',
      'type' => 'campaign',
      'status' => 1,
    ];
    node_save($public_campaign);
    $this->publicNode = $public_campaign;
  }

  /**
   * Checks if anonymous users are disallowed from (un)publishing.
   */
  public function testAnonymousUser() {
    $publish_job = new ContentPublish();
    $unpublish_job = new ContentUnpublish();

    // Check that publishing the draft node fails.
    $msgs = $publish_job->apply([$this->draftNode->nid], []);
    $this->assertNotEmpty($msgs);

    // Check that 'unpublishing' the draft node results in no
    // errors.
    $msgs = $unpublish_job->apply([$this->draftNode->nid], []);
    $this->assertEmpty($msgs);

    // Check that unpublishing a public node fails.
    $msgs = $unpublish_job->apply([$this->publicNode->nid], []);
    $this->assertNotEmpty($msgs);

    // Check that 'publishing' a public node results in no errors.
    $msgs = $publish_job->apply([$this->publicNode->nid], []);
    $this->assertEmpty($msgs);
  }

  /**
   * Deletes the test `Campaign` node.
   */
  public function tearDown() {
    node_delete($this->draftNode->nid);
    node_delete($this->publicNode->nid);
  }

}
