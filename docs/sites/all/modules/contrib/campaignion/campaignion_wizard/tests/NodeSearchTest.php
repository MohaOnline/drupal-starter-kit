<?php

namespace Druppal\campaignion_wizard;

/**
 * Test the node search API used for conditional redirects.
 */
class NodeSearchTest extends \DrupalUnitTestCase {

  /**
   * Set up nodes for testing.
   */
  public function setUp() {
    parent::setUp();
    $node1 = (object) [
      'type' => 'petition',
      'title' => 'First test node',
    ];
    node_save($node1);
    $node2 = (object) [
      'type' => 'petition',
      'title' => "Second test node with number ({$node1->nid}).",
    ];
    node_save($node2);
    $this->nodes = [$node1, $node2];
  }

  /**
   * Delete test nodes.
   */
  public function tearDown() {
    foreach ($this->nodes as $node) {
      node_delete($node->nid);
    }
    parent::tearDown();
  }

  /**
   * Test selecting nodes by nid.
   */
  public function testSelectByNid() {
    $_GET['s'] = $this->nodes[0]->nid;
    $result = campaignion_wizard_search_nodes();
    $this->assertEqual([
      [
        'value' => "node/{$this->nodes[0]->nid}",
        'label' => "First test node [{$this->nodes[0]->nid}]",
      ],
      [
        'value' => "node/{$this->nodes[1]->nid}",
        'label' => $this->nodes[1]->title . " [{$this->nodes[1]->nid}]",
      ],
    ], $result['values']);
  }

  /**
   * Test selecting nodes by title.
   */
  public function testSelectByTitle() {
    $_GET['s'] = 'First test';
    $result = campaignion_wizard_search_nodes();
    $this->assertEqual([
      [
        'value' => "node/{$this->nodes[0]->nid}",
        'label' => "First test node [{$this->nodes[0]->nid}]",
      ],
    ], $result['values']);
  }

}
