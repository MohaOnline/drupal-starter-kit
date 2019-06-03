<?php

namespace Drupal\campaignion_newsletters_optivo;

use \Drupal\campaignion\CRM\Import\Source\ArraySource;
use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\QueueItem;
use \Drupal\campaignion_newsletters\Subscription;

/**
 * Test the Provider implementation.
 */
class OptivoTest extends \DrupalUnitTestCase {

  /**
   * Construct a partially stubbed Optivo object using a mock Client object.
   */
  protected function mockProvider($overrides = []) {
    $overrides[] = 'getSource';
    $api = $this->getMockBuilder(Client::class)
      ->setMethods([
      ])
      ->disableOriginalConstructor()
      ->getMock();
    $cr = $this->getMockBuilder(Optivo::class)
      ->setMethods($overrides)
      ->disableOriginalConstructor()
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
   * Test generating data for subscriptions.
   */
  public function testData() {
    $subscription = $this->mockSubscription('test@example.com', (object) [
      'attributeNames' => [
        'Firstname',
        'Lastname',
      ],
    ]);

    // Test new item.
    list($cr, $api) = $this->mockProvider();
    $source1 = new ArraySource([
      'firstname' => 'test',
    ]);
    $cr->method('getSource')->willReturn($source1);
    list($data, $fingerprint) = $cr->data($subscription, []);
    $this->assertEqual([
      'names' => ['Firstname'],
      'values' => ['test'],
    ], $data);

    // Test item with existing data.
    list($cr, $api) = $this->mockProvider();
    $source2 = new ArraySource([
      'lastname' => 'test',
    ]);
    $cr->method('getSource')->willReturn($source2);
    list($data, $fingerprint) = $cr->data($subscription, $data);
    $this->assertEqual([
      'names' => ['Firstname', 'Lastname'],
      'values' => ['test', 'test'],
    ], $data);
  }

  /**
   * Test that the update function simply calls subscribe().
   */
  public function testUpdateCallsSubscribe() {
    list($cr, $api) = $this->mockProvider(['subscribe']);
    $l = new NewsletterList(['list_id' => 'list-id']);
    $q = new QueueItem([
      'list_id' => 'list-id',
      'email' => 'test@example.com',
      'data' => ['names' => [], 'values' => []],
    ]);
    $cr->expects($this->once())->method('subscribe')
      ->with($this->equalTo($l), $this->equalTo($q));
    $cr->update($l, $q);
  }

}
