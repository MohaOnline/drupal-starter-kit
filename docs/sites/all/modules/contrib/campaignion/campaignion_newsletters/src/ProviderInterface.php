<?php
/**
 * @file
 * Abstract base class for email marketing providers
 */

namespace Drupal\campaignion_newsletters;

interface ProviderInterface {

  public static function fromParameters(array $params);
  /**
   * Fetches current lists from the provider.
   *
   * @return array
   *   An array of Drupal\campaignion_newsletters\NewsletterList objects
   */
  public function getLists();

  /**
   * Fetches current lists of subscribers from the provider.
   *
   * @return array
   *   an array of subscribers.
   */
  public function getSubscribers($list);

  /**
   * Subscribe a user, given a newsletter identifier and email address.
   */
  public function subscribe(NewsletterList $newsletter, QueueItem $item);

  /**
   * Update user data without modifying subscription status.
   */
  public function update(NewsletterList $newsletter, QueueItem $item);

  /**
   * Subscribe a user, given a newsletter identifier and email address.
   *
   * Should ignore the request if there is no such subscription.
   */
  public function unsubscribe(NewsletterList $newsletter, QueueItem $item);

  /**
   * Get additional data for this subscription and a unique fingerprint.
   *
   * @param Subscription $subscription
   *   The subscription object.
   * @param mixed|null $old_data
   *   Data from an existing queue item or NULL if there is none.
   *
   * @return array
   *   An array containing some data object and a fingerprint:
   *   array($data, $fingerprint).
   *   - The $data is passed as $data parameter of subscribe() during
   *     cron runs.
   *   - The $fingerprint must be an sha1-hash. Usually it's a hash
   *     of some subset of $data.
   */
  public function data(Subscription $subscription, $old_data);

  /**
   * Get a provider polling object if this provider uses polling.
   *
   * @return \Drupal\campaignion_newsletters\PollingInterface|null
   *   A polling object or NULL if the provider doesn’t implement polling.
   */
  public function polling();

}
