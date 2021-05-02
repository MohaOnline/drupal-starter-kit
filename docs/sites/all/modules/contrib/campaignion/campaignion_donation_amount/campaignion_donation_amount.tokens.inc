<?php

/**
 * @file
 * Token hook implementations for campaignion_donation_amount.
 */

use Drupal\campaignion_donation_amount\Utils;
use Drupal\little_helpers\Webform\Submission;

/**
 * Implements hook_token_info().
 *
 * Provide a custom token for the total donation amount.
 */
function campaignion_donation_amount_token_info() {
  $info['tokens']['submission']['amount-total'] = [
    'name' => t('Donation amount total'),
    'description' => t('Calculates the sum of two donation amount fields'),
  ];
  return $info;
}

/**
 * Implements hook_tokens().
 */
function campaignion_donation_amount_tokens($type, $tokens, array $data = [], array $options = []) {
  // Return early unless submission tokens are needed and there is a submission.
  if ($type != 'submission' || empty($data['webform-submission']) || !webform_variable_get('webform_token_access')) {
    return [];
  }

  $node = $data['node'] ?? node_load($data['webform-submission']->nid);
  $submission = new Submission($node, $data['webform-submission']);
  $amount = Utils::submissionTotal($submission);

  $replacements = [];
  foreach ($tokens as $name => $original) {
    switch ($name) {
      case 'amount-total':
        $replacements[$original] = $amount;
        break;
    }
  }
  return $replacements;
}
