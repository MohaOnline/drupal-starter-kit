<?php

namespace Drupal\campaignion_newsletters;

/**
 * Test the subscriptions matrix.
 */
class SubscriptionsTest extends \DrupalWebTestCase {

  /**
   * Test storing new subscriptions.
   */
  public function testSubscribeNew() {
    $list_stubs = [4711 => NULL, 4712 => NULL];
    $addresses = ['test1@example.com', 'test2@example.com'];
    $subscriptions = new Subscriptions($list_stubs, $addresses, []);

    $values[$addresses[0]][4711] = 4711;
    $values[$addresses[1]][4712] = 4712;
    $subscriptions->update($values);
    $subscriptions->save();

    $s1 = Subscription::byEmail($addresses[0]);
    $this->assertEquals(1, count($s1));
    $this->assertEquals(4711, $s1[0]->list_id);

    $s2 = Subscription::byEmail($addresses[1]);
    $this->assertEquals(1, count($s2));
    $this->assertEquals(4712, $s2[0]->list_id);
  }

  /**
   * Test merging subscriptions.
   */
  public function testMergeSubscriptions() {
    $list_stubs = [1 => NULL, 2 => NULL];
    $email = 'example@test.com';
    $subscriptions = new Subscriptions($list_stubs, [], []);

    $subscriptions->merge([
      Subscription::byData(1, $email),
      Subscription::byData(2, $email),
    ]);
    $this->assertCount(2, $subscriptions->values($email));

    // Opt-in wins over opt-out.
    $subscriptions->merge([Subscription::byData(1, $email, ['delete' => TRUE])]);
    $this->assertCount(2, $subscriptions->values($email));
    $this->assertEquals(1, $subscriptions->values($email)[1]);
  }

}
