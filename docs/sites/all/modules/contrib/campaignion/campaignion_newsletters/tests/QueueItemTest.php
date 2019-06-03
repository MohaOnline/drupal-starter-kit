<?php

namespace Drupal\campaignion_newsletters;


class QueueItemTest extends \DrupalWebTestCase {
  public function testSaveAndLoad_withOptinInfo() {
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

  function tearDown() {
    db_delete('campaignion_newsletters_queue')->execute();
  }
}
