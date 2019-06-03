<?php

namespace Drupal\campaignion_email_to_target;

/**
 * Test generation of pretty URLs for redirects.
 */
class MessageTemplatePrettyUrlTest extends \DrupalUnitTestCase {

  /**
   * Create a test node.
   */
  public function setUp() {
    parent::setUp();
    $this->node = (object) [
      'type' => 'petition',
      'title' => 'Redirect here',
    ];
    node_save($this->node);
  }

  /**
   * Delete the test node.
   */
  public function tearDown() {
    node_delete($this->node->nid);
    parent::tearDown();
  }

  /**
   * Test generation of the pretty URL.
   */
  public function testRedirectToArray() {
    $node = $this->node;
    $t = new MessageTemplate([
      'type' => 'exclusion',
      'label' => 'Exclusion',
      'url' => "node/{$node->nid}",
    ]);
    $this->assertEqual("{$node->title} [{$node->nid}]", $t->toArray()['urlLabel']);
  }

}

