<?php
/**
 * @file
 */

/**
 * Implements hook_rules_data_info().
 */
function webform_confirm_email_rules_data_info() {
  return array(
    'webform-submission' => array(
      'label'         => t('Webform submission'),
      'wrap'          => TRUE,
      'property info' => _webform_confirm_email_rules_webform_submission_data_info(),
    ),
  );
}

/**
 * rules data property info callback.
 */
function _webform_confirm_email_rules_webform_submission_data_info() {
  return array(
    'sid' => array(
      'type'              => 'integer',
      'label'             => t('Webform submission ID'),
      'description'       => t('The webform submission ID.'),
      'setter permission' => FALSE,
    ),
    'nid' => array(
      'type'              => 'integer',
      'label'             => t('Node ID'),
      'description'       => t('The node ID where the webform belongs to.'),
      'setter permission' => FALSE,
    ),
    'submitted' => array(
      'type'              => 'integer',
      'label'             => t('Submission timestamp'),
      'description'       => t('Time when the webform was submitted.'),
      'setter permission' => FALSE,
    ),
    'remote_addr' => array(
      'type'              => 'text',
      'label'             => t('IP address of the submitter'),
      'description'       => t('The IP address of the submitter.'),
      'setter permission' => FALSE,
    ),
    'uid' => array(
      'type'              => 'user',
      'label'             => t('User'),
      'description'       => t('The Drupal user.'),
      'setter permission' => FALSE,
    ),
    'is_draft' => array(
      'type'              => 'boolean',
      'label'             => t('Submission is in draft mode'),
      'description'       => t('True if webform is in draft mode.'),
      'setter permission' => FALSE,
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 *
 * Define a rule event when an email was confirmed
 */
function webform_confirm_email_rules_event_info() {

  return array(
    'webform_confirm_email_email_confirmed' => array(
      'label'     => t('A webform email address was confirmed.'),
      'group'     => t('Webform'),
      'module'    => 'webform_confirm_email',
      'variables' => array(
        'node' => array(
          'label'       => t('Drupal node object'),
          'type'        => 'node',
          'description' => t('Node object of the webform for which an email was confirmed.'),
        ),
        'submission' => array(
          'label'       => t('Webform submission'),
          'type'        => 'webform-submission',
          'description' => t('The webform submission for which an email was confirmed.'),
        )
      ),
    ),
  );
}