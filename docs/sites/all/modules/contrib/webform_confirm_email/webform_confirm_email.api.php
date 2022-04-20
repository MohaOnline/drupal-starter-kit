<?php

/**
 * @file
 * Document hooks invoked by this module.
 */

/**
 * React when a confirmation link was clicked.
 *
 * @param object $node
 *   The webform node of the the submission.
 * @param object $submission
 *   The confirmed webform submission.
 * @param bool $first_confirmation
 *   TRUE the first time a confirmation link for this submission was clicked.
 */
function hook_webform_confirm_email_email_confirmed($node, $submission, $first_confirmation) {
  db_insert('my_confirmed_submission_list')
    ->fields(['nid' => $node->nid, 'sid' => $submission->sid])
    ->execute();
}

/**
 * React on an email confirmation request that has expired according
 * to the maximum request lifetime that the admin had set
 *
 * @param $expired_submissions
 *   An associative array of submissions that have expired. It's indexed
 *   by nid's (node ID's), it's values are subarrays containing the sid's
 *   (webform submission ID's) that have expired for this nid
 */
function hook_webform_confirm_email_request_expired($expired_submissions) {

  $report = count($expired_submissions) . ' confirmation request have expired.';
  drupal_mail(
    'example',
    'notice',
    'analysis@example.com',
    language_default(),
    array(
      'subject' => 'Report expired submissions',
      'body'    => $report,
    ),
    'cron@example.com'
  );
}

/**
 * Alter the redirect after a successful confirmation.
 *
 * @param $redirect
 *  An associative representing the redirect. Using the following keys.
 *   - 'path': The path argument for drupal_goto().
 *   - 'code': The code argument for drupal_goto().
 *   The other keys are identical with the options argument for drupal_goto().
 * @param $node
 *   The node object of the webform for which an email was confirmed.
 * @param $submission
 *   The submission object of hereby confirmed submission.
 */
function hook_webform_confirm_email_confirmation_redirect_alter(&$redirect, $node, $submission) {
  // Add a share=node/{nid} as query-parameter to the URL.
  $redirect['query']['share'] = "node/{$node->nid}";
}
