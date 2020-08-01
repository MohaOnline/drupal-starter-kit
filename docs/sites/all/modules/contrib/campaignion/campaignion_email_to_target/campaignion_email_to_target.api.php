<?php

/**
 * @file
 * Documentation for hooks invoked by this module.
 *
 * This file does not contain actually executed code.
 */

use Drupal\little_helpers\System\FormRedirect;
use Drupal\little_helpers\Webform\Submission;

/**
 * Declare selection mode plugins.
 *
 * @return array
 *   Associative array containing plugin metadata keyed by a unique ID.
 *   Each array item is an associative array with the following keys:
 *   - class: The FQN of the plugin class.
 *   - title: A title shown in the
 */
function hook_campaignion_email_to_target_selection_modes() {
  $plugins['custom'] = [
    'class' => '\\Drupal\\campaignion_email_to_target\\SelectionMode\\Custom',
    'title' => t('Some special way of selecting targets.'),
  ];
  return $plugins;
}

/**
 * Alter exclusion redirects.
 *
 * When an exclusion matches and it’s configured to redirect other modules can
 * manipulate the redirect destination using this hook.
 *
 * @param \Drupal\little_helpers\System\FormRedirect $redirect
 *   The redirect to be altered.
 * @param \Drupal\little_helpers\Webform\Submission $submission
 *   The submission that is about to be finished.
 */
function hook_campaignion_email_to_target_redirect_alter(FormRedirect &$redirect, Submission $submission) {
  $redirect->query['utm_source'] = 'my-tracking-parameter';
}
