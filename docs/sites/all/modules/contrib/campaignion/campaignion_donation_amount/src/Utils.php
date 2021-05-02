<?php

namespace Drupal\campaignion_donation_amount;

use Drupal\little_helpers\Webform\Submission;

/**
 * Utility functions for donation amount components.
 */
abstract class Utils {

  /**
   * Check if a webform component is a donation amount component.
   */
  public static function isAmountComponent(array $component) {
    return $component['form_key'] == 'donation_amount' || in_array('donation-amount', explode(' ', $component['extra']['wrapper_classes'] ?? ''));
  }

  /**
   * Get all amount components of a node.
   */
  public static function getAmountComponents($node) {
    return array_filter($node->webform['components'], [static::class, 'isAmountComponent']);
  }

  /**
   * Calculate the (current) total amount of all amount components.
   */
  public static function submissionTotal(Submission $submission) {
    $amount = 0;
    foreach (static::getAmountComponents($submission->node) as $cid => $_) {
      $amount += $submission->valueByCid($cid) ?: 0;
    }
    return $amount;
  }

}
