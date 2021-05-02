<?php

namespace Drupal\campaignion_opt_in;

use Drupal\little_helpers\Webform\Submission;

/**
 * Namespace for form-value constants.
 */
class Values {

  const OPT_IN = 'opt-in';
  const OPT_OUT = 'opt-out';
  const NO_CHANGE = 'no-change';
  const NOT_SELECTED = 'not-selected';

  protected $submission;
  protected $_values;
  protected $_valuesPerChannel;

  /**
   * Convert form API values to stored values.
   *
   * @param mixed $value
   *   Form API value to convert.
   * @param string $component
   *   The webform component.
   *
   * @return string
   *   Prefixed value for storing in webform_submitted_data.
   */
  public static function addPrefix($value, $component) {
    if (is_array($value)) {
      $value = reset($value);
    }
    if (strpos($value, ':') !== FALSE) {
      // There is already a prefix.
      return $value;
    }

    $display = $component['extra']['display'];
    if (!$value) {
      if ($display == 'radios') {
        $value = static::NOT_SELECTED;
      }
      else {
        $value = static::checkboxValues($component)[1];
      }
    }
    return $display . ':' . $value;
  }

  /**
   * Split a value into its display and value part.
   *
   * @param mixed $value
   *   The value to split.
   *
   * @return string[]
   *   Array with two items:
   *   - The display or an emtpy string if there was none.
   *   - The value.
   */
  public static function split($value) {
    if (is_array($value)) {
      $value = reset($value);
    }
    $parts = explode(':', $value, 2);
    return count($parts) > 1 ? $parts : ['', $parts[0]];
  }

  /**
   * Convert stored values to form API values.
   *
   * @param mixed $value
   *   Stored value to convert.
   *
   * @return string
   *   Un-prefixed value for form API.
   */
  public static function removePrefix($value) {
    return static::split($value)[1];
  }

  /**
   * Return translated labels keyed by display and value.
   *
   * @return string[][]
   *   Translated labels.
   */
  protected static function labels() {
    return [
      'checkbox' => [
        Values::OPT_IN => t('Checkbox opt-in'),
        Values::NO_CHANGE => t('Checkbox no change'),
        Values::OPT_OUT => t('Checkbox opt-out'),
        Values::NOT_SELECTED => t('Checkbox hidden (no change)'),
      ],
      'checkbox-inverted' => [
        Values::OPT_IN => t('Inverted checkbox opt-in'),
        Values::NO_CHANGE => t('Inverted checkbox no change'),
        Values::OPT_OUT => t('Inverted checkbox opt-out'),
        Values::NOT_SELECTED => t('Inverted checkbox hidden (no change)'),
      ],
      'radios' => [
        Values::OPT_IN => t('Radio opt-in'),
        Values::NO_CHANGE => t('Radio no change'),
        Values::OPT_OUT => t('Radio opt-out'),
        Values::NOT_SELECTED => t('Radio not selected (no change)'),
      ],
    ];
  }

  /**
   * Get label for values stored in submitted data.
   *
   * @param mixed $values
   *   Values as stored in the submitted data for this component.
   */
  public static function labelByValue($values) {
    if (!$values) {
      return t('Unknown value');
    }
    list($display, $value) = static::split($values);
    if ($value === '') {
      return t('Private or hidden by conditionals (no change)');
    }
    $labels = static::labels();
    if (isset($labels[$display][$value])) {
      return $labels[$display][$value];
    }
    return t('Unknown value');
  }

  /**
   * Get availabe options for a component.
   *
   * @param array $component
   *   The webform component.
   *
   * @return string[]
   *   Labels for available options keyed by the prefixed value.
   */
  public static function optionsByComponent(array $component) {
    $display = $component['extra']['display'];
    $labels = static::labels()[$display];
    if (!empty($component['extra']['no_is_optout'])) {
      if ($display != 'radios' && !empty($component['extra']['disable_optin'])) {
        unset($labels[Values::OPT_IN]);
      }
      else {
        unset($labels[Values::NO_CHANGE]);
      }
    }
    else {
      unset($labels[Values::OPT_OUT]);
    }

    $prefixed_labels = [];
    foreach ($labels as $value => $label) {
      $prefixed_labels["$display:$value"] = $label;
    }
    return $prefixed_labels;
  }

  /**
   * Get the pair of values for a checkbox.
   *
   * @param array $component
   *   The webform component.
   *
   * @return string[]
   *   Array of two strings: The checked value and the unchecked value.
   */
  public static function checkboxValues(array $component) {
    $values = [static::OPT_IN, static::NO_CHANGE];
    if (!empty($component['extra']['no_is_optout'])) {
      $values[1] = static::OPT_OUT;
      if (!empty($component['extra']['disable_optin'])) {
        $values[0] = static::NO_CHANGE;
      }
    }
    if ($component['extra']['display'] == 'checkbox-inverted') {
      $values = array_reverse($values);
    }
    return $values;
  }

  /**
   * Check whether a submission has a opt-in for a channel.
   *
   * @param \Drupal\little_helpers\Webform\Submission $submission
   *   The submission to check.
   * @param string $channel
   *   The channel we are looking for.
   *
   * @return bool
   *   TRUE if the submitted values contain at least one opt-in for the channel.
   */
  public static function submissionHasOptIn(Submission $submission, $channel) {
    return $submission->opt_in->hasOptIn($channel);
  }

  /**
   * Create a new values instance for a submission.
   *
   * Usually such an object is attached to a submission in $submission->opt_in.
   */
  public function __construct(Submission $submission) {
    $this->submission = $submission;
  }

  /**
   * Lazy load values for this submission.
   */
  public function values() {
    if (!isset($this->_values)) {
      module_load_include('components.inc', 'webform', 'includes/webform');
      $submission = $this->submission;
      $this->_values = [];
      foreach ($submission->node->webform['components'] as $cid => $component) {
        if (webform_component_feature($component['type'], 'opt_in')) {
          $values = $submission->valuesByCid($cid);
          if ($opt_in = webform_component_invoke($component['type'], 'opt_in', $component, $values)) {
            $this->_values[$cid] = $opt_in;
          }
        }
      }
    }
    return $this->_values;
  }

  /**
   * Return all values keyed by channel.
   */
  public function valuesPerChannel($channel = NULL) {
    if (!isset($this->_valuesPerChannel)) {
      $this->_valuesPerChannel = [];
      foreach ($this->values() as $cid => $v) {
        $this->_valuesPerChannel[$v['channel']][$cid] = $v;
      }
    }
    if ($channel) {
      return $this->_valuesPerChannel[$channel] ?? [];
    }
    return $this->_valuesPerChannel;
  }

  /**
   * Check whether the submission has any matching value.
   */
  public function hasValue($channel, $value) {
    foreach ($this->valuesPerChannel($channel) as $v) {
      if ($v['value'] == $value) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Check whether a submission has any opt-in for a channel.
   *
   * @param string $channel
   *   The channel we are looking for.
   *
   * @return bool
   *   TRUE if the submitted values contain at least one opt-in for the channel.
   */
  public function hasOptIn($channel) {
    return $this->hasValue($channel, static::OPT_IN);
  }

  /**
   * Calculate the overall submission result for a channel.
   */
  public function canonicalValue($channel, $simple = FALSE) {
    $value = NULL;

    $priority = [
      static::NOT_SELECTED => 0,
      static::NO_CHANGE => 1,
      static::OPT_OUT => 2,
      static::OPT_IN => 3,
    ];

    foreach ($this->valuesPerChannel($channel) as $v) {
      if (!$value || ($priority[$value['value']] ?? -1) < ($priority[$v['value']] ?? -1)) {
        $value = $v;
      }
    }
    return $simple && $value ? $value['value'] : $value;
  }

}
