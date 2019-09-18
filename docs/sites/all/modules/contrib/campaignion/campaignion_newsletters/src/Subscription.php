<?php

namespace Drupal\campaignion_newsletters;

use Drupal\little_helpers\DB\Model;

/**
 * Subscription model.
 */
class Subscription extends Model {
  public $list_id;
  public $email;
  public $fingerprint = '';
  public $updated = NULL;
  public $last_sync = 0;

  public $delete = FALSE;
  public $source = NULL;
  public $components = [];

  public static $lists = array();

  protected static $table = 'campaignion_newsletters_subscriptions';
  protected static $key = array('list_id', 'email');
  protected static $values = array('fingerprint', 'updated', 'last_sync');
  protected static $serial = FALSE;

  /**
   * Get an instance from DB or create a new one.
   *
   * @param string $list_id
   *   A list id.
   * @param string $email
   *   An email address.
   * @param array $data
   *   Additional data to pass to the constructor.
   */
  public static function byData($list_id, $email, array $data = []) {
    $result = db_select(static::$table, 's')
      ->fields('s')
      ->condition('list_id', $list_id)
      ->condition('email', $email)
      ->execute();
    if ($row = $result->fetchAssoc()) {
      return new static($row + $data, FALSE);
    }
    else {
      return static::fromData($list_id, $email, $data);
    }
  }

  /**
   * Construct a new instance from data.
   *
   * @param string $list_id
   *   A list id.
   * @param string $email
   *   An email address.
   * @param array $data
   *   Additional data to pass to the constructor.
   */
  public static function fromData($list_id, $email, array $data = []) {
    return new static(array(
      'list_id' => $list_id,
      'email' => $email,
    ) + $data, TRUE);
  }

  /**
   * Load all subscriptions for one email address.
   *
   * @param string $email
   *   An email address.
   *
   * @return static[]
   *   List of subscriptions.
   */
  public static function byEmail($email) {
    $subscriptions = array();
    $result = db_select(static::$table, 's')
      ->fields('s')
      ->condition('email', $email)
      ->execute();
    foreach ($result as $row) {
      $subscriptions[] = new static($row, FALSE);
    }
    return $subscriptions;
  }

  /**
   * Bulk delete subscriptions based on their list.
   *
   * @param int $list_id
   *   All queue items with this $list_id will be deleted.
   *
   * @return int
   *   Number of affected rows.
   */
  public static function bulkDelete($list_id) {
    return db_delete(static::$table)
      ->condition('list_id', $list_id)
      ->execute();
  }

  /**
   * Get the newsletter list instance.
   *
   * @return \Drupal\campaignion_newsletters\NewsletterList
   *   The newsletter list for this subscription.
   */
  public function newsletterList() {
    if (!isset(self::$lists[$this->list_id])) {
      self::$lists[$this->list_id] = NewsletterList::load($this->list_id);
    }
    return self::$lists[$this->list_id];
  }

  /**
   * Save the subscription to the database.
   *
   * @param bool $from_provider
   *   TRUE if this is an update based on data from the provider and thus should
   *   not be sent to provider.
   * @param bool $update
   *   TRUE if the updated timestamp should be updated.
   */
  public function save($from_provider = FALSE, $update = TRUE) {
    if ($update) {
      $this->updated = REQUEST_TIME;
    }
    $this->last_sync = max([$this->last_sync, $this->updated]);
    module_invoke_all('campaignion_newsletters_subscription_presave', $this);
    if ($this->delete) {
      return $this->delete($from_provider);
    }
    if (!$from_provider && ($provider = $this->provider())) {
      $item = QueueItem::fromSubscription($this, $provider);
      if ($item->fingerprint != $this->fingerprint) {
        $this->fingerprint = $item->fingerprint;
        $item->save();
      }
    }
    db_merge(static::$table)
      ->key($this->values(static::$key))
      ->fields($this->values(static::$values))
      ->execute();
    $was_new = $this->new;
    $this->new = FALSE;
    module_invoke_all('campaignion_newsletters_subscription_saved', $this, $from_provider, $was_new);
  }

  /**
   * Get the newsletter provider for this subscriptions's list.
   *
   * @return \Drupal\campaignion_newsletters\ProviderInterface
   *   The provider object.
   */
  protected function provider() {
    if (($l = $this->newsletterList()) && ($p = $l->provider())) {
      return $p;
    }
  }

  /**
   * Merge data from another subscription into this subscription.
   *
   * Assumes list_id, email and source are the same.
   * Ignores last_sync and updated timestamps.
   *
   * @param Subscription $subscription
   *   Another subscription for the same email address and list.
   */
  public function merge(Subscription $subscription) {
    $this->components = array_merge($this->components, $subscription->components);
    $this->delete = $this->delete && $subscription->delete;
    $this->fingerprint = '';
  }

  /**
   * Delete the subscription from the database.
   *
   * @param bool $from_provider
   *   TRUE if this is an unsubscribe from the provider and thus needs not be
   *   forwarded to provider again.
   */
  public function delete($from_provider = FALSE) {
    $this->delete = TRUE;
    if (!$from_provider) {
      QueueItem::fromSubscription($this)->save();
    }
    parent::delete();
    module_invoke_all('campaignion_newsletters_subscription_deleted', $this, $from_provider);
    $this->fingerprint = '';
  }

  /**
   * Calculate the arguments for a queue item.
   */
  public function queueItemArgs() {
    $args['send_welcome'] = FALSE;
    $args['send_optin'] = FALSE;
    foreach ($this->components as $component) {
      $args['send_welcome'] = $args['send_welcome'] || !empty($component['extra']['send_welcome']);
      $args['send_optin'] = $args['send_optin'] || empty($component['extra']['opt_in_implied']);
    }
    return $args;
  }

}
