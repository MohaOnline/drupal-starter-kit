<?php

namespace Drupal\campaignion_newsletters;

class CronRunner {

  protected $sendBatchSize;
  protected $pollTime;

  /**
   * Create a CronRunner instance based on configuration variables.
   */
  public static function fromConfig() {
    $batch_size = variable_get('campaignion_newsletters_batch_size', 50);
    $poll_time = variable_get('campaignion_newsletters_poll_time', 10);
    return new static($batch_size, $poll_time);
  }

  /**
   * Instantiate and run sendQueue job.
   */
  public static function cronSendQueue() {
    static::fromConfig()->sendQueue();
  }

  /**
   * Instantiate and run poll job.
   */
  public static function cronPoll() {
    static::fromConfig()->poll();
  }


  public function __construct($batch_size, $poll_time) {
    $this->sendBatchSize = $batch_size;
    $this->pollTime = $poll_time;
  }

  /**
   * Send a batch of queue items to their respective provider.
   */
  public function sendQueue() {
    $lists = NewsletterList::listAll();
    $items = QueueItem::claimOldest($this->sendBatchSize);

    foreach ($items as $item) {
      $list = $lists[$item->list_id];
      $provider = $list->provider();
      $method = $item->action;

      try {
        $provider->{$method}($list, $item);
        $item->delete();
      }
      catch (ApiError $e) {
        $e->log();
        if ($e->isPersistent()) {
          // There is no point to items with persistent errors in the queue.
          $item->delete();
        }
      }
    }
  }

  /**
   * Generator to loop over all providers.
   */
  protected function getProviders() {
    $f = ProviderFactory::getInstance();
    foreach ($f->providers() as $key) {
      yield $f->providerByKey($key);
    }
  }

  /**
   * Let the provider(s) poll their subscriber lists for a given amount of time.
   */
  public function poll() {
    $end = microtime(TRUE) + $this->pollTime;

    foreach ($this->getProviders() as $provider) {
      $continue = TRUE;
      while ($continue && microtime(TRUE) < $end) {
        $poll = $provider->polling();
        $continue = $poll && $poll->poll();
      }
    }
  }

}
