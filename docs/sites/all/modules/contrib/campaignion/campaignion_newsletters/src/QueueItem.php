<?php

namespace Drupal\campaignion_newsletters;

use Drupal\little_helpers\DB\Model;

/**
 * Model class for newsletter queue items.
 *
 * Queue items represent data that’s to be sent to the newsletter provider.
 */
class QueueItem extends Model {

  const SUBSCRIBE = 'subscribe';
  const UNSUBSCRIBE = 'unsubscribe';
  const UPDATE = 'update';

  public $list_id;
  public $email;
  public $created;
  public $locked = 0;
  public $action;
  public $args = [];
  public $data;
  public $optin_info = NULL;
  public $fingerprint;

  protected static $table = 'campaignion_newsletters_queue';
  protected static $key = ['id'];
  protected static $values = [
    'list_id',
    'email',
    'created',
    'locked',
    'action',
    'args',
    'data',
    'optin_info',
  ];
  protected static $serialize = [
    'args' => TRUE,
    'data' => TRUE,
    'optin_info' => TRUE,
  ];
  protected static $serial = TRUE;

  /**
   * Load a queue item by its primary keys.
   */
  public static function load($list_id, $email) {
    $table = static::$table;
    $keys = [':list_id' => $list_id, ':email' => $email, ':now' => REQUEST_TIME];
    $result = db_query("SELECT * FROM {{$table}} WHERE list_id=:list_id AND email=:email AND locked<:now ORDER BY created DESC LIMIT 1", $keys);
    if ($row = $result->fetch()) {
      return new static($row, FALSE);
    }
  }

  /**
   * Load a queue item by its id.
   */
  public static function byId($id) {
    $table = static::$table;
    $keys = [':id' => $id];
    $result = db_query("SELECT * FROM {{$table}} WHERE id=:id", $keys);
    if ($row = $result->fetch()) {
      return new static($row, FALSE);
    }
  }

  /**
   * Load or create queue item.
   */
  public static function byData($data) {
    if ($item = static::load($data['list_id'], $data['email'])) {
      $item->__construct($data, FALSE);
    }
    else {
      $item = new static($data);
    }
    return $item;
  }

  /**
   * Load and lock queue items in order to process them.
   *
   * @param int $limit
   *   Maximum number of queue items to load.
   * @param int $time
   *   Duration of the lock.
   *
   * @return \Drupal\campaignion_newsletters\QueueItem[]
   *   An array of locked queue items.
   */
  public static function claimOldest($limit, $time = 600) {
    $transaction = db_transaction();
    $t = static::$table;
    $now = time();
    $limit = (int) $limit;
    // This is MySQL specific and there is no abstraction in Drupal for it.
    $result = db_query("SELECT * FROM {{$t}} WHERE LOCKED<$now ORDER BY CREATED LIMIT $limit LOCK IN SHARE MODE");
    $items = [];
    $ids = [];
    foreach ($result as $row) {
      $row->locked = $now + $time;
      $item = new static($row, FALSE);
      $ids[] = $row->id;
      $items[] = $item;
    }
    if ($ids) {
      db_update($t)
        ->fields(['locked' => $now + $time])
        ->condition('id', $ids)
        ->execute();
    }
    return $items;
  }

  /**
   * Bulk delete queue items based on their list.
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
   * Prepare a new queue item instance.
   */
  public function __construct($data = array(), $new = TRUE) {
    parent::__construct($data, $new);
    if (!isset($this->created)) {
      $this->created = time();
    }
  }

  /**
   * Lock this item for $time seconds.
   *
   * @param int $time
   *   Seconds to lock this item for.
   */
  public function claim($time = 600) {
    $this->locked = time() + $time;
    $this->save();
  }

  /**
   * Release the lock on this item.
   */
  public function release() {
    $this->locked = 0;
    $this->save();
  }

  /**
   * Check whether an opt-in email should be sent.
   */
  public function optIn() {
    return !empty($this->args['send_optin']);
  }

  /**
   * Check whether a welcome email should besent.
   */
  public function welcome() {
    return !empty($this->args['send_welcome']);
  }

}
