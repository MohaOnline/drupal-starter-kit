<?php

namespace Drupal\campaignion_newsletters;

class SubscriptionTest extends \DrupalWebTestCase {
  public function tearDown() {
    db_delete('campaignion_newsletters_subscriptions')->execute();
    db_delete('campaignion_newsletters_queue')->execute();
  }

  public function test_byData_doesntDuplicate() {
    $email = 'bydataduplicate@test.com';
    $list_id = 4711;
    $s = Subscription::byData($list_id, $email);
    $s->save();
    Subscription::byData($list_id, $email);
    $s->save();
    $this->assertFalse($s->isNew());
    $this->assertEqual(1, count(Subscription::byEmail($email)));
    $s->delete();
    $this->assertTrue($s->isNew());
    $this->assertEqual(0, count(Subscription::byEmail($email)));
  }

  public function test_delete_worksForNonExisting() {
    Subscription::fromData(4711, 'this@doesnot.exist')->delete();
  }

  /**
   * Test that a proper QueueItem exists a user does first opt-out then opt-in.
   */
  public function testOptOutThenOptIn() {
    $email = 'bydataduplicate@test.com';
    $list_id = 4711;
    $provider = $this->createMock(ProviderInterface::class);
    $provider->method('data')->willReturn([['data'], 'fingerprint']);
    $s = $this->getMockBuilder(Subscription::class)
      ->setMethods(['provider'])
      ->setConstructorArgs([[
        'list_id' => $list_id,
        'email' => $email,
      ], TRUE])
      ->getMock();
    $s->method('provider')->willReturn($provider);

    // Initial opt-in.
    $s->save(TRUE);
    // Opt-out.
    $s->delete();

    // New unsubscribe QueueItem.
    $item = QueueItem::load($list_id, $email);
    $this->assertEqual(QueueItem::UNSUBSCRIBE, $item->action);
    $this->assertNull($item->data);

    // Opt-in again.
    $s->save();

    // QueueItem was changed into a subscription again.
    $item = QueueItem::load($list_id, $email);
    $this->assertEqual(QueueItem::SUBSCRIBE, $item->action);
    $this->assertEqual(['data'], $item->data);
  }

}
