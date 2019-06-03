<?php

namespace Drupal\campaignion_newsletters;

use Drupal\little_helpers\DB\Model;

/**
 * DB-Model for {campaignion_newsletters_lists}.
 */
class NewsletterList extends Model {

  public $list_id;
  public $source;
  public $identifier;
  public $language;
  public $title;
  public $data;
  public $updated;

  protected static $table = 'campaignion_newsletters_lists';
  protected static $key = array('list_id');
  protected static $values = [
    'source',
    'identifier',
    'title',
    'language',
    'data',
    'updated',
  ];
  protected static $serial = TRUE;
  protected static $serialize = array('data' => TRUE);

  /**
   * Load all lists from one source.
   */
  public static function bySource($source) {
    return static::loadQuery(['source' => $source]);
  }

  /**
   * Generic function to load using conditions and order criteria.
   */
  protected static function loadQuery($conditions = [], $order_by = []) {
    $q = db_select(static::$table, 'l')->fields('l');
    foreach ($conditions as $field => $value) {
      if (is_numeric($field)) {
        list($field, $value, $op) = $value;
      }
      else {
        $op = NULL;
      }
      $q->condition($field, $value, $op);
    }
    foreach ($order_by as $field => $direction) {
      $q->orderBy($field, $direction);
    }
    $lists = [];
    foreach ($q->execute() as $row) {
      $lists[$row->list_id] = new static($row);
    }
    return $lists;
  }

  /**
   * Get list of all newsletter lists sorted by title.
   */
  public static function listAll() {
    return static::loadQuery([], ['title' => 'ASC']);
  }

  /**
   * Load a list by it’s ID.
   */
  public static function load($id) {
    if ($rows = static::loadQuery(['list_id' => $id])) {
      return $rows[$id];
    }
  }

  /**
   * Load all lists not seen since a specific time.
   */
  public static function notUpdatedSince($time) {
    return static::loadQuery([['updated', $time, '<']]);
  }

  /**
   * Load list by source and identifier.
   */
  public static function byIdentifier($source, $identifier) {
    $rows = static::loadQuery([
      'source' => $source,
      'identifier' => $identifier,
    ]);
    return reset($rows);
  }

  /**
   * Load or create a list from a data object or array.
   */
  public static function fromData($data) {
    $adata = array();
    foreach ($data as $k => $v) {
      $adata[$k] = $v;
    }
    if ($item = self::byIdentifier($data['source'], $data['identifier'])) {
      unset($adata['list_id']);
      $item->__construct($adata);
      return $item;
    }
    else {
      return new static($data, TRUE);
    }
  }

  /**
   * Initialize a new list instance.
   */
  public function __construct($data = array(), $new = FALSE) {
    parent::__construct($data, $new);
    foreach ($data as $k => $v) {
      $this->$k = (is_string($v) && !empty(self::$serialize[$k])) ? unserialize($v) : $v;
    }
    if (!isset($this->language)) {
      $this->language = language_default('language');
    }
  }

  /**
   * Get the newsletter provider of this list.
   *
   * @return \Drupal\campaignion_newsletter\ProviderInterface
   *   The newletter provider of this list.
   */
  public function provider() {
    return ProviderFactory::getInstance()->providerByKey($this->source);
  }

  /**
   * Subscribe a single email-address to this newsletter.
   */
  public function subscribe($email, $fromProvider = FALSE) {
    $fields = array(
      'list_id' => $this->list_id,
      'email' => $email,
    );
    // MySQL supports multi-value merge queries, drupal does not so far,
    // so we could replace the following by a direct call to db_query().
    db_merge('campaignion_newsletters_subscriptions')
      ->key($fields)
      ->fields($fields)
      ->execute();

    if (!$fromProvider) {
      QueueItem::byData(array(
        'list_id' => $this->list_id,
        'email' => $email,
        'action' => QueueItem::SUBSCRIBE,
      ))->save();
    }
  }

  /**
   * Unsubscribe an email address from this list.
   */
  public function unsubscribe($email, $fromProvider = FALSE) {
    db_delete('campaignion_newsletters_subscriptions')
      ->condition('list_id', $this->list_id)
      ->condition('email', $email)
      ->execute();

    if (!$fromProvider) {
      QueueItem::byData(array(
        'list_id' => $this->list_id,
        'email' => $email,
        'action' => QueueItem::UNSUBSCRIBE,
      ))->save();
    }
  }

  /**
   * Save this list to the database.
   */
  public function save($updated = TRUE) {
    if ($updated) {
      $this->updated = REQUEST_TIME;
    }
    module_invoke_all('campaignion_newsletters_list_presave', $this);
    parent::save();
    module_invoke_all('campaignion_newsletters_list_saved', $this);
  }

  /**
   * Delete this list.
   */
  public function delete() {
    $transaction = db_transaction();
    parent::delete();
    module_invoke_all('campaignion_newsletters_list_deleted', $this);
  }

  /**
   * Delete stale newsletter lists.
   *
   * For most newsletter providers we don’t get any notification when a list is
   * deleted. So we keep track of when a list was last seen when pollig lists
   * and delete all lists that haven’t been seen in a while.
   */
  public static function deleteStaleLists() {
    $threshold = variable_get('campaignion_newsletters_last_list_poll', 0) - variable_get('campaignion_newsletters_list_expiry', 86400);
    $lists = static::notUpdatedSince($threshold);
    foreach ($lists as $list) {
      $list->delete();
    }
  }

}
