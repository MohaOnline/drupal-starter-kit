<?php

/**
 * @file
 * Token hook implementations for campaignion_giftaid.
 */

use Drupal\little_helpers\Webform\Submission;

/**
 * Implements hook_token_info().
 *
 * Provide a custom token for giftaid.
 */
function campaignion_giftaid_token_info() {
  $info['tokens']['submission']['amount-including-giftaid'] = [
    'name' => t('Donation amount including Gift Aid'),
    'description' => t('The donation amount increased by 25%. Note that this does not account for whether there was an actual opt-in for Gift Aid.'),
  ];
  return $info;
}

/**
 * Implements hook_tokens().
 */
function campaignion_giftaid_tokens($type, $tokens, array $data = [], array $options = []) {
  // Return early unless submission tokens are needed and node and submission
  // are available.
  if ($type != 'submission' || empty($data['webform-submission']) || empty($data['node']) || !webform_variable_get('webform_token_access')) {
    return [];
  }

  $submission = new Submission($data['node'], $data['webform-submission']);
  $donation_amount = $submission->valueByKey('donation_amount');

  $replacements = [];
  foreach ($tokens as $name => $original) {
    switch ($name) {
      case 'amount-including-giftaid':
        $giftaid_amount = $donation_amount * 1.25;
        $replacements[$original] = number_format($giftaid_amount, 2);
        break;
    }
  }
  return $replacements;
}
