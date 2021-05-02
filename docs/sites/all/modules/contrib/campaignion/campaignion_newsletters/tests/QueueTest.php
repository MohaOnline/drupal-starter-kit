<?php

namespace Drupal\campaignion_newsletters;

use \Drupal\campaignion\Contact;

class QueueTest extends \DrupalUnitTestCase {

  function test_updateContactWhileCronIsRunnning() {
    $subscription = $this->getMockBuilder(Subscription::class)
      ->setMethods(['provider'])
      ->setConstructorArgs([['email' => 't@e.org', 'list_id' => 4711], TRUE])
      ->getMock();
    $provider = $this->createMock(ProviderBase::class);
    $provider->method('data')
      ->will($this->onConsecutiveCalls(
        [['test' => '1'], 'fingerprint1'],
        [['test' => '2'], 'fingerprint2']
      ));
    $subscription->expects($this->any())->method('provider')
      ->willReturn($provider);
    $subscription->save();
    $items = QueueItem::claimOldest(2);
    $this->assertCount(1, $items);

    $subscription->save();

    foreach ($items as $item) {
      $item->delete();
    }

    $items = QueueItem::claimOldest(2);
    $this->assertCount(1, $items, 'New data failed to override old (but claimed) data.');
  }

  function tearDown() : void {
    db_delete('campaignion_newsletters_subscriptions')->execute();
    db_delete('campaignion_newsletters_queue')->execute();
  }
}
