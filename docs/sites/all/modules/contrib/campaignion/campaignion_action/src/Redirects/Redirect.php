<?php

namespace Drupal\campaignion_action\Redirects;

use Drupal\little_helpers\DB\Model;
use Drupal\little_helpers\Webform\Submission;

/**
 * Model class for redirects.
 */
class Redirect extends Model {
  const CONFIRMATION_PAGE = 0;
  const THANK_YOU_PAGE = 1;

  protected static $table = 'campaignion_action_redirect';
  protected static $key = ['id'];
  protected static $values = ['nid', 'delta', 'weight', 'label', 'destination'];

  public $id;
  public $nid;
  public $delta;
  public $weight = 0;
  public $filters = [];
  public $label = '';
  public $destination = '';

  /**
   * Construct a new redirect object.
   *
   * @param array|object $data
   *   Initial data for the new redirect object.
   * @param bool $new
   *   TRUE if this redirect should be treated as not yet in the database.
   */
  public function __construct($data = [], $new = TRUE) {
    parent::__construct($data, $new);
    $filters = $this->filters;
    $this->filters = [];
    $this->setFilters($filters);
  }

  /**
   * Reset data based on an array.
   */
  public function setData($data = []) {
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
   * Update the set of filters to match the input.
   *
   * @param array $new_filters
   *   New set of filters. Array members can either be Filter objects or array
   *   representations of filter objects.
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
      $f->redirect_id = $this->id;
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
   * Get a list of redirects by their their nid.
   *
   * Redirects are ordered by their weight.
   *
   * @param int $nid
   *   Node ID of the action.
   * @param int $delta
   *   The number of the redirect set.
   *
   * @return array
   *   Array of redirect objects keyed by their id.
   */
  public static function byNid($nid, $delta) {
    $result = db_select(static::$table, 'm')
      ->fields('m')
      ->condition('nid', $nid)
      ->condition('delta', $delta)
      ->orderBy('weight')
      ->execute();
    $redirects = [];
    foreach ($result as $row) {
      $redirects[$row->id] = new static($row, FALSE);
    }
    foreach (Filter::byRedirectIds(array_keys($redirects)) as $filter) {
      $redirects[$filter->redirect_id]->filters[] = $filter;
    }
    return $redirects;
  }

  /**
   * Export data to an array suitable for being serialized as JSON.
   *
   * @return array
   *   Nested array representing the redirect and all its filters.
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
    // Weights are only represented by order.
    unset($data['weight']);
    unset($data['nid']);
    unset($data['delta']);

    $data['prettyDestination'] = $data['destination'];
    if (substr($data['destination'], 0, 5) == 'node/') {
      if ($node = menu_get_object('node', 1, $data['destination'])) {
        $data['prettyDestination'] = "{$node->title} [{$node->nid}]";
      }
    }

    return $data;
  }

  /**
   * Save the redirect and all its filters to the database.
   */
  public function save() {
    parent::save();
    foreach ($this->filters as $f) {
      $f->redirect_id = $this->id;
      $f->save();
    }
  }

  /**
   * Delete the redirect and all its filters from the database.
   */
  public function delete() {
    parent::delete();
    foreach ($this->filters as $f) {
      $f->delete();
    }
  }

  /**
   * Check whether all filters match the given submission.
   *
   * @param Drupal\little_helpers\Webform\Submission $submission
   *   The submission to evaluate.
   *
   * @return bool
   *   TRUE if all filters match, otherwise FALSE.
   */
  public function checkFilters(Submission $submission) {
    foreach ($this->filters as $f) {
      if (!$f->match($submission)) {
        return FALSE;
      }
    }
    return TRUE;
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

  /**
   * Normalize redirect data-structure.
   *
   * @param mixed $r
   *   Either a string URL or an array of arguments for drupal_goto().
   *
   * @return array
   *   An array of arguments for drupal_goto().
   */
  public static function normalizeRedirect($r) {
    if (!is_array($r)) {
      $parsed_url = drupal_parse_url($r);
    }
    else {
      // In general $form_state['redirect'] is some input for drupal_goto().
      $parsed_url = isset($r[1]) && is_array($r[1]) ? $r[1] : [];
      $parsed_url['path'] = isset($r[0]) ? $r[0] : '';
    }
    $path = $parsed_url['path'];
    unset($parsed_url['path']);
    return [$path, $parsed_url];
  }

  /**
   * Get the normalized destination.
   *
   * @return array
   *   Normalized redirect destination.
   */
  public function normalized() {
    return static::normalizeRedirect($this->destination);
  }

}
