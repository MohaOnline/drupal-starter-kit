<?php

/**
 * @file
 * Example implementation of hooks that are invoked by this module.
 * See: https://drupal.org/node/292 for more information about hooks.
 */

use \Drupal\campaignion_activity\ActivityInterface;

/** 
 * React to a newly logged activity.
 */
function hook_campaignion_activity_save(ActivityInterface $activity, ActivityInterface $original = NULL) {
  if (!($activity instanceof \Drupal\campaignion_activity\WebformSubmission)) {
    return;
  }

  if (!empty($activity->confirmed)) {
    watchdog('campaignion_activity', 'WebformSubmission has been confirmed.');
  }
}
