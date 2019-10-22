<?php

namespace Drupal\campaignion_newsletters;

/**
 * Manage a matrix of subscriptions statuses.
 */
class Subscriptions {

  /**
   * Subscriptions keyed by email and list ID.
   *
   * @var \Drupal\campaignion_newsletters\Subscription[][]
   */
  protected $subscriptions;

  /**
   * Static cache for all newsletter lists.
   *
   * @var \Drupal\campaignion_newsletters\NewsletterList[]
   */
  protected static $lists = NULL;

  /**
   * Generate a new subscription matrix for a contact.
   *
   * @param \RedhenContact $contact
   *   The redhen contact to generate the matrix for.
   */
  public static function byContact(\RedhenContact $contact) {
    $lists = static::lists();
    $addresses = array();
    foreach ($contact->allEmail() as $address) {
      $addresses[] = $address['value'];
    }
    $storedSubscriptions = Subscription::byEmail($addresses);
    return new static($lists, $addresses, $storedSubscriptions);
  }

  /**
   * Construct a new subscription matrix.
   *
   * @param string[] $lists
   *   Associative array of all list titles keyed by list ID.
   * @param string[] $addresses
   *   Array of email addresses that may be subscribed.
   * @param \Drupal\campaignion_newsletters\Subscription[] $stored_subscriptions
   *   Existing subscriptions.
   */
  public function __construct(array $lists, array $addresses, array $stored_subscriptions) {
    $subscriptions = array();
    foreach ($addresses as $email) {
      $subscriptions[$email] = array();
      foreach (array_keys($lists) as $list_id) {
        $subscriptions[$email][$list_id] = NULL;
      }
    }

    foreach ($stored_subscriptions as $s) {
      $subscriptions[$s->email][$s->list_id] = $s;
    }
    $this->subscriptions = $subscriptions;
  }

  /**
   * Get an array of all newsletter lists.
   *
   * @return string[]
   *   Associative array of all list titles keyed by list ID.
   */
  public static function lists() {
    if (!isset(static::$lists)) {
      static::$lists = NewsletterList::options();
    }
    return static::$lists;
  }

  /**
   * Update the subscription matrix using a matrix of booleans.
   *
   * @param bool[][] $values
   *   New subscription statuses keyed by email and list ID:
   *   - TRUE for an active or new subscription.
   *   - FALSE for no subscription or for deleting the subscription.
   */
  public function update(array $values) {
    foreach ($values as $email => $lists) {
      foreach ($lists as $list_id => $subscribed) {
        if ($subscription = &$this->subscriptions[$email][$list_id]) {
          $subscription->delete = !$subscribed;
        }
        elseif ($subscribed) {
          $subscription = Subscription::fromData($list_id, $email);
        }
      }
    }
  }

  /**
   * Update the subscription matrix by merging a list of subscriptons.
   *
   * In case there are opt-ins and opt-outs for the same email and list,
   * the opt-outs will be ignored and the opt-ins merged.
   *
   * @param \Drupal\campaignion_newsletters\Subscription[] $subscriptions
   *   A list of subscriptions.
   */
  public function merge(array $subscriptions) {
    foreach ($subscriptions as $subscription) {
      if ($existing = &$this->subscriptions[$subscription->email][$subscription->list_id]) {
        $existing->merge($subscription);
      }
      else {
        $this->subscriptions[$subscription->email][$subscription->list_id] = $subscription;
      }
    }
  }

  /**
   * Make all changes to the matrix persistent.
   */
  public function save() {
    foreach ($this->subscriptions as $email => $lists) {
      foreach ($lists as $list_id => $subscription) {
        if ($subscription) {
          $subscription->save();
        }
      }
    }
  }

  /**
   * Remove all subscriptions.
   */
  public function unsubscribeAll() {
    foreach ($this->subscriptions as $email => $lists) {
      foreach ($lists as $list_id => $subscription) {
        if ($subscription) {
          $subscription->delete = TRUE;
        }
      }
    }
  }

  /**
   * Generate a values array suitable for the form-API #default_value.
   *
   * @param string $email
   *   The email address for which to generate the options array.
   *
   * @return bool[]
   *   Subscription status keyed by list_id.
   */
  public function values($email) {
    $values = array();
    foreach ($this->subscriptions[$email] as $list_id => $subscription) {
      $values[$list_id] = ($subscription && !$subscription->delete) ? $list_id : 0;
    }
    return $values;
  }

}
