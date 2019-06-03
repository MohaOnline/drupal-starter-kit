<?php

namespace Drupal\campaignion_newsletters_optivo;

use \Drupal\campaignion_newsletters\PollingInterface;
use \Drupal\campaignion_newsletters\Subscription;


/**
 * Handles polling and importing of modified subscribers.
 */
class SubscriberPolling implements PollingInterface {

  /**
   * Machine name of the newsletters provider that we are polling.
   *
   * @var str
   */
  protected $provider;

  /**
   * A client factory instance.
   *
   * @var \Drupal\campaignion_newsletters_optivo\ClientFactory
   */
  protected $factory;

  /**
   * Number of email subscribers that are checked in each batch.
   *
   * @var int
   */
  protected $batchSize;

  /**
   * The minimal amount of time between polls of the same email address.
   *
   * @var int
   */
  protected $pollInterval;

  public static function create($provider ,ClientFactory $factory) {
    $page_size = variable_get('campaignion_newsletters_optivo_unsubscribe_poll_batch_size', 20);
    $interval = variable_get('campaignion_newsletters_optivo_unsubscribe_poll_interval', 7 * 24 * 3600);
    return new static($provider, $factory, $page_size, $interval);
  }

  public function __construct($provider, ClientFactory $factory, $batch_size, $poll_interval) {
    $this->provider = $provider;
    $this->factory = $factory;
    $this->batchSize = $batch_size;
    $this->pollInterval = $poll_interval;
  }

  /**
   * Check for unsubscribed email addresses.
   *
   * @return bool
   *   FALSE when there are no more addresses that need polling.
   */
  public function pollUnsubscribes() {
    $sql = <<<SQL
SELECT list_id, email, identifier
FROM {campaignion_newsletters_subscriptions}
  INNER JOIN {campaignion_newsletters_lists} USING(list_id)
WHERE source=:source AND last_sync<=:most_recent
SQL;
    $args[':source'] = $this->provider;
    $args[':most_recent'] = REQUEST_TIME - $this->pollInterval;
    if ($rows = db_query_range($sql, 0, $this->batchSize, $args)->fetchAll()) {
      $client = $this->factory->getClient('Unsubscribe');
      foreach ($rows as $row) {
        $subscription = Subscription::byData($row->list_id, $row->email);
        if ($client->containsByRecipientList($row->identifier, $row->email)) {
          $subscription->delete(TRUE);
        }
        else {
          $subscription->last_sync = time();
          $subscription->save(TRUE, FALSE);
        }
      }
      return TRUE;
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function poll() {
    return $this->pollUnsubscribes();
  }

}
