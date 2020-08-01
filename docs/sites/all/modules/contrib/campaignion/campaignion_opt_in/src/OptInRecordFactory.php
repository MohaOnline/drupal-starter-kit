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
   * @param array $opt_in
   *   Opt-in data as returned from the webform component.
   */
  public function recordOptIn(array $opt_in) {
    if (in_array($opt_in['value'], [Values::OPT_IN, Values::OPT_OUT])) {
      db_insert('campaignion_opt_in')->fields([
        'activity_id' => $this->activity->activity_id,
        'operation' => $opt_in['value'] == Values::OPT_IN ? 1 : 0,
        'value' => $opt_in['raw_value'],
        'channel' => $opt_in['channel'],
        'statement' => $opt_in['statement'],
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
