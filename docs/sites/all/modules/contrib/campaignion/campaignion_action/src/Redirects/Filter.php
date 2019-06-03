<?php

namespace Drupal\campaignion_action\Redirects;

use Drupal\little_helpers\DB\Model;
use Drupal\little_helpers\Webform\Submission;
use Drupal\campaignion_newsletters\Subscription;
use Drupal\campaignion_opt_in\Values;

/**
 * Model class for redirect filters.
 */
class Filter extends Model {
  protected static $table = 'campaignion_action_redirect_filter';
  protected static $key = ['id'];
  protected static $values = ['redirect_id', 'weight', 'type', 'config'];
  protected static $serialize = ['config' => TRUE];

  public $id;
  public $redirect_id;
  public $weight = 0;
  public $type;
  public $config = [];

  /**
   * Construct a new instance from an array (as given in the API).
   *
   * @param array $data
   *   Data representing the filter.
   */
  public static function fromArray(array $data) {
    $data += ['id' => NULL, 'weight' => 0];
    $filter = new static();
    $filter->id = $data['id'];
    $filter->setData($data);
    return $filter;
  }

  /**
   * Update filter data from array.
   */
  public function setData(array $data) {
    unset($data['id']);
    unset($data['redirect_id']);
    foreach (['weight', 'type'] as $k) {
      if (isset($data[$k])) {
        $this->{$k} = $data[$k];
        unset($data[$k]);
      }
    }
    $this->config = $data;
  }

  /**
   * Get filters for given redirect_ids.
   *
   * @param array $ids
   *   Redirect IDs to get the filters for.
   *
   * @return array
   *   Filters ordered by redirect_id and weight, and keyed by their Id.
   */
  public static function byRedirectIds(array $ids) {
    // DB queries doesn't work well with empty arrays in IN() clauses.
    if (!$ids) {
      return [];
    }
    $result = db_select(static::$table, 'f')
      ->fields('f')
      ->condition('redirect_id', $ids)
      ->orderBy('redirect_id')
      ->orderBy('weight')
      ->execute();
    $filters = [];
    foreach ($result as $row) {
      $filters[$row->id] = new static($row, FALSE);
    }
    return $filters;
  }

  /**
   * Dump data into an array (as used by the API).
   *
   * @return array
   *   Filter data as an array.
   */
  public function toArray() {
    $data = [];
    foreach (array_merge(static::$key, static::$values) as $k) {
      $data[$k] = $this->$k;
    }
    if (isset($data['config']) && is_array($data['config'])) {
      $config = $data['config'];
      unset($data['config']);
      $data += $config;
    }
    unset($data['weight']);
    unset($data['redirect_id']);
    return $data;
  }

  /**
   * Check whether the filter condition is fulfilled.
   *
   * @param Drupal\little_helpers\Webform\Submission $submission
   *   The submission to check.
   *
   * @return bool
   *   TRUE if the filter is fulfilled, otherwise FALSE.
   */
  public function match(Submission $submission) {
    switch ($this->type) {
      case 'submission-field':
        $value = $submission->valueByCid($this->config['field']);
        return $this->matchValue($value);

      case 'opt-in':
        $optin = $this->hasOptin($submission);
        return $this->config['value'] ? $optin : !$optin;
    }
    return FALSE;
  }

  /**
   * Check whether the contact has an opt-in after submitting this form.
   *
   * @param Drupal\little_helpers\Webform\Submission $submission
   *   The submission to check for an opt-in.
   *
   * @return bool
   *   TRUE if the contact will have an opt-in after submitting this form,
   *   otherwise FALSE.
   */
  protected function hasOptin(Submission $submission) {
    // Check for opt-ins or opt-outs in this form.
    $components = $submission->webform->componentsByType('opt_in');
    foreach ($components as $cid => $component) {
      if ($component['extra']['channel'] == 'email') {
        $value = Values::removePrefix($submission->valuesByCid($cid));
        if ($value == 'opt-in') {
          return TRUE;
        }
        elseif ($value == 'opt-out') {
          return FALSE;
        }
      }
    }

    // If there is at least one subscription then we assume we have an opt-in.
    if (module_exists('campaignion_newsletters')) {
      if ($email = $submission->valueByKey('email')) {
        if (Subscription::byEmail($email)) {
          return TRUE;
        }
      }
    }

    return FALSE;
  }

  /**
   * Evaluate filter with regard to a submission value.
   *
   * @param mixed $target_value
   *   The value to check.
   *
   * @return bool
   *   TRUE when the value matches the filter conditions, otherwise FALSE.
   */
  protected function matchValue($target_value) {
    $value = $this->config['value'];
    switch ($this->config['operator']) {
      case '==':
        return $target_value == $value;

      case '!=':
        return $target_value != $value;

      case 'contains':
        return strpos($target_value, $value) !== FALSE;

      case '!contains':
        return strpos($target_value, $value) === FALSE;

      case 'regexp':
        return (bool) preg_match("/$value/", $target_value);

      case '!regexp':
        return !preg_match("/$value/", $target_value);
    }
    return FALSE;
  }

  /**
   * Clear out all IDs in order to create a real copy.
   */
  public function __clone() {
    $this->id = NULL;
    $this->new = TRUE;
  }

}
