<?php

namespace Drupal\campaignion_newsletters_dotmailer;

use \Drupal\campaignion\CRM\Import\Source\ArraySource;
use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\Subscription;

/**
 * Test the Provider implementation.
 */
class ProviderTest extends \DrupalUnitTestCase {

  /**
   * Construct a partially stubbed Provider object using a mock Api object.
   */
  protected function mockProvider() {
    $api = $this->getMockBuilder(Api\Client::class)
      ->setMethods([
      ])
      ->disableOriginalConstructor()
      ->getMock();
    $cr = $this->getMockBuilder(Provider::class)
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
   * Test generating data for subscriptions.
   */
  public function testData() {
    $subscription = $this->mockSubscription('test@example.com', (object) [
      'fields' => [
        ['name' => 'FIRSTNAME'],
        ['name' => 'LASTNAME'],
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
      'FIRSTNAME' => 'test',
    ], $data);

    // Test item with existing data.
    list($cr, $api) = $this->mockProvider();
    $source2 = new ArraySource([
      'lastname' => 'test',
    ]);
    $cr->method('getSource')->willReturn($source2);
    list($data, $fingerprint) = $cr->data($subscription, $data);
    $this->assertEqual([
      'FIRSTNAME' => 'test',
      'LASTNAME' => 'test',
    ], $data);
  }

}
