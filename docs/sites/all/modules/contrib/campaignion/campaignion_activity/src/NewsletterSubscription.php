<?php

namespace Drupal\campaignion_activity;

use Drupal\campaignion\ContactTypeManager;
use Drupal\campaignion\CRM\Import\Source\ArraySource;
use Drupal\campaignion_newsletters\Subscription;

/**
 * Partial model object to store newsletter subscription data for activities.
 */
class NewsletterSubscription extends ActivityBase {

  protected $type = 'newsletter_subscription';

  public $list_id;
  public $action;
  public $from_provider;
  public $optin_statement;
  public $remote_addr;

  /**
   * Create a new activity object from a subscription object.
   *
   * @param \Drupal\campaignion_newsletters\Subscription $subscription
   *   The subscription object.
   * @param string $action
   *   The change made to the subscription (subscribe, unsubscribe).
   * @param bool $from_provider
   *   Whether this change was initiatey by the newsletter provider.
   *
   * @return static
   */
  public static function fromSubscription(Subscription $subscription, $action, $from_provider) {
    $importer = ContactTypeManager::instance()->importer('campaignion_activity');
    $source = new ArraySource(['email' => $subscription->email]);
    $contact = $importer->findOrCreateContact($source);
    return new static([
      'contact_id' => $contact->contact_id,
      'list_id' => $subscription->list_id,
      'action' => $action,
      'from_provider' => (int) $from_provider,
      'optin_statement' => $subscription->optin_statement,
      'remote_addr' => self::getRemoteAddr(),
    ]);
  }

  /**
   * Load an activity by it’s id.
   *
   * @param int $activity_id
   *   The ID of the activity to load.
   */
  public static function load($activity_id) {
    $query = static::buildJoins();
    $query->condition('a.activity_id', $activity_id);
    return $query->execute()->fetchObject(static::class);
  }

  /**
   * Get the IP address of the requesting client.
   *
   * @return string
   *   Remote IP address
   */
  protected static function getRemoteAddr() {
    return ip_address();
  }

  /**
   * Build the database query for getting activities.
   */
  protected static function buildJoins() {
    $query = db_select('campaignion_activity', 'a')
      ->fields('a');
    $query->innerJoin('campaignion_activity_newsletter_subscription', 'ans', 'ans.activity_id=a.activity_id');
    $query->fields('ans');
    return $query;
  }

  /**
   * Save changes to the database.
   */
  protected function insert() {
    parent::insert();
    db_insert('campaignion_activity_newsletter_subscription')
      ->fields($this->values([
        'activity_id',
        'list_id',
        'action',
        'from_provider',
        'optin_statement',
        'remote_addr',
      ]))
      ->execute();
  }

  /**
   * Save changes to the database.
   */
  protected function update() {
    parent::update();
    // `optin_statement` and `remote_addr` intentionally left out, as
    // these are not supposed to be changed.
    db_update('campaignion_activity_newsletter_subscription')
      ->fields($this->values(['list_id', 'action', 'from_provider']))
      ->condition('activity_id', $this->activity_id)
      ->execute();
  }

}
