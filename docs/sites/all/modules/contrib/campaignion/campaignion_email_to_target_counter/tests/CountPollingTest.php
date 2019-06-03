<?php

namespace Drupal\campaignion_email_to_target_counter;

/**
 * Test that polling the counts works.
 */
class CountPollingTest extends \DrupalWebTestCase {

  protected $node = NULL;

  public function setUp() {
    $this->node = (object) ['nid' => 123456789];
    db_merge('campaignion_email_to_target_counter')
      ->fields(['label' => 'Test target 1', 'count' => 1])
      ->key(['nid' => $this->node->nid, 'target_id' => 't1'])
      ->execute();
    db_merge('campaignion_email_to_target_counter')
      ->fields(['label' => 'Test target 2', 'count' => 2])
      ->key(['nid' => $this->node->nid, 'target_id' => 't2'])
      ->execute();
  }

  public function testGetData() {
      $p = new CountPolling($this->node->nid);
      $expected['campaignion_email_to_target_counter'] = [
        't2' => ['label' => 'Test target 2', 'count' => 2],
        't1' => ['label' => 'Test target 1', 'count' => 1],
      ];
      $this->assertEqual($expected, $p->getData());

      $p = new CountPolling(NULL);
      $this->assertEqual([], $p->getData());
  }

  public function tearDown() {
    campaignion_email_to_target_counter_node_delete($this->node);
  }

}
