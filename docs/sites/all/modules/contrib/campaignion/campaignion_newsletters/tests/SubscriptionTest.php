<?php

namespace Drupal\campaignion_newsletters;

class SubscriptionTest extends \DrupalWebTestCase {

  public function tearDown() : void {
    db_delete('campaignion_newsletters_subscriptions')->execute();
    db_delete('campaignion_newsletters_queue')->execute();
  }

  /**
   * Test `byData()` doesn’t duplicate Subscriptions.
   */
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

  /**
   * Test deleting a non-existing Subscription.
   */
  public function test_delete_worksForNonExisting() {
    Subscription::fromData(4711, 'this@doesnot.exist')->delete();
  }

  /**
   * Test that a proper QueueItem exists if a user does first opt-out then -in.
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
    $s->delete = FALSE;
    $s->save();

    // QueueItem was changed into a subscription again.
    $item = QueueItem::load($list_id, $email);
    $this->assertEqual(QueueItem::SUBSCRIBE, $item->action);
    $this->assertEqual(['data'], $item->data);
  }

  /**
   * Test opt-in then update: QueueItem keeps subscribe status.
   */
  public function testOptInThenUpdate() {
    $email = 'subscriber@example.com';
    $list_id = 4711;
    $provider = $this->createMock(ProviderInterface::class);
    $provider->method('data')->will($this->onConsecutiveCalls(
      [['data'], 'fingerprint'],
      [['new-data'], 'new-fingerprint']
    ));
    $s = $this->getMockBuilder(Subscription::class)
      ->setMethods(['provider'])
      ->setConstructorArgs([[
        'list_id' => $list_id,
        'email' => $email,
      ], TRUE])
      ->getMock();
    $s->method('provider')->willReturn($provider);

    // Initial opt-in.
    $s->save();

    // Subcribe QueueItem
    $item = QueueItem::load($list_id, $email);
    $this->assertEqual(QueueItem::SUBSCRIBE, $item->action);
    $this->assertEqual(['data'], $item->data);

    // Update subscription data.
    $s->save();

    // QueueItem is still for subscribing although the data was updated.
    $item = QueueItem::load($list_id, $email);
    $this->assertEqual(QueueItem::SUBSCRIBE, $item->action);
    $this->assertEqual(['new-data'], $item->data);
  }

  /**
   * Test merging subscriptions.
   */
  public function testMerge() {
    $c1 = ['cid' => 1, 'extra' => ['opt_in_implied' => 1, 'send_welcome' => 1]];
    $c2 = ['cid' => 2, 'extra' => ['opt_in_implied' => 0, 'send_welcome' => 0]];
    $email = 'merge@test.com';
    $s1 = Subscription::byData(1, $email, [
      'fingerprint' => 'fingerprint1',
      'components' => [$c1],
    ]);
    $s2 = Subscription::byData(1, $email, [
      'fingerprint' => 'fingerprint2',
      'components' => [$c2],
    ]);
    $s1->merge($s2);

    $this->assertEqual($s1->components, [$c1, $c2]);
    // Fingerprint is reset.
    $this->assertEqual($s1->fingerprint, '');
    // TRUE wins.
    $args = $s1->queueItemArgs();
    $this->assertTrue($args['send_welcome']);
    $this->assertTrue($args['send_optin']);
  }
}
