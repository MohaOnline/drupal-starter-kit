<?php

use \Drupal\campaignion_activity\WebformSubmission;

function webform_campaignion_activity_type_info() {
  $info['webform_submission'] = 'Drupal\campaignion_activity\WebformSubmissionType';
}

/**
 * Helper function to log a webform activity.
 */
function _campaignion_activity_webform_log($node, $submission) {
  try {
    $activity = WebformSubmission::fromSubmission($node, $submission);
    $activity->save();
  } catch (Exception $e) {
    watchdog('campaignion_activity', 'Error when trying to log activity: !message', array('!message' => $e->getMessage()), WATCHDOG_WARNING);
  }
}

/**
 * Implements hook_webform_submission_insert().
 */
function campaignion_activity_webform_submission_insert($node, $submission) {
  _campaignion_activity_webform_log($node, $submission);
}

/**
 * Implements hook_webform_submission_update().
 */
function campaignion_activity_webform_submission_update($node, $submission) {
  _campaignion_activity_webform_log($node, $submission);
}

/**
 * Implements hook_webform_confirm_email_email_confirmed().
 */
function campaignion_activity_webform_confirm_email_email_confirmed($node, $submission) {
  if (!($activity = WebformSubmission::bySubmission($node, $submission))) {
    watchdog('campaignion_activity', 'Trying to confirm a not yet logged submission: !nid, !sid', array('!nid' => $node->nid, '!sid' => $submission->sid), WATCHDOG_WARNING);
    try {
      $activity = WebformSubmission::fromSubmission($node, $submission);
    } catch (Exception $e) {
      watchdog('campaignion_activity', 'Error when trying to log activity: !message', array('!message' => $e->getMessage()), WATCHDOG_WARNING);
    }
  }
  $activity->confirmed = REQUEST_TIME;
  $activity->save();
}

