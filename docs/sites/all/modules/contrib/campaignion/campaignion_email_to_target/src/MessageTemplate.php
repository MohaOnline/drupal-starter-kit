<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\little_helpers\DB\Model;

/**
 * Model class for message/exclusion templates.
 */
class MessageTemplate extends Model {

  protected static $table = 'campaignion_email_to_target_messages';
  protected static $key = ['id'];
  protected static $values = [
    'nid',
    'weight',
    'type',
    'label',
    'subject',
    'header',
    'message',
    'footer',
    'url',
  ];
  protected static $types = [
    'message' => Message::class,
    'message-template' => Message::class,
    'exclusion' => Exclusion::class,
  ];

  public $id;
  public $nid;
  public $weight = 0;
  public $type = 'message';
  public $label = '';
  public $subject = '';
  public $header = '';
  public $message = '';
  public $footer = '';
  public $filters = [];
  public $url = NULL;

  /**
   * Create new instance by passing the data.
   *
   * @param mixed $data
   *   The data as loaded from the database.
   * @param bool $new
   *   TRUE if the data was not yet saved to the database, otherwise FALSE.
   */
  public function __construct($data = [], $new = TRUE) {
    parent::__construct($data, $new);
    $filters = $this->filters;
    $this->filters = [];
    $this->setFilters($filters);
  }

  /**
   * Reset data based on an array.
   *
   * @param array $data
   *   New data.
   */
  public function setData(array $data = []) {
    foreach (static::$values as $k) {
      if ($k == 'nid') {
        continue;
      }
      if (isset($data[$k])) {
        $this->{$k} = $data[$k];
      }
    }
    if (isset($data['filters'])) {
      $this->setFilters($data['filters']);
    }
  }

  /**
   * Update the filter list to match the new filters.
   *
   * @param array $new_filters
   *   Array of new filters, which can be either associative data arrays or
   *   Filter instances.
   */
  public function setFilters(array $new_filters) {
    $old_filters = [];
    foreach ($this->filters as $f) {
      $old_filters[$f->id] = $f;
    }
    $w = 0;
    $filters = [];

    foreach ($new_filters as $nf) {
      if ($nf instanceof Filter) {
        $f = $nf;
      }
      else {
        // Reuse filter objects if 'id' is passed and found.
        if (isset($nf['id']) && isset($old_filters[$nf['id']])) {
          $f = $old_filters[$nf['id']];
          $f->setData($nf);
          unset($old_filters[$f->id]);
        }
        // Create a new filter object.
        else {
          $f = Filter::fromArray($nf);
        }
      }
      $f->message_id = $this->id;
      $f->weight = $w++;
      $filters[] = $f;
    }
    $this->filters = $filters;
    // Remove all filters that are not reused.
    foreach ($old_filters as $f) {
      $f->delete();
    }
  }

  /**
   * Get a list of message templates by their their nid.
   *
   * Messages are ordered by their weight.
   *
   * @param int $nid
   *   Node ID of the action.
   *
   * @return array
   *   Array of MessageTemplate objects keyed by their id.
   */
  public static function byNid($nid) {
    $result = db_select(static::$table, 'm')
      ->fields('m')
      ->condition('nid', $nid)
      ->orderBy('weight')
      ->execute();
    $messages = [];
    foreach ($result as $row) {
      $messages[$row->id] = new static($row, FALSE);
    }
    foreach (Filter::byMessageIds(array_keys($messages)) as $filter) {
      $messages[$filter->message_id]->filters[] = $filter;
    }
    return $messages;
  }

  /**
   * Get an array representation for this template as used in the API.
   *
   * @return array
   *   An array representation of this template.
   */
  public function toArray() {
    $data = [];
    foreach (array_merge(static::$key, static::$values) as $k) {
      $data[$k] = $this->$k;
    }
    $filters = [];
    foreach ($this->filters as $f) {
      $filters[] = $f->toArray();
    }
    $data['filters'] = $filters;
    $data['urlLabel'] = $this->urlLabel($data['url']);
    // Weights are only represented by order.
    unset($data['weight']);
    unset($data['nid']);
    return $data;
  }

  /**
   * Calculate a display version for a node URL.
   *
   * @param string $url
   *   The URL.
   * @return string
   *   A pretty label for the URL if available otherwise the URL itself.
   */
  protected function urlLabel($url) {
    if ((substr($url, 0, 5) == 'node/') && ($node = menu_get_object('node', 1, $url))) {
      return "{$node->title} [{$node->nid}]";
    }
    return $url;
  }

  /**
   * Save this template to the database.
   */
  public function save() {
    parent::save();
    foreach ($this->filters as $f) {
      $f->message_id = $this->id;
      $f->save();
    }
  }

  /**
   * Delete this template from the database.
   */
  public function delete() {
    parent::delete();
    foreach ($this->filters as $f) {
      $f->delete();
    }
  }

  /**
   * Check whether all the template’s filters match the given target.
   *
   * @param array $target
   *   Target data as received by the e2t-api.
   *
   * @return bool
   *   TRUE if all filters matched the target, otherwise FALSE.
   */
  public function checkFilters(array $target) {
    foreach ($this->filters as $f) {
      if (!$f->match($target)) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Create a new instance of this template.
   */
  public function createInstance() {
    $class = static::$types[$this->type];
    $vars = array_intersect_key(get_object_vars($this), get_class_vars($class));
    return new $class($vars);
  }

  /**
   * Clear out all IDs in order to create a real copy.
   */
  public function __clone() {
    $this->id = NULL;
    $this->new = TRUE;
    $filters = [];
    foreach ($this->filters as $f) {
      $filters[] = clone $f;
    }
    $this->filters = $filters;
  }

}
