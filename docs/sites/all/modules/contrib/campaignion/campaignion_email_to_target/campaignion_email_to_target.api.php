<?php

/**
 * @file
 * Documentation for hooks invoked by this module.
 *
 * This file does not contain actually executed code.
 */

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
