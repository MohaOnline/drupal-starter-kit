<?php

namespace Drupal\campaignion_newsletters;

use Upal\DrupalWebTestCase;

/**
 * Test the queue item model class.
 */
class QueueItemTest extends DrupalWebTestCase {

  /**
   * Test loading and saving a queue item.
   */
  public function testSaveAndLoadWithOptinInfo() {
    $i = new QueueItem([
      'action' => QueueItem::SUBSCRIBE,
      'list_id' => 1,
      'email' => 'test@example.org',
      'created' => 4711,
      'data' => [],
      'optin_info' => new FormSubmission('', '', 'http://example.com', 4712, []),
    ]);
    $i->save();
    $i = QueueItem::load(1, 'test@example.org');
    $this->assertInstanceOf('\\Drupal\\campaignion_newsletters\\QueueItem', $i);
    $this->assertInstanceOf('\\Drupal\\campaignion_newsletters\\FormSubmission', $i->optin_info);
    $this->assertEquals('http://example.com', $i->optin_info->url);
  }

  /**
   * Test that the order for a single list / email combo is preserved.
   */
  public function testClaimOldestKeepsOrdering() {
    $now = REQUEST_TIME;
    (new QueueItem([
      'created' => $now - 3,
      'list_id' => 1,
      'email' => 't1@example.org',
      'action' => QueueItem::SUBSCRIBE,
      'locked' => $now + 3600,
    ]))->save();
    (new QueueItem([
      'created' => $now - 2,
      'list_id' => 2,
      'email' => 't1@example.org',
      'action' => QueueItem::SUBSCRIBE,
    ]))->save();
    (new QueueItem([
      'created' => $now - 1,
      'list_id' => 1,
      'email' => 't1@example.org',
      'action' => QueueItem::UNSUBSCRIBE,
    ]))->save();
    $items = QueueItem::claimOldest(3);
    $this->assertCount(1, $items);
    $this->assertEqual(2, $items[0]->list_id);
  }

  /**
   * Remove all stray queue items from the DB.
   */
  function tearDown() : void {
    db_delete('campaignion_newsletters_queue')->execute();
  }

}
