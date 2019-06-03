<?php

namespace Drupal\campaignion_wizard;

use Upal\DrupalUnitTestCase;

/**
 * Test the thank you page step.
 */
class ThankyouStepTest extends DrupalUnitTestCase {

  /**
   * Create a test node.
   */
  public function setUp() {
    parent::setUp();
    $node = (object) ['type' => 'petition', 'title' => __CLASS__];
    node_object_prepare($node);
    node_save($node);
    $this->node = node_load($node->nid);
  }

  /**
   * Test loading the node’s thank you step.
   */
  public function testLoadStep() {
    $wizard = new PetitionWizard([], $this->node, $this->node->type);
    $page = $wizard->run('thank');
    $this->assertNotEmpty($page[0]['thank_you_node']);
  }

  /**
   * Remove the test node.
   */
  public function tearDown() {
    node_delete($this->node->nid);
    parent::tearDown();
  }

}
