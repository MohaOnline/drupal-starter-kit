<?php

namespace Drupal\campaignion_newsletters_optivo;

use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\Subscription;

/**
 * Test polling of unsubscribes.
 */
class SubscriberPollingTest extends \DrupalUnitTestCase {

  /**
   * The list used for testing.
   *
   * @var \Drupal\campaignion_newsletters\NewsletterList
   */
  protected $list;

  /**
   * Create a SubscriberPolling object that stub client objects.
   *
   * @param bool
   *   Whether the Unsubscribe service should yield unsubscribes.
   *
   * @return \Drupal\camaignion_newsletters_optivo\SubscriberPolling
   *   A newly created SubscriberPolling object.
   */
  protected function clientStubPolling($unsubscribe) {
    $mock_factory = $this->createMock(ClientFactory::class);
    $mock_client = $this->createMock(Client::class);
    $mock_client->method('__call')->willReturn($unsubscribe);
    $mock_factory->method('getClient')->willReturn($mock_client);
    return new SubscriberPolling($this->list->source, $mock_factory, 20, 10);
  }

  public function setUp() : void {
    $this->list = NewsletterList::fromData([
      'source' => 'test',
      'identifier' => 'test-4711',
      'language' => 'en',
      'title' => 'Test',
      'data' => [],
    ]);
    $this->list->save();
    $s = Subscription::fromData($this->list->list_id, 'test@example.com');
    $s->save(TRUE);
    // Set time so that last_sync is in the past.
    $t = REQUEST_TIME - 30;
    db_update('campaignion_newsletters_subscriptions')
      ->fields(['updated' => $t, 'last_sync' => $t])
      ->condition('email', $s->email)
      ->condition('list_id', $s->list_id)
      ->execute();
  }

  public function tearDown() : void {
    db_delete('campaignion_newsletters_subscriptions')
      ->condition('list_id', $this->list->list_id)
      ->execute();
    db_delete('campaignion_newsletters_lists')
      ->condition('source', 'test')
      ->execute();
  }

  /**
   * Test polling for provider-side unsubscribe.
   */
  public function testUnsubscribe() {
    // Simulate polling with unusbscribe.
    $this->clientStubPolling(TRUE)->poll(1.0);

    $s = Subscription::byData($this->list->list_id, 'test@example.com');
    $this->assertTrue($s->isNew());
  }

  /**
   * Test polling without any unsubscribe.
   */
  public function testNoUnsubscribe() {
    // Polling without unsubscribe.
    $this->clientStubPolling(FALSE)->poll(1.0);

    $s = Subscription::byData($this->list->list_id, 'test@example.com');
    $this->assertFalse($s->isNew());

    // Check if last poll time was updated correctly.
    $this->assertGreaterThanOrEqual(REQUEST_TIME, $s->last_sync);
  }

  /**
   * Polling again within the polling interval does nothing.
   */
  public function testPollIntervall() {
    // Don’t delete in first polling.
    $polling = $this->clientStubPolling(FALSE)->poll(1.0);
    // This should do nothing because it was already polled recently.
    $polling = $this->clientStubPolling(TRUE)->poll(1.0);

    $s = Subscription::byData($this->list->list_id, 'test@example.com');
    $this->assertFalse($s->isNew());
  }

}
