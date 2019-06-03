<?php

/**
 * @file
 * Hook documentation for the hooks invoked by the campaignion_opt_in module.
 */

/**
 * Add opt-in communication channels.
 */
function hook_campaignion_opt_in_channel_info() {
  $channels['pigeon']['title'] = t('Carrier pigeon');
  return $channels;
}

/**
 * Alter the list of opt-in communication channels.
 *
 * @param array $channels
 *   Reference to the list of the channels.
 */
function hook_campaignion_opt_in_channel_info_alter(array &$channels) {
  $channels['mobile']['title'] = t('Mobile');
  unset($channels['phone']);
}
