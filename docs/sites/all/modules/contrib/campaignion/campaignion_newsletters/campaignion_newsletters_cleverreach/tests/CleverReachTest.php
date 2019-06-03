<?php

namespace Drupal\campaignion_newsletters_cleverreach;

use \Drupal\campaignion\CRM\Import\Source\ArraySource;
use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\QueueItem;
use \Drupal\campaignion_newsletters\Subscription;

/**
 * Test the CleverReach API implementation.
 */
class CleverReachTest extends \DrupalUnitTestCase {

  /**
   * Construct a partially stubbed CleverReach object using a mock Api object.
   */
  protected function mockProvider() {
    $api = $this->getMockBuilder(ApiClient::class)
      ->setMethods([
        'receiverGetByEmail',
        'receiverAdd',
        'receiverUpdate',
        'formsSendActivationMail',
      ])
      ->disableOriginalConstructor()
      ->getMock();
    $cr = $this->getMockBuilder(CleverReach::class)
      ->setMethods(['getSource'])
      ->setConstructorArgs([$api, 'test'])
      ->getMock();
    return [$cr, $api];
  }

  /**
   * Generate a mock subscription with an accompanying list.
   *
   * @param string $email
   *   An email address.
   * @param mixed $data
   *   List data.
   *
   * @return \Drupal\campaignion_newsletters\Subscription
   *   A newly created subscription object.
   */
  protected function mockSubscription($email, $data) {
    $subscription = $this->getMockBuilder(Subscription::class)
      ->setMethods(['newsletterList'])
      ->setConstructorArgs([['email' => $email], TRUE])
      ->getMock();
    $subscription->expects($this->any())->method('newsletterList')
      ->will($this->returnValue(new NewsletterList([
        'list_id' => 2048,
        'data' => $data,
      ])));
    return $subscription;
  }

  /**
   * Test that subscribe() does not pass 'registered' when updating subscribers.
   */
  public function testSubscribeUpdateNoRegisteredDate() {
    list($cr, $api) = $this->mockProvider();

    $result = (object) [
      'status' => 'SUCCESS',
      'data' => [],
    ];
    $item = new QueueItem(['created' => 42]);
    $list = new NewsletterList(['data' => (object) ['id' => 42]]);
    $api->expects($this->once())->method('receiverUpdate')
      ->with($this->anything(), $this->equalTo([
        'email' => $item->email,
        'attributes' => NULL,
        'active' => TRUE,
        'activated' => 42,
      ]))->willReturn($result);
    $api->method('receiverGetByEmail')
      ->willReturn((object) ['message' => 'found']);
    $cr->subscribe($list, $item);
  }

  /**
   * Test generating data for subscriptions.
   */
  public function testData() {
    $subscription = $this->mockSubscription('test@example.com', (object) [
      'attributes' => [(object) ['key' => 'firstname']],
      'globalAttributes' => [(object) ['key' => 'lastname']],
    ]);

    // Test new item.
    list($cr, $api) = $this->mockProvider();
    $source1 = new ArraySource([
      'firstname' => 'test',
    ]);
    $cr->method('getSource')->willReturn($source1);
    list($data, $fingerprint) = $cr->data($subscription, []);
    $this->assertEqual([
      ['key' => 'firstname', 'value' => 'test'],
    ], $data);

    // Test item with existing data.
    list($cr, $api) = $this->mockProvider();
    $source2 = new ArraySource([
      'lastname' => 'test',
    ]);
    $cr->method('getSource')->willReturn($source2);
    list($data, $fingerprint) = $cr->data($subscription, $data);
    $this->assertEqual([
      ['key' => 'firstname', 'value' => 'test'],
      ['key' => 'lastname', 'value' => 'test'],
    ], $data);
  }

}
