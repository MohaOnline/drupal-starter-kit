<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_expiry_variable_info($options) {
  $v['campaignion_expiry_cron_time_limit'] = [
    'title' => t('Time limit for expiry cron-jobs'),
    'description' => t('When an expiry cron-job has been running for more than this amount of seconds no new batch will be started during this cron-run.'),
    'type' => 'number',
    'default' => 20,
    'localize' => FALSE,
  ];
  $v['campaignion_expiry_submission_time_frame'] = [
    'title' => t('Submission expiry time frame'),
    'description' => t('Define a time frame after which form submissions should be anonymized. This must be a string valid for strtotime(). Example: "-12 months" would expire submissions after a year.'),
    'type' => 'string',
    'default' => '-12 months',
    'localize' => FALSE,
  ];
  $v['campaignion_expiry_contact_time_frame'] = [
    'title' => t('Contact expiry time frame'),
    'description' => t('Contacts that didn’t have any associated activity within that time-frame will be anonymized. This must be a string valid for strtotime(). Example: "-12 months" would expire contacts without activity in the past year.'),
    'type' => 'string',
    'default' => '-12 months',
    'localize' => FALSE,
  ];
  $v['campaignion_expiry_contact_keep_mp_data'] = [
    'title' => t('Contact expiry: keep MP data'),
    'description' => t('A flag that decides whether MP data is kept when a contact is anonymized.'),
    'type' => 'boolean',
    'default' => TRUE,
    'localize' => FALSE,
  ];
  return $v;
}
