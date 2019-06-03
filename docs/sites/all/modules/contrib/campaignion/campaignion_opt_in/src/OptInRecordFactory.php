<?php

namespace Drupal\campaignion_opt_in;

use Drupal\campaignion_activity\ActivityBase;
use Drupal\campaignion_opt_in\Values;

/**
 * Generate opt-in records for an activity.
 */
class OptInRecordFactory {

  /**
   * The activity we are adding opt-in records to.
   *
   * @var Drupal\campaignion_activity\ActivityBase
   */
  protected $activity;

  /**
   * Create a factory for an activity.
   *
   * @param Drupal\campaignion_activity\ActivityBase $activity
   * The activity for which opt-in records will be added.
   */
  public function __construct(ActivityBase $activtiy) {
    $this->activity = $activtiy;
  }

  /**
   * Add a new opt-in record.
   *
   * @param array $component
   *   The webform component that is being processed.
   * @param mixed $value
   *   The submission value for the webform component.
   */
  public function recordOptIn($component, $values) {
    $value = Values::removePrefix($values);
    if (in_array($value, [Values::OPT_IN, Values::OPT_OUT])) {
      db_insert('campaignion_opt_in')->fields([
        'activity_id' => $this->activity->activity_id,
        'operation' => $value == Values::OPT_IN ? 1 : 0,
        'value' => reset($values),
        'channel' => $component['extra']['channel'],
        'statement' => $component['extra']['optin_statement'],
        'ip_address' => $this->getIpAddress(),
      ])->execute();
    }
  }

  /**
   * Return the user’s IP-address.
   */
  protected function getIpAddress() {
    return ip_address();
  }

}
