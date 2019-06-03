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
    $list_stubs = array(4711 => NULL, 4712 => NULL);
    $addresses = array('test1@example.com', 'test2@example.com');
    $subscriptions = new Subscriptions($list_stubs, $addresses, array());

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

}
