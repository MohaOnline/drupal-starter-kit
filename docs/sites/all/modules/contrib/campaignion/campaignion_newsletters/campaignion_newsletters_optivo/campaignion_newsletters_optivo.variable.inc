<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_newsletters_optivo_variable_info($options) {
  $v['optivo_credentials'] = [
    'title' => t('Optivo API-credentials', [], $options),
    'type' => 'unknown',
  ];
  $v['campaignion_newsletters_optivo_unsubscribe_poll_size'] = [
    'title' => t('Unsubscribe batch size', [], $options),
    'description' => t('The number of subscriptions being checked for unsubscribes in one batch.', [], $options),
    'type' => 'number',
    'default' => 20,
    'localize' => FALSE,
  ];
  $v['campaignion_newsletters_optivo_unsubscribe_poll_interval'] = [
    'title' => t('Unsubscribe poll interval (seconds)', [], $options),
    'description' => t('The minimum amount of time between two unsubscribe checks of one subscription. Only subscriptions that haven’t been checked for at least this amount of time will be re-checked.', [], $options),
    'type' => 'number',
    'default' => 7 * 24 * 3600,
    'localize' => FALSE,
  ];
  return $v;
}
